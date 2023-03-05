#ifndef INC_BT_H_
#define INC_BT_H_
#include "stdbool.h"

#ifdef __cplusplus
extern "C"{
#endif



typedef void (*btCallback)(char *) ;

void BT_on_Urc(char *str);
bool ssp_sendData(const char *str);
void ssp_setReceiver(btCallback);

#ifdef __cplusplus
}
#endif

#endif /* INC_BT_H_ */
