#include "sim800c_general.h"



bool sim800c_getTime(time *t){
  sim800c_setTimeout(100);
  sim800c_sendReadCommand("+CCLK");
  if(sim800c_getResult()!=OK)
    return false;
  int parse = sscanf(sim800c_getBuffer(),"+CCLK: \"%d/%d/%d,%d:%d:%d%d\""
		     ,&t->year,&t->month,&t->day,&t->hour,&t->minute,&t->second,&t->timezone);
  if(parse==7)
    return true;
  return false;

}
const char * sim800c_getTime_str(){
  sim800c_setTimeout(100);
  sim800c_sendReadCommand("+CCLK");
  if(sim800c_getResult()!=OK)
    return NULL;
  char *firstQuot,*end;
  firstQuot=strchr(sim800c_getBuffer(),'"');
  if(firstQuot==NULL)
    return NULL;
  end=strchr(firstQuot++,'"');
   if(end==NULL)
     return NULL;
   end-=3;
   end='\0';
   return firstQuot;
}

bool sim800c_setTime(time *t){
  sprintf(sim800c_getBuffer(),"\"%2d/%2d/%2d,%2d:%2d:%2d%+2d\""
	  ,t->year,t->month,t->day,t->hour,t->minute,t->second,t->timezone);
  return sim800c_setTime_str(sim800c_getBuffer());
}
bool sim800c_setTime_str(const char *str){
  sim800c_setTimeout(100);
  sim800c_sendWriteCommand("+CCLK",str);
  if(sim800c_getResult()!=OK)
     return false;
  return true;
}
