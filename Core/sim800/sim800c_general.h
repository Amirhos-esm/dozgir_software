#ifndef SIM800_SIM800C_GENERAL_H_
#define SIM800_SIM800C_GENERAL_H_

#include "sim800c.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct{
  int year;
  int month;
  int day;
  int hour,minute,second;
  int timezone;

} time;

bool sim800c_getTime(time *t);
const char * sim800c_getTime_str();
bool sim800c_setTime(time *t);
bool sim800c_setTime_str(const char *str);


#ifdef __cplusplus
}
#endif
#endif /* SIM800_SIM800C_GENERAL_H_ */
