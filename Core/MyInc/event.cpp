#include "mainCodes.h"
#define UNSECURE_SETTINGS_BASE  0x23
#define SECURE_SETTINGS_BASE	0x23

#define SECURE_SECTOR 		4
#define UNSECURE_SECTOR 	6

#define SETTING_SEG_SIZE 256

#define EACH_SET_SIZE 5

#define MAX_NUMBER_LEN	30

//sector 0 for number/s

char numbers[4][MAX_NUMBER_LEN];


uint8_t getNumberCount(){


}
const char * getNumbers(){

}

void readCommand(const char * par,char * buffer){
  sim800c_sendReadCommand(par);
  if(sim800c_getResult()==OK)
    {
     char *s= strchr(sim800c_getBuffer(),' ');
     if(s==NULL)
       {
	 strcpy(buffer,"ERROR2");
       }
     else
       strncpy(buffer,s+1,40);
       }
  else if (sim800c_getResult()==ERROR){
      strcpy(buffer,"ERROR");
  }
  else  strcpy(buffer,"TIMEOUT");
}

void handle_StatusCommand(){
  char *cpin,*creg,*cops,*csq,*cbc;
  cpin=(char *)&dummyBuffer[0];
  creg=(char *)&dummyBuffer[50];
  cops=(char *)&dummyBuffer[100];
  csq=(char *)&dummyBuffer[200];
  cbc=(char *)&dummyBuffer[250];

  sim800c_setTimeout(100);

  readCommand("+CPIN",cpin);
  readCommand("+CREG",creg);
  readCommand("+COPS",cops);
  readCommand("+CSQ",csq);
  readCommand("+CBC",cbc);
  sprintf((char *)&dummyBuffer[300],"{\"cpin\":\"%s\",\"creg\":\"%s\",\"cops\":\"%s\",\"csq\":\"%s\",\"cbc\":\"%s\",\"version\":\"%s\"}"
	  ,cpin,creg,cops,csq,cbc,SOFTWARE);
  ssp_sendData ((char *)&dummyBuffer[300]);
}
void
spp_data (char *str)
{
  if (strcmp_m ("set1", str))
    {
      set_settings ((uint8_t*) str + 5, MODE_UNSECURE);
      ssp_sendData ("OK\r\n");
    }
  else if (strcmp_m ("set2", str))
    {
      set_settings ((uint8_t*) str + 5, MODE_SECURE);
      ssp_sendData ("OK\r\n");
    }
  else if (strcmp_m ("get", str))
    {
      int m, id;
      if (2 != sscanf (str, "get%d,%d", &m, &id))
	return;
      EventSetting e;
      get_setting (id, &e, m);
      sprintf (
	  (char*) dummyBuffer,
	  "{\"active\":%d,\"call\":%d,\"sms\":%d,\"alarm\":%d,\"speaker\":%d}\r\n",
	  e.active, e.call, e.sms, e.alarm, e.speaker);
      ssp_sendData ((char*) dummyBuffer);
    }
  else if (strcmp_m ("status", str))
    {
      handle_StatusCommand();
    }
}

void
set_settings (uint8_t *str, uint8_t mode)
{
  if (MODE_SECURE == mode)
    {
      W25qxx_EraseSector (SECURE_SECTOR);
      W25qxx_WriteSector (str, SECURE_SECTOR, 0, EACH_SET_SIZE * EVENT_COUNT);
    }
  else if (MODE_UNSECURE == mode)
    {
      W25qxx_EraseSector (UNSECURE_SECTOR);
      W25qxx_WriteSector (str, UNSECURE_SECTOR, 0, EACH_SET_SIZE * EVENT_COUNT);
    }
}

void
get_setting (uint8_t eventID, EventSetting *e, uint8_t mode)
{
  e->active = 0xff;
  uint8_t tBuff[EACH_SET_SIZE + 5];
  if (!mode)
    return;
  if (MODE_SECURE == mode)
    {
      W25qxx_ReadSector (tBuff, SECURE_SECTOR, eventID * EACH_SET_SIZE,
      EACH_SET_SIZE);
    }
  else if (MODE_UNSECURE == mode)
    {
      W25qxx_ReadSector (tBuff, UNSECURE_SECTOR, eventID * EACH_SET_SIZE,
      EACH_SET_SIZE);
    }
  e->active = tBuff[0];
  e->call = tBuff[1];
  e->sms = tBuff[2];
  e->alarm = tBuff[3];
  e->speaker = tBuff[4];
}

void
onEvent (uint8_t eventId)
{

  EventSetting setting;
  get_setting (eventId, &setting, getMode ());

  if (setting.active != 0xaa)
    return;

  if (setting.alarm >> 7 == 0)
    {
      // active alaram here
    }
  if (setting.speaker >> 7 == 0)
    {
      // play sound here
    }

  if (setting.sms >> 7 == 0)
    {
      // send sms here
    }

  if (setting.call >> 7 == 0)
    {
      // call here
    }

}
