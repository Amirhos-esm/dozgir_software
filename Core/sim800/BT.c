#include "BT.h"
#include "mainCodes.h"
btCallback callback=NULL;

void ssp_setReceiver(btCallback c){
    callback = c;
}
bool ssp_sendData(const char *str)
{

	sim800c_setTimeout(500);
	sim800c_putData(str);
	sim800c_sendExeCommand("+BTSPPSEND");
	sim800c_setTimeout(DEF_TIMEOUT);
	return sim800c_getResult()==OK;
}

//static void resp()
//{
//	ssp_sendData("Bad Args\r\n");
//}
//static void splitConfigs(const char *ptr)
//{
//	logln("Splitting %s", ptr);
//	int opId = 0;
//	if (sscanf(ptr, "set=%d,", &opId) != 1 || opId < 0)
//	{
//		resp();
//		loglnL2("fail.operator id: %d", opId);
//		return;
//	}
//	loglnL2("OP ID:%d", opId);
//
//	ptr = strchr(ptr, ',');
//	if (ptr == NULL)
//	{
//		return;
//	}
//	ptr++;
//	int numCnt = 0;
//	if (sscanf(ptr, "%d,", &numCnt) != 1 || numCnt <= 0 || numCnt > NUM_MAX)
//	{
//		resp();
//		loglnL2("Fail.number count %d", numCnt);
//		return;
//	}
//	loglnL2("Numbers:%d", numCnt);
//
//	eeprom_write_begin();
//	eeprom_write(ADD_NUMCNT, numCnt);
//
//	char mBuffer[NUM_SIZE + 1];
//
//	uint8_t address = ADD_NUM1;
//	while (numCnt--)
//	{
//		ptr = strchr(ptr, ',');
//		if (ptr == NULL)
//		{
//			loglnL2("null pointer");
//			resp();
//			eeprom_write_end();
//			return;
//		}
//		ptr++;
//		if (sscanf(ptr, "%19[0-9+]", mBuffer) != 1)
//		{
//			loglnL2("Parse error at number %d : %s", numCnt, ptr);
//			resp();
//			eeprom_write_end();
//			return;
//		}
//		eeprom_write_n(address, (uint8_t*) mBuffer, strlen(mBuffer) + 1);
//		address += NUM_SIZE + 1;
//		loglnL2("number#%d: %s written", numCnt + 1, mBuffer);
//	}
//
//	eeprom_write(ADD_OP_ID, opId);
//	eeprom_write(ADD_BEH, 0x10);
//	eeprom_write(ADD_STR, 0xaa);
//	eeprom_write_end();
//
//	ssp_sendData("OK\r\n");
//	logln("Write OK");
//
//}
//static void resetConfigs()
//{
//	if (isConfiged())
//	{
//		eeprom_write_begin();
//		eeprom_write(ADD_NUMCNT, 0xff);
//		eeprom_write(ADD_OP_ID, 0xff);
//		eeprom_write(ADD_BEH, 0xff);
//		eeprom_write(ADD_STR, 0xff);
//		eeprom_write_end();
//	}
//	ssp_sendData("OK\r\n");
//}
//static void readConfigs()
//{
//	if (!isConfiged())
//	{
//		ssp_sendData("Error\r\n");
//		return;
//	}
//	char *mbuffer = sim800c_getBuffer();
//	sprintf(mbuffer, "opId: %d\r\n", getOperatorId());
//	char number[NUM_SIZE + 1];
//	for (uint8_t i = 0; i < getNumberCounts(); i++)
//	{
//		mbuffer += strlen(mbuffer);
//		getAdmin(i, number);
//		sprintf(mbuffer, "number#%d: %s\r\n", i, number);
//	}
//	ssp_sendData(sim800c_getBuffer());
//}
//static void status()
//{
//	char *ptr = sim800c_getBuffer();
//	ssp_sendData("+HV: " HARDWARE_VERSION_ID "\r\n");
//
//	ssp_sendData("+SV: " SOFTWARE_VERSION_ID "\r\n");
//
//	sprintf(ptr, "+CONFIG: %d\r\n", isConfiged());
//	ssp_sendData(ptr);
//
//	sim800c_sendReadCommand("+CPIN");
//	ssp_sendData(sim800c_cmdState() == OK ? ptr : "+CPIN: Error\r\n");
//
//	sim800c_sendReadCommand("+CREG");
//	ssp_sendData(sim800c_cmdState() == OK ? ptr : "+CREG: Error\r\n");
//
//	sim800c_sendReadCommand("+COPS");
//	ssp_sendData(sim800c_cmdState() == OK ? ptr : "+COPS: Error\r\n");
//
//	sim800c_sendExeCommand("+CSQ");
//	ssp_sendData(sim800c_cmdState() == OK ? ptr : "+CSQ: Error\r\n");
//
//	sim800c_sendExeCommand("+CBC");
//	ssp_sendData(sim800c_cmdState() == OK ? ptr : "+CBC: Error\r\n");
//
//}
static void ssp_data(char *str)
{
	int length;
	if (sscanf(str, "%*19[^,],%d", &length) != 1)
		return;
	char *ptr = strchr(str, ',');
	if (ptr == NULL)
		return;
	ptr = strchr(ptr + 1, ',');
	if (ptr == NULL)
		return;
	ptr++;

	log("SSP Data: %s",ptr);
//	char * p=ptr;
//	while(*ptr)
//	  {
//	    log("%2x",*ptr++);
//	  }
//	log("\n");
	if(callback!=NULL)
	  callback(ptr);
//
//	if (strlen(ptr) != length)
//	{
//		loglnL2("SPP data no match to size");
//		return;
//	}
//	if (strcmp_m("status", ptr))
//	{
//		status();
//	}
//	else if (strcmp_m("set", ptr))
//	{
//		splitConfigs(ptr);
//	}
//	else if (strcmp_m("readConfigs", ptr))
//	{
//		readConfigs();
//	}
//	else if (strcmp_m("simreset", ptr))
//	{
//		ssp_sendData("OK\r\n");
//		ls += 3;
//	}
//	else if (strcmp_m("resetConfigs", ptr))
//	{
//		resetConfigs();
//	}
//	else if (strcmp_m("close", ptr))
//	{
//		ssp_sendData("OK\r\n");
//		ls++;
//	}
//	else if (strcmp_m("update", ptr))
//	{
//		setBootLoaderFlag();
//		HAL_NVIC_SystemReset(); // go to bootloader
//	}
//	else
//		ssp_sendData("Bad command\r\n");

}

void BT_on_Urc(char *str)
{

	if (strcmp_m("+BTCONNECTING", str))
	{
		sim800c_sendWriteCommand("+BTACPT", "1");
	}
	else if (strcmp_m("+BTCONNECT", str))
	{
		int index = 0;
		if (sscanf(str, "%*19[^ ]%d", &index) != 1)
			return;

		char *ret = strrchr(str, ',');
		if (ret == NULL)
			return;
		if (strcmp_m(",\"SPP\"", ret) /*&& index == 1*/)
		{
			// spp connect on index
			logln("SSP connect on %d\n", index);
		}
		else
		{
			char _buff[5];
			sprintf(_buff, "%d", index);
			delay(100);
			sim800c_sendWriteCommand("+BTDISCONN", _buff);
		}
	}
	else if (strcmp_m("+BTDISCONN", str))
	{

		char *ret = strrchr(str, ',');
		if (ret == NULL)
			return;
		if (strcmp_m(",\"SPP\"", ret))
		{
			// spp connect on index
			logln("SSP disconct \n");
		}
	}
	else if (strcmp_m("+BTOPPPUSHING", str))
	{

	}
	else if (strcmp_m("+BTOPPPUSH", str))
	{

	}

	else if (strcmp_m("+BTSPPDATA", str))
	{
		ssp_data(str);
	}
	else if (strcmp_m("+BTPAIRING", str))
	{
		sim800c_sendWriteCommand("+BTPAIR", "1,1");
	}

}

