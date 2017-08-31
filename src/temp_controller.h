/*
 * temp_controller.h
 *
 *  Created on: 25 Aug 2017
 *      Author: christo
 */

#ifndef SRC_TEMP_CONTROLLER_H_
#define SRC_TEMP_CONTROLLER_H_

#include "temp.h"
#include "display_controller.h"

class cTempController
{
    enum
    {
        TC_SHOW_TEMP,
        TC_SHOW_HIGH,
        TC_SHOW_LOW,
        TC_SET_HIGH,
        TC_SET_LOW
    }mTempControllerState;

    cTemp *mTemp;
    cDisplayController *mDisplayController;
    uint8_t mCurrTemp;

    bool mBusy;

    void showTemp();
    uint8_t setMode();

public:
    cTempController(cTemp *temp, cDisplayController *displayController);
    virtual ~cTempController();

    void btnShortPress();
    void btnLongPress();
    void run();
};

#endif /* SRC_TEMP_CONTROLLER_H_ */
