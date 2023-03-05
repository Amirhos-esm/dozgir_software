#include "mainCodes.h"
#include "cppQueue.h"

cppQueue smsQueue (sizeof(int), 10);

extern "C"
{
  void
  init_uart (); // toPort.c
  uint8_t ls = 0;
  uint8_t temp = 0;
  uint8_t dummyBuffer[300];
  volatile bool tapFlag = false, longTapFlag = false, touchFlag = false;

  void
  HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin)
  {
    if (GPIO_Pin == GPIO_PIN_11)
      {
	static uint32_t lastTouch = 0;
	if (readPin(userKey) == HIGH)
	  { // rising edge
	    touchFlag = true;
	    lastTouch = millis() - lastTouch;
	    if (lastTouch < 400)
	      return;
	    if (lastTouch < 2000)
	      {
		tapFlag = true;
	      }
	    else if (lastTouch < 10000)
	      {
		longTapFlag = true;
	      }
	    else if (lastTouch > 10000)
	      {
		HAL_NVIC_SystemReset ();
	      }
	  }
	else
	  { // falling edge
	    lastTouch = millis();
	  }
      }
  }


}


uint8_t
getMode ()
{

  return MODE_NONE;
}
void
speaker_add (uint8_t id)
{

}

void
smsQueueAdd (int index)
{
  smsQueue.push (&index);
}
void
smsQueueHandle ()
{
  if (smsQueue.isEmpty ())
    return;
  int r;
  smsQueue.pop (&r);
//	printf("address %x\n",	__get_MSP());
  char **ret = sim800c_readSms (r, (char*) dummyBuffer);
  printf ("sms from %s: %s\n", ret[0], ret[1]);

  sim800c_sendWriteCommand ("+CMGD", "1,4");

}

void
turnOffModule ()
{
  if (readPin(simStatus) == LOW)
    return;
  setPin(simPower, HIGH);
  setPin(simPower, LOW);
  delay(2000);
  setPin(simPower, HIGH);
}
void
turnOnModule ()
{
  if (readPin(simStatus) == HIGH)
    return;
  setPin(simPower, HIGH);
  setPin(simPower, LOW);
  delay(2000);
  setPin(simPower, HIGH);
}

void
printfRes (uint8_t s)
{
  if (s == OK)
    printf ("ok\n");
  else if (s == ERROR)
    printf ("error\n");
  else if (s == TIMEOUT)
    printf ("timeout\n");
  else
    printf ("noOne\n");
}

void
setup (void)
{

  // for gpio pa2 : swap uart
//	GPIOA->MODER |= 2 << 4;    // aleternate function push pull
//	GPIOA->OSPEEDR |= 3 << 4;  //high speed
  GPIO_InitTypeDef GPIO_InitStruct =
    { 0 };
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init (GPIOA, &GPIO_InitStruct);

  init_uart (); // toPort.c

  printf ("hello world\n");

//	player_init();
//	while(1);
//	player_play(0, 200, 98304);
//	player_play(1, 200, 98304);
  speaker_add (SOUND_ONBOOT);

  while (!W25qxx_Init ())
    {
      printf ("can`t start spi flash");
      speaker_add (SOUND_ERROR1);
      delay(10000);
      NVIC_SystemReset ();
    };

  if (!smsQueue.isInitialized ())
    {
      printf ("smsQueue: no more heap\n");
      NVIC_SystemReset ();
    }


  sim800c_sendWriteCommand ("+BTPOWER", "0");

  ssp_setReceiver(spp_data);
}
void
loop (void)
{
  sim800c_loop ();

  switch (ls)
    {
    case 0:
      setPin(statusLed, LOW);

      printf ("AT checking:");
      sim800c_setTimeout (100);
      sim800c_sendExeCommand ("");
      sim800c_sendExeCommand ("");
      if (sim800c_getResult () == OK)
	{
	  ls = 2;
	  printf ("-module is on\n");
	}
      else
	{
	  ls++;
	  printf ("-module is off\n");
	}

      break;
    case 1:
      printf ("turning module on...\n");
      turnOffModule ();
      delay(6000);
      turnOnModule ();
      delay(6000);
      ls = 0;
      break;
    case 2:
      ls = 120; // set correct ipr and ate
      break;
      /////////////////////////////////////////////////////////////////////////////
    case 4: // check simcard.base
      printf ("checking Simcard:");
      speaker_add (SOUND_SIM_CHECK);
      ls++;
      break;
    case 5:
      sim800c_setTimeout (1000);
      sim800c_sendReadCommand ("+CPIN");
      if (sim800c_getResult () == OK)
	{
	  if (strcmp_m ("+CPIN: READY", sim800c_getBuffer ()))
	    {
	      printf ("checked\n");
	      ls += 2;
	    }
	  else // need pinCode{
	    {
	      printf ("pinCode needed\n");
	      speaker_add (SOUND_SIM_NEEDPIN);
	      ls++;
	      turnOffModule ();
	    }
	}
      else if (sim800c_getResult () == ERROR)
	{
	  printf ("no simCard\n");
	  speaker_add (SOUND_SIM_INSERT_PLZ);
	  ls++;
	  turnOffModule ();
	}
      else if (sim800c_getResult () == TIMEOUT)
	{
	  printfRes (sim800c_getResult ());
	}
      break;
    case 6: // wait for reset to recheck simcard
      break;
    case 7: // sim check over
      ls++;
      break;
      /////////////////////////////////////////////////////////////////////////////
    case 8: // check network.base
      printf ("waiting for network.");
      speaker_add (SOUND_NETWORK_SEARCH);
      ls++;
      temp = 0;
      break;
    case 9:
      sim800c_setTimeout (100);
      sim800c_sendReadCommand ("+CREG");
      if (sim800c_getResult () == OK
	  && (strcmp_m ("+CREG: 0,1", sim800c_getBuffer ())))
	{
	  printf ("done\n");
	  ls += 2;
	  break;
	}
      temp++;
      printf (".");
      delay(1000);
      if (temp == 30)
	{
	  ls++;
	  printf ("no network\n");
	}
      break;
    case 10:
      // no network
      speaker_add (SOUND_NETWORK_NOTFOUND);
      delay(15000);
      ls -= 2;
      break;
    case 11:
      // check network finish
      ls = 20;
      break;
      /////////////////////////////////////////////////////////////////////////////
    case 20: // normal mode
      printf ("unSecure mode activated\n");
      speaker_add (SOUND_UNSECUREMODE);
      ls++;
      break;
    case 21:
      smsQueueHandle ();
      if (tapFlag)
	{
	  ls += 43;
	  tapFlag = false;
	}
      break;
      /////////////////////////////////////////////////////////////////////////////

    case 64:
      printf ("setting Mode\n");
      sim800c_setTimeout(DEF_TIMEOUT);
      ls++;
      break;
    case 65:
      printf ("Check BT name:");
      sim800c_sendReadCommand ("+BTHOST");
      if (sim800c_getResult () != OK)
	break;
      if (strcmp_m ("+BTHOST: MyDozgir", sim800c_getBuffer ()))
	{
	  ls++;
	  printf ("checked\n");
	  break;
	}
      printf ("Changing BT Name\n");
      sim800c_sendWriteCommand ("+BTHOST", "\"MyDozgir\"");
      if (sim800c_getResult () == OK)
	ls++;
      break;
    case 66:
      printf ("enabling BT:");
      sim800c_sendWriteCommand ("+BTPOWER", "1");
      if (sim800c_getResult () != TIMEOUT)
	{
	  printf ("BT is Enable\n");
	  ls++;
	  speaker_add (SOUND_BT_IS_ENABLE);
	}
      else printfRes(sim800c_getResult());
      break;
    case 67: // bt is enable here
      if (tapFlag)
	{
	  ls++;
	  tapFlag = false;
	}
      break;
    case 68:
      printf ("Disabling BT:");
      sim800c_sendWriteCommand ("+BTPOWER", "0");
      if (sim800c_getResult () != TIMEOUT)
	{
	  printf ("BT is Disable\n");
	  speaker_add (SOUND_BT_IS_DISABLE);
	  ls++;
	}
      else printfRes(sim800c_getResult());
      break;

    case 69:
      ls -= 49;
      break;
      /////////////////////////////////////////////////////////////////////////////
    case 120:
      printf ("checking IPR:");
      sim800c_setTimeout (1000);

      sim800c_sendReadCommand ("+IPR");
      if (sim800c_getResult () == OK)
	{
	  if (strcmp_m ("+IPR: 38400", sim800c_getBuffer ()))
	    {
	      ls = 4;
	      printf ("done\n");

	    }
	  else
	    {
	      printf ("change needed\n");
	      ls++;

	    }
	}
      else
	printfRes (sim800c_getResult ());
      break;
    case 121:
      printf ("setting correct IPR & ATE:");
      sim800c_setTimeout (1000);
      sim800c_sendExeCommand ("E1");
      if (sim800c_getResult () != OK)
	{
	  printf ("error\n");
	  break;
	}
      sim800c_sendWriteCommand ("+IPR", "38400");
      if (sim800c_getResult () == OK)
	{
	  printf ("ok\n");
	  ls++;
	}
      break;
    case 122:
      printf ("saving:");
      sim800c_setTimeout (1000);
      sim800c_sendExeCommand ("&W");
      if (sim800c_getResult () == OK)
	{
	  ls = 4;
	  ;
	  printf ("ok\n");
	}
      else
	printf ("error\n");
      break;

    }
}

