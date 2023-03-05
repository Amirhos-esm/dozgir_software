//
// Created by AmirHos_PC on 4/12/2021.
//

#ifndef SIM800C_SIM800C_H
#define SIM800C_SIM800C_H


#include "sim800c_config.h"

#ifdef __cplusplus
extern "C" {
#endif




#define TIMEOUT 2
#define ERROR 1
#define OK 0

#define DEF_TIMEOUT 5000

void sim800c_onDataReceive(uint8_t data); // call on IT
void sim800c_transmit(uint8_t data);   // should implement by user
void sim800c_onError(const char *);

void sim800c_strTransmit(const char *str);
void sim800c_loop();
void sim800c_setTimeout(uint32_t);
void sim800c_releaseBuffer();
uint8_t sim800c_getResult();
char *sim800c_getBuffer();
uint16_t sim800c_bufferLength();
bool strcmp_m(const char *str1, const char *str2);

void sim800c_putData(const char *d);
void sim800c_sendTestCommand(const char *cmd);
void sim800c_sendReadCommand(const char *cmd);
void sim800c_sendExeCommand(const char *cmd);
void sim800c_sendWriteCommand(const char *cmd,const char *wrt);


#ifdef __cplusplus
}
#endif



#endif //SIM800C_SIM800C_H
