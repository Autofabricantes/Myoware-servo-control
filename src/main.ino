/* This firmware implements a simple threshold-based myoelectric control system
that uses a Myoware EMG sensor as the control input. Instead of using the
hardware-processed EMG signal provided by the Myoware sensor, the raw EMG signal
is used. This signal is digitally processed to obtain a measure of its amplitude.
First, the signal baseline is lowered to 0 V and then rectified. Then, the running
moving average of the rectified signal is computed to have the amplitude of the
signal. This amplitude is compared to an activation threshold in order to move
a servomotor. */

#include <Servo.h>
#include <MsTimer2.h>

static const unsigned int EMG_CH1 = 0;
static const unsigned int ledPin = 13;
static const unsigned int sampleTime = 1;
static const double adcRef= 5.00;
static const unsigned int adcRes = 1023;
static const double adcConv = adcRef/adcRes;

volatile unsigned int emg = 0;
double emgMean = 0.00;
double emgMovav = 0.00;
double emgMvc = 0.00;
volatile unsigned int sampleOk = 0;
unsigned int i = 0, k = 0;

Servo finger;

/* sampling reads the ADC every 1 ms with the MsTimer2 interrupt. */
void sampling()
{
    emg = analogRead(EMG_CH1);
    sampleOk = 1; // sampleOk indicates that a new sample is ready to be processed
}

/* meanCalc computes the mean value of the EMG signal during a period of
meanSamples. The mean value of the signal is its baseline. This value is
subtracted from the measured EMG signals to have a baseline of 0 V. */
void meanCalc(unsigned int meanSamples)
{
    while(i < meanSamples)
    {
        delayMicroseconds(50);
        if(sampleOk==1)
        {
            sampleOk = 0;
            i++;
            emgMean = emgMean + emg*adcConv;
        }
    }
    i = 0;
    emgMean = emgMean/meanSamples;
}

/* movAv computes the running moving average of the EMG signal. First, the
baseline of the signal is lowered to 0 V, to be able to rectify it. The running
moving average rectifies and smooths the signal (acts as a low pass filter),
returning the amplitude of the signal. */
void movAv()
{
    double emgZero = emg*adcConv - emgMean; // Signal with 0 V baseline
    emgMovav = emgMovav*0.99 + abs(emgZero)*0.01; // Rectified and smoothed signal
}

/* mvcCalc computes the maximum voluntary contraction, the maximum force the
user is able to exert. This value is used to compute the activation threshold */
void mvcCalc(unsigned int mvcSamples)
{
    while(i < mvcSamples)
    {
        delayMicroseconds(50);
        if(sampleOk==1)
        {
            sampleOk = 0;
            i++;
            movAv();
            if(emgMovav > emgMvc) {
                emgMvc = emgMovav;
            }
        }
    }
    i = 0;
    emgMovav = 0.00;
}

/* blinkLED blinks a led "repeat" times with a "bTime" interval between on and off */
void blinkLED(unsigned int repeat,unsigned int bTime)
{
    for(i=0;i<repeat;i++)
    {
        digitalWrite(ledPin,HIGH);
        delay(bTime);
        digitalWrite(ledPin,LOW);
        delay(bTime);
    }
}

void setup() {
    pinMode(EMG_CH1,INPUT);
    pinMode(ledPin,OUTPUT);
    finger.attach(9);
    finger.writeMicroseconds(1800);
    MsTimer2::set(sampleTime, sampling); // Set the sampling function to run every sampleTime (1 ms in this case)
    MsTimer2::start();
    Serial.begin(115200);

    /* System calibration */
    delay(5000);
    /* Calibration step #1: calculate the baseline of the signal during 10 s */
    blinkLED(1,500); // LED blinks once to indicate calibration step #1 start
    meanCalc(10000);
    blinkLED(1,500); // LED blinks once to indicate calibration step #1 end
    delay(1000);
    /* Calibration step #2: calculate the maximum voluntary contraction during 5 s*/
    blinkLED(2,500); // LED blinks twice to indicate calibration step #2 start
    mvcCalc(5000);
    blinkLED(2,500); // LED bliks twice to indicate calibration step #2 end
}

void loop() {
    delayMicroseconds(50);
    if(sampleOk) {
        sampleOk = 0;
        movAv(); // Gets the amplitude of the measured EMG signal

        /* If the amplitude of the EMG signal is greater than the activation threshold
        (a 35% of the MVC), the servo moves from its rest position and the LED turns on. */
        if(emgMovav > 0.35*emgMvc) {
            finger.writeMicroseconds(1200);
            digitalWrite(ledPin, HIGH);
        }
        /* If the amplitude of the EMG signal is below the activation threshold,
        the servo moves to its rest position and the LED turns off. */
        else {
            finger.writeMicroseconds(1800);
            digitalWrite(ledPin, LOW);
        }
    }
    // Serial.println(emgMovav);
}
