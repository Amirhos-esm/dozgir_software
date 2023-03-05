//
// Created by AmirHos_PC on 4/12/2021.
//

#ifndef SIM800C_SIM800C_CONFIG_H
#define SIM800C_SIM800C_CONFIG_H

#include "string.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "debug.h"
#include "sim800c_sms.h"
#include "BT.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * some notes:
 *  	port this part
 */

#include "main.h" // needed for delay and debug
#define delay(X) HAL_Delay(X)
#define millis() HAL_GetTick()

/*
 *
 * */

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))


#define URC_TIMEOUT 40
#define RX_BUFFER 500
#define SIM_DEBUG_ENABLE 0



#if SIM_DEBUG_ENABLE==1
#define sim_dbg(...)	log(__VA_ARGS__)
#else
#define sim_dbg(...)
#endif




void on_urc(char *str);

#ifdef __cplusplus
}
#endif



#endif //SIM800C_SIM800C_CONFIG_H
