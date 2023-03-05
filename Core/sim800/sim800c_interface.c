#include "sim800c.h"
#include "BT.h"

#ifndef __weak
#define __weak
#endif

__weak void
on_RDY (char *str)
{
//	logln("RADY");
}

__weak void
on_NORMAL_POWER_DOWN (char *str)
{
//	logln("N power down");

}
__weak void
on_RING (char *str)
{
//	logln("Ring");
  sim800c_sendExeCommand ("H");
}
__weak void
on_NO_CARRIER (char *str)
{
//	logln("NO_CARRIER");
}
__weak void
on_plus (char *str)
{
  if (strcmp_m ("+CMTI", str))
    {
      int smsIndex = 0;
      if (sscanf (str, "%*[^,],%d", &smsIndex) == 1)
	{
	  logln("sms %d added to queue ", smsIndex);
	  smsQueueAdd (smsIndex);

	  return;
	}
    }
  else if (strcmp_m ("+BT", str))
    {
      BT_on_Urc (str);
    }
}

#ifdef _SIM800C_SMS_H_
extern bool
sim800c_call_onUrc (const char *str);
#endif

__weak void
on_urc (char *str)
{
  printf ("urc: ");
  printf ("%s\n", str);

#ifdef _SIM800C_SMS_H_
  if (sim800c_call_onUrc (str))
    return;
#endif

  if (strcmp_m ("+", str))
    on_plus (str);
  else if (strcmp_m ("RDY", str))
    on_RDY (str);
  else if (strcmp_m ("RING", str))
    on_RING (str);
  else if (strcmp_m ("NO CARRIER", str))
    on_NO_CARRIER (str);
  else if (strcmp_m ("NORMAL POWER DOWN", str))
    on_NORMAL_POWER_DOWN (str);

}

//#include "BT.h"
//__weak void on_BT(char *str)
//{
//	BT_on_Urc(str);
//}
//
//#include "SMS.h"
//__weak void on_plus(char *str)
//{
//	if (strcmp_m("+BT", str))
//	{
//		on_BT(str);
//	}
//	else if(strcmp_m("+CMTI", str)){
//		on_sms(str);
//		//delete sms
//		sim800c_sendWrtieCommand("+CMGD", "1,4");
//	}
//	else if(strcmp_m("+CUSD", str))
//	{
//		on_USD(str);
//	}
//	else if(strcmp_m("+CPIN: NOT READY",str))
//	{
//		HAL_NVIC_SystemReset();
//	}
//
//}

