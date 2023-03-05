#include "sim800c_call.h"

#include "sim800c.h"
#include "stdint.h"
#include "string.h"

static callStatus status;
static void
(*dmtfCallback) (int) = NULL;
bool
sim800c_call_onUrc (const char *str)
{
  if (strcmp_m ("RING", str))
    {
      status = ringing;
      return true;
    }
  if (strcmp_m ("NO CARRIER", str) || strcmp_m ("BUSY", str))
    {

      sim800c_call_hangup ();
      return true;
    }
  if (strcmp_m ("+COLP:", str) && status == calling)
    {

      status = talking;
      return true;
    }
  if (strcmp_m ("+DTMF:", str))
    {
      int dmtf;
      if (sscanf (str, "+DTMF: %d", &dmtf) && dmtfCallback != NULL)
	dmtfCallback (dmtf);

      return true;
    }
  return false;
}
bool
sim800c_call (const char *number)
{
  sprintf (sim800c_getBuffer (), "D%s;", number);
  sim800c_setTimeout (5000);
  sim800c_sendExeCommand (sim800c_getBuffer ());
  if (sim800c_getResult () != OK)
    {
      sim800c_call_hangup ();
      return false;
    }
  sim800c_sendWriteCommand ("+COLP", "1");
  if (sim800c_getResult () != OK)
    {
      sim800c_call_hangup ();
      return false;
    }

  status = calling;
  return true;

}

bool
sim800c_call_hangup ()
{
  sim800c_setTimeout (5000);
  sim800c_sendExeCommand ("H");
  sim800c_sendWriteCommand ("+COLP", "0");
  status = call_idle;
  return true;
}

bool
sim800c_call_answer ()
{
  if (status == ringing)
    {
      sim800c_sendExeCommand ("A");
      if (sim800c_getResult () != OK)
	{
	  sim800c_call_hangup ();
	  return false;
	}
      status = talking;
      return true;
    }
  return false;
}

bool
sim800c_call_getNumber (char *buffer, uint8_t size)
{
  /*
   * at+clcc
   +CLCC: 1,0,2,0,0,"09150077970",129,""
   */
  sim800c_sendExeCommand ("+CLCC");
  if (sim800c_getResult () != OK || strlen (sim800c_getBuffer ()) < 10)
    {
      sim800c_call_hangup ();
      return false;
    }
  char *firstQuot, *secQuot;
  firstQuot = strchr (sim800c_getBuffer (), '"');
  if (firstQuot == NULL)
    {
      sim800c_call_hangup ();
      return false;
    }
  secQuot = strchr (firstQuot++, '"');
  if (secQuot == NULL)
    {
      sim800c_call_hangup ();
      return false;
    }
  *secQuot = '\0';
  strncpy (buffer, firstQuot, size);
  return true;
}
callStatus
sim800c_call_getStatus ()
{
  return status;
}
bool
sim800c_dtmf_enable ()
{
  sim800c_setTimeout (100);
  sim800c_sendWriteCommand ("+DDET", "1");
  if (sim800c_getResult () != OK)
    {
      return false;
    }
  return true;
}
bool
sim800c_dtmf_disable ()
{
  sim800c_sendWriteCommand ("+DDET", "0");
  if (sim800c_getResult () != OK)
    {
      return false;
    }
  return true;
}
void
sim800c_dtmf_register_cb (void
(*cb) (int))
{
  dmtfCallback = cb;
}
