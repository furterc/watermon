/*
 * temp_controller.h
 *
 *  Created on: 25 Aug 2017
 *      Author: christo
 */

#ifndef SRC_TEMP_CONTROLLER_H_
#define SRC_TEMP_CONTROLLER_H_

#include <avr/io.h>
#include "temp.h"
#include "buzzer.h"
#include "display_controller.h"

#define WATER_DETECT_PORT   PORTD
#define WATER_DETECT_DDR    DDRD
#define WATER_DETECT_PIN    PD2

class cTempController
{
    enum
    {
        TC_SHOW_TEMP,
        TC_SHOW_HIGH,
        TC_SHOW_LOW,
        TC_SET_HIGH,
        TC_SET_LOW,
        TC_ERROR_TEMP,
        TC_ERROR_DETECT
    }mTempControllerState;

    cTemp *mTemp;
    cDisplayController *mDisplayController;
    cBuzzer *mBuzzer;

    uint8_t mCurrTemp;
    uint8_t mSetTemp;
    bool mBusy;

    void showTemp();
    uint8_t setMode();
public:
    cTempController(cTemp *temp, cDisplayController *displayController, cBuzzer *buzzer);
    virtual ~cTempController();

    void btnShortPress();
    void btnLongPress();
    void run();

    bool checkWater();
};

#endif /* SRC_TEMP_CONTROLLER_H_ */
