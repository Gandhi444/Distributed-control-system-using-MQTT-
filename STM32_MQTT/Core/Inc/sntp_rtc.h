/*
 * sntp_rtc.h
 *
 *  Created on: Nov 12, 2022
 *      Author: anton_000
 */

#ifndef INC_SNTP_RTC_H_
#define INC_SNTP_RTC_H_
#include "main.h"
#include <stdio.h>

extern RTC_HandleTypeDef hrtc;

void SetRTC(uint64_t seconds,uint64_t ms);

#endif /* INC_SNTP_RTC_H_ */
