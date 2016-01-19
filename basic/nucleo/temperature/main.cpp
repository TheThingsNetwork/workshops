// License: Revised BSD License, see LICENSE.TXT, (c)2015 Semtech

#include "mbed.h"
#include "lmic.h"
#include "debug.h"

#include "DHT.h"

#define LORAWAN_NET_ID (uint32_t) 0x00000000
// TODO: enter device address below, for TTN just set ???
#define LORAWAN_DEV_ADDR (uint32_t) 0x02031003
#define LORAWAN_ADR_ON 1
#define LORAWAN_CONFIRMED_MSG_ON 1
#define LORAWAN_APP_PORT 3//15

DHT sensor(A1, AM2302);

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

osjob_t initjob;
osjob_t sendFrameJob;
u1_t n = 0;

void os_getArtEui (uint8_t *buf) {} // ignore
void os_getDevEui (uint8_t *buf) {} // ignore
void os_getDevKey (uint8_t *buf) {} // ignore

float getTemperature() {

    int err = 1;
  
    while(err != 0) {
        wait(2.0f);
        err = sensor.readData();
    }
    
    return sensor.ReadTemperature(CELCIUS);
}

void onSendFrame (osjob_t* j) {
 
    char message[32];
    
    float temperature = getTemperature();
    printf("Temperature is %4.2f \r\n", temperature);
    
    sprintf(message, "%4.2f", temperature);
    
    int frameLength = strlen(message); // keep it < 32
    for (int i = 0; i < frameLength; i++) {
        LMIC.frame[i] = message[i];
    }
    int result = LMIC_setTxData2(LORAWAN_APP_PORT, LMIC.frame, 
        frameLength, LORAWAN_CONFIRMED_MSG_ON); // calls onEvent()
        
    os_setTimedCallback(j, os_getTime() + sec2osticks(60), onSendFrame);
}

void onInit (osjob_t* j) {
    LMIC_reset();
    LMIC_setAdrMode(LORAWAN_ADR_ON);
    LMIC_setDrTxpow(DR_SF12, 14);
    LMIC_setSession(LORAWAN_NET_ID, LORAWAN_DEV_ADDR, NwkSKey, ArtSKey);
    onSendFrame(NULL);
}

void onEvent (ev_t ev) { // called by lmic.cpp, see also oslmic.h
    debug_event(ev);
    if (ev == EV_TXCOMPLETE) {
        os_setCallback(&sendFrameJob, onSendFrame);
    }
}

int main (void) {
    debug_str("main\r\n");
    os_init();
    os_setCallback(&initjob, onInit);
    os_runloop(); // blocking
}
