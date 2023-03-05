#ifndef __MAINCODES_H
#define __MAINCODES_H

#include "main.h"
#include "debug.h"
//#include "Player.h"
#include "w25qxx.h"
#include "sim800c.h"



#ifdef __cplusplus
extern "C"  {
#endif

#define HARDWARE	"v1.00"
#define SOFTWARE  	"v1.00"

#define readPin(X) HAL_GPIO_ReadPin(X)
#define setPin(X,Y) HAL_GPIO_WritePin(X, Y)
#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET

#define simPower 	simPower_GPIO_Port,simPower_Pin
#define simStatus 	simStatus_GPIO_Port,simStatus_Pin
#define statusLed 	statusLed_GPIO_Port,statusLed_Pin
#define userKey 	userKey_GPIO_Port,userKey_Pin

extern uint8_t dummyBuffer[];



#define SOUND_ONBOOT 		0
#define SOUND_ERROR1 		1
#define SOUND_SIM_CHECK		2
#define SOUND_SIM_NEEDPIN	3
#define SOUND_SIM_INSERT_PLZ	4
#define SOUND_NETWORK_SEARCH	5
#define SOUND_NETWORK_NOTFOUND	6
#define SOUND_UNSECUREMODE	7
#define SOUND_WARNING    	8
#define SOUND_BT_IS_ENABLE	9
#define SOUND_BT_IS_DISABLE	10


#define EVENT_COUNT		11
#define EVENT_DOOR_OPEN		0
#define EVENT_DOOR_CLOSE	1

#define EVENT_MOTION		2

#define EVENT_SHAKE		3

#define EVENT_POWER_GONE	4
#define EVENT_POWER_CAME	5

#define EVENT_LOW_CHARGE	6
#define EVENT_TURN_OFF		7

#define EVENT_LOW_SIM_CHARGE	8

#define EVENT_DEVICE_MOVE	9

#define EVENT_ALARM_DISCONNECTED	10


typedef struct
{
  uint8_t active; // 0xaa mean active other mean in active
  uint8_t call;	//first bit: 0 => active ; 1->inactive :7 remain bit is number id to call
  uint8_t sms;//first bit: 0 => active ; 1->inactive : 7 remain bit : each bit represent who to sms
  uint8_t alarm;  //first bit: 0 => active ; 1->inactive
  uint8_t speaker;//first bit: 0 => active ; 1->inactive : 7 remain bit : each bit represent who to sms

} EventSetting;


void setup(void);
void loop(void);
void smsQueueAdd(int index);// for add received sms


void spp_data(char *str);
void onEvent (uint8_t eventId);
void set_settings (uint8_t *str, uint8_t mode);
void get_setting (uint8_t eventID, EventSetting *e,uint8_t mode);

#define MODE_NONE	 0
#define MODE_UNSECURE 	1
#define MODE_SECURE  	 2
uint8_t getMode();


typedef struct {
  uint32_t address;
  uint32_t length;
} Sound_t;


#ifdef __cplusplus
}
#endif

#endif
