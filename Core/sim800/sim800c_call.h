#ifndef _SIM800C_SMS_H_
#define _SIM800C_SMS_H_

#include "stdbool.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif
  typedef enum
  {
    call_idle = 0, calling, talking, ringing

  } callStatus;


  bool
  sim800c_call (const char *number);

  bool
  sim800c_call_hangup ();
  bool
  sim800c_call_answer ();
  bool
  sim800c_call_getNumber (char *buffer, uint8_t size);

  callStatus
  sim800c_call_getStatus ();

  bool sim800c_dtmf_enable();
  bool sim800c_dtmf_disable();
  void sim800c_dtmf_register_cb(void (*cb)(int));

#ifdef __cplusplus
}
#endif

#endif
