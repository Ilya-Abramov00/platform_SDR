#include "platform_SDR/receiver.h"

#include "rtl-sdr.h"

#include <iostream>
#include <vector>

void Receiver::setSettingsReceiver(const ReceiverSettings& settings) {
    if(settings.direct_sampling) {
        setDirectSampling(settings.direct_sampling);
    }
    setSampleRate(settings.sampleFreq);
    setCenterFreq(settings.centralFreq);
    if(settings.gain == 0) {
        setAutoGain();
    } else {
        auto gain = nearestGain(settings.gain);
        setGain(gain);
    }
    setPpm(settings.ppm_error);
    setAgcMode(settings.agcMode);
}

/**
 * @brief Метод для установки центральной частоты.
 */
void Receiver::setCenterFreq(uint32_t freq) {
    auto r = rtlsdr_set_center_freq(dev, freq);
    if(r < 0)
        std::cerr << "WARNING: Failed to set center freq." << std::endl;

    freq = rtlsdr_get_center_freq(dev);
    std::cerr << "Tuned to sentral freq " << double(freq) / 1e3 << " kHz." << std::endl;
}

/**
 * @brief Метод для установки частоты дискретизации приемника.
 */
void Receiver::setSampleRate(uint32_t samp_rate) {
    auto r = rtlsdr_set_sample_rate(dev, samp_rate);
    if(r < 0) {
        std::cerr << "WARNING: Failed to set sample rate." << std::endl;
    }
    auto Fs = rtlsdr_get_sample_rate(dev);
    if(Fs <= 0) {
        std::cerr << "WARNING: Failed to get sample rate." << std::endl;
    }
    std::cerr << "Sampling at " << double(Fs) / 1e6 << " МГц" << std::endl;
}

/**
 * @brief Метод, который устанавливает автоматическое усисление
 * приемника.
 */
void Receiver::setAutoGain() {
    auto r = rtlsdr_set_tuner_gain_mode(dev, 0);
    if(r != 0)
        std::cerr << "WARNING: Failed to set tuner gain " << r << std::endl;
    else
        std::cerr << "Tuner gain set to automatic." << std::endl;
}

/**
 * @brief Метод для поиска ближайшего усиления, которое
 * возможно установить для данного приемника.
 */

int Receiver::nearestGain(int gain) {
    std::vector<int> gains;

    auto r = rtlsdr_set_tuner_gain_mode(dev, 1);
    if(r < 0) {
        std::cerr << "WARNING: Failed to enable manual gain." << std::endl;
        return r;
    }

    auto count = rtlsdr_get_tuner_gains(dev, nullptr);
    if(count <= 0)
        return 0;

    gains.resize(count);

    count = rtlsdr_get_tuner_gains(dev, gains.data());

    auto nearest = gains[0];
    for(auto i = 0; i < count; i++) {
        auto err1 = abs(gain - nearest);
        auto err2 = abs(gain - gains[i]);
        if(err2 < err1)
            nearest = gains[i];
    }

    return nearest;
}

/**
 * @brief Метод для установки усиления.
 */
void Receiver::setGain(int gain) {
    auto r = rtlsdr_set_tuner_gain_mode(dev, 1);
    if(r < 0) {
        std::cerr << "WARNING: Failed to enable manual gain" << r << std::endl;
        return;
    }

    r = rtlsdr_set_tuner_gain(dev, gain);
    if(r != 0)
        std::cerr << "WARNING: Failed to set tuner gain" << r << std::endl;
    else
        std::cerr << "Tuner gain set to" << gain / 10.0 << "dB" << std::endl;
}

/**
 * @brief Метод для установки режима Direct Sampling
 * 0 - Откючение режима прямой выборки
 * 1 - Включение режима прямой выборки для вывода I
 * 2 - Включение режима прямой выборки для вывода Q
 * 3 - Включен режим прямой выборки
 */
void Receiver::setDirectSampling(int on) {
    int r;
    r = rtlsdr_set_direct_sampling(dev, on);
    if(r != 0) {
        std::cerr << "WARNING: Failed to set direct sampling mode " << r << std::endl;
    }

    switch(on) {
        case(0):
            std::cerr << "Direct sampling mode disabled. " << std::endl;
            break;
        case(1):
            std::cerr << "Enabled direct sampling mode, input 1/I." << std::endl;
            break;
        case(2):
            std::cerr << "Enabled direct sampling mode, input 2/Q." << std::endl;
            break;
        case(3):
            std::cerr << "Enabled no-mod direct sampling mode." << std::endl;
            break;
    }
}

void Receiver::setPpm(int ppm_error) {
    if(ppm_error == 0)
        return;
    auto r = rtlsdr_set_freq_correction(dev, ppm_error);
    if(r < 0)
        std::cerr << "WARNING: Failed to set ppm error.\n";
    else
        std::cerr << "Tuner error set to " << ppm_error << "ppm.\n";
}

/**
 * @brief Функция для включения/выключения автоматического контроля усиления.
 * Он необходим для того, чтобы при большой мощности сигнала приемник
 * не ограничивал сигнал и можно было видеть его без искажений.
 * Например, без включения этого метода синусоиды при большой мощности
 * генератора приемник отсекал часть синусоиды сверху и снизу из-за этого часть
 * данных была неправильной.
 */
void Receiver::setAgcMode(int on) {
    auto r = rtlsdr_set_agc_mode(dev, on);
    if(r < 0) {
        std::cerr << "WARNING: Failed to reset buffers." << std::endl;
        return;
    }
    if(on)
        std::cerr << "AGC mode on\n";
    else
        std::cerr << "AGC mode off\n";
}

/**
 * @brief Метод для очиски буфера приемника. Необходимо вызывать каждый раз
 * перед чтением данных (асинхронным или синхронным)
 */

void Receiver::resetBuffer() {
    int r;
    r = rtlsdr_reset_buffer(dev);
    if(r < 0)
        std::cerr << "WARNING: Failed to reset buffers " << r << std::endl;
}

/**
 * @brief Метод для изменения полосы, но не понятно насколько он действильно меняет, но замечано, что он ломает счетчик
 */
void Receiver::setTunerBandwidth(uint32_t bw) {
    auto r = rtlsdr_set_tuner_bandwidth(
        dev, bw); // не понятно насколько влияет этот метод вообще, но замечано, что он ломает счетчик
    if(r < 0) {
        std::cerr << "WARNING: Failed set tuner_bandwidth " << std::endl;
    }
}

/**
 * @brief Метод для округление до ближайшей степни двойки вверх.
 * Он необходим для того, чтобы можно было правильно считать данные.
 */

uint32_t Receiver::roundPowerTwo(uint32_t& size) {
    auto result = size - 1;
    for(unsigned k = 0; k <= 4; ++k)
        result |= result >> (1 << k);
    ++result;
    return result;
}
Receiver::Receiver(rtlsdr_dev_t* dev) : dev(dev) { }
