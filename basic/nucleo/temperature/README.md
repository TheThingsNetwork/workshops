
# Temperature readings using NUCLEO-L152RE and Grove – Temperature&Humidity Sensor Pro

This example measures temperature each minute. The temperature reading is shipped off to
the things network over LoRaWAN.

## Hardware

![nucle](nucleo.jpg)

- [NUCLEO-L152RE](https://developer.mbed.org/platforms/ST-Nucleo-L152RE/)
- [Seeed Grove Shield V2 ](https://developer.mbed.org/components/Seeed-Grove-Shield-V2/)
- [Grove – Temperature&Humidity Sensor Pro](http://www.seeedstudio.com/depot/grove-temperaturehumidity-sensor-pro-p-838.html)


## Console output

    Temperature is 23.00
    Temperature is 23.00
    Temperature is 23.00
    Temperature is 23.00
    Temperature is 23.00
    Temperature is 23.00
    Temperature is 23.00
    Temperature is 23.00

# Code

The full code repository can be found on [embed.org](https://developer.mbed.org/users/dagga/code/lora-temperature/)

The main.cpp is repeated here:

    // License: Revised BSD License, see LICENSE.TXT, (c)2015 Semtech
    
    #include "mbed.h"
    #include "lmic.h"
    #include "debug.h"
    //#include <SoftwareSerial.h>
    
    #include "DHT.h" // Sensor library nececessary
    
    #define LORAWAN_NET_ID (uint32_t) 0x00000000
    // TODO: enter device address below, for TTN just set ???
    #define LORAWAN_DEV_ADDR (uint32_t) 0x02031002
    #define LORAWAN_ADR_ON 1
    #define LORAWAN_CONFIRMED_MSG_ON 1
    #define LORAWAN_APP_PORT 3//15
    
    static uint8_t NwkSKey[] = {
        // TODO: enter network, or use TTN default
        // e.g. for 2B7E151628AED2A6ABF7158809CF4F3C =>
        0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 
        0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
    };
    
    static uint8_t ArtSKey[] = {
        // TODO: enter application key, or use TTN default
        // e.g. for 2B7E151628AED2A6ABF7158809CF4F3C =>
        0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 
        0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
    };
    
    DHT sensor(PA_1,AM2302); // The sensor
    
    InterruptIn motion(A2); // PIR motion sensor
    int motion_detected = 0;
    
    void irq_handler(void)
    {
        motion_detected = 1;
    }
    
    osjob_t initjob;
    osjob_t sendFrameJob;
    u1_t n = 0;
    
    void os_getArtEui (uint8_t *buf) {} // ignore
    void os_getDevEui (uint8_t *buf) {} // ignore
    void os_getDevKey (uint8_t *buf) {} // ignore
    
    void checkTemp(osjob_t* t) {
        printf("Checking temperature\n");
        
        int err = 0;
    
        err = sensor.readData();
    
        while(err != 0) {
            wait(1);
            err = sensor.readData();
        }
    
        char message[25];
    
        float temp = sensor.ReadTemperature(CELCIUS);
        float humidity = sensor.ReadHumidity();
        sprintf(message, "%.3f", temp);
        
        int frameLength = strlen(message); // keep it < 32
    
        for (int i = 0; i < frameLength; i++) {
            LMIC.frame[i] = message[i];
        }
        int result = LMIC_setTxData2(LORAWAN_APP_PORT, LMIC.frame, frameLength, LORAWAN_CONFIRMED_MSG_ON);
            
    
        printf("Current temperature: %s\r\n", message);
            
        os_setTimedCallback(t, os_getTime() + sec2osticks(60), checkTemp);
    }
    
    
    int cnt = 0;
    
        
    void checkMotionSensor(osjob_t *t)
    {
        printf("checkMotionSensor \r\n");
        
        if (motion_detected) {
            cnt++;
            motion_detected = 0;
            printf("Hello!aaAAAAAAAAAAAAAAA I've detected %d times since reset\r\n", cnt);
            
            char message[25] = "motion detected";
            int frameLength = strlen(message); // keep it < 32
    
            for (int i = 0; i < frameLength; i++) {
                LMIC.frame[i] = message[i];
            }
    
            // Prepare upstream data transmission
            int result = LMIC_setTxData2(LORAWAN_APP_PORT, LMIC.frame, frameLength, LORAWAN_CONFIRMED_MSG_ON); 
        }
        
        os_setTimedCallback(t, os_getTime() + sec2osticks(1), checkMotionSensor);
        
    }
        
    void onInit (osjob_t* j) {
        LMIC_reset(); // Reset the MAC state
        LMIC_setAdrMode(LORAWAN_ADR_ON); // Enable data rate adaption
        LMIC_setDrTxpow(DR_SF12, 14); // Set data rate and transmit power
        LMIC_setSession(LORAWAN_NET_ID, LORAWAN_DEV_ADDR, NwkSKey, ArtSKey);
        
        motion.rise(&irq_handler);
        // os_setTimedCallback(j, os_getTime() + sec2osticks(1), checkMotionSensor);
        os_setTimedCallback(j, os_getTime() + sec2osticks(0), checkTemp);
    
        //onSendFrame(NULL);
    }
    
    void onEvent (ev_t ev) { // called by lmic.cpp, see also oslmic.h
        debug_event(ev);
        if (ev == EV_TXCOMPLETE) {
            // os_setCallback(&sendFrameJob, onSendFrame);
        }
    }
    
    int main (void) {
        debug_str("main\r\n");
        os_init();
        os_setCallback(&initjob, onInit);
    
        os_runloop(); // blocking
    }
