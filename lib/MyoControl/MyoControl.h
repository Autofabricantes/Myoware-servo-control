#ifndef MyoControl_h
#define MyoControl_h

#include <Arduino.h>

class MyoControl {
    public:
        MyoControl(uint8_t emg_pin);
        void sampling();
        void calibration();
        bool activation();
    private:
        void blinkLED(uint8_t ledPin, unsigned int repeat, unsigned int bTime);
        void meanCalc(unsigned int meanSamples);
        double movAv();
        void mvcCalc(unsigned int mvcSamples);
        uint8_t _emg_pin;
        volatile unsigned int emg;
        double emgMean;
        double emgMvc;
        bool sampleOk;
};

#endif
