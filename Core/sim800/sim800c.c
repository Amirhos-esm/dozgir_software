#include "sim800c.h"

#define available() (isDataAvailable)

static uint8_t cmdState = OK;
static const char *appendingData = NULL;
static uint32_t timeout = DEF_TIMEOUT;

static char rxBuffer[RX_BUFFER + 1]; // plus for \0
static volatile uint16_t buffer_index = 0U;
static volatile uint32_t mMillis = 0;
volatile uint8_t read;
volatile bool isDataAvailable = false;
volatile bool ignore = false;

bool strcmp_m(const char *str1, const char *str2)
{
	uint16_t in = 0;
	for (; 1; in++)
	{
		if (!str1[in])
			break;
		if (str1[in] != str2[in])
			return false;

	}
	return true;
}

static void sim800c_urcHandle(uint8_t data)
{
	if (millis() - mMillis > URC_TIMEOUT)
		return;
	if (buffer_index == RX_BUFFER)
		sim800c_onError("BUFFER OVERFLOW");
	rxBuffer[buffer_index++] = data;
	mMillis = millis();

}
void sim800c_onDataReceive(uint8_t data)
{
	if (ignore)
	{
		read = data;
		isDataAvailable = true;
	}
	else
	{
		sim800c_urcHandle(data);
	}

}
static uint8_t readData()
{
	isDataAvailable = false;
	return read;
}
void sim800c_strTransmit(const char *str)
{
	while (*str)
		sim800c_transmit(*str++);
}
void sim800c_setTimeout(uint32_t t)
{
	timeout = t;
}

char* sim800c_getBuffer()
{
	return rxBuffer;
}
uint16_t sim800c_bufferLength()
{
	return buffer_index;
}

void sim800c_releaseBuffer()
{
	buffer_index = 0;
	mMillis = millis();
	ignore = false;
}
uint8_t sim800c_getResult()
{
	return cmdState;
}

void sim800c_putData(const char *d)
{
	appendingData = d;
}
// urc format : +CMD: some text\0
void sim800c_loop()
{
	if (ignore)
	{
		sim800c_releaseBuffer();
		return;
	}
	if (millis() - mMillis <= URC_TIMEOUT)
		return;

	if (buffer_index < 8)
	{
		sim800c_releaseBuffer();
		return;
	}

	// ignore first \r\n
	uint16_t in;
	for (in = 0; (rxBuffer[in] == '\n') || (rxBuffer[in] == '\r'); in++)
	{
		if (in == 7)
		{
			sim800c_releaseBuffer();
			return;
		}
	}

	rxBuffer[buffer_index - 2] = '\0';
	char *ptr = rxBuffer + in;

	sim_dbg("{URC: %s}",ptr);
	on_urc(ptr);
	sim800c_releaseBuffer();
}

static void waitForNewLine()
{
	uint8_t last = 0;
	while (1)
	{
		if (available())
		{
			if (read == 10 && last == 13)
			{
				readData();
				break;
			}
			last = readData();
		}
		if (millis() - mMillis > timeout)
		{
			cmdState = TIMEOUT;
			break;
		}
	}
}

void readResponse(const char *cmd)
{

	static const char *ok_text = "OK\r\n";
	static const char *ok_text2 = "SEND OK\r\n";
	static const char *error_text = "ERROR\r\n";

	mMillis = millis();
	buffer_index = 0;
	cmdState = OK;
	uint8_t last = 0, mflag = 1, checkingIndex = 0;

	waitForNewLine();
	if (cmdState != OK)
	{
		rxBuffer[buffer_index] = '\0';
		return;
	}
	while (1)
	{

		if (available())
		{
			if (mflag == 1)
			{
				checkingIndex = 1;
				if (ok_text[0] == read)
				{
					mflag = 2;

				}
				else if (error_text[0] == read)
				{
					mflag = 3;
				}
				else if (ok_text2[0] == read)
				{
					mflag = 4;
				}
				else
					mflag = 0;
			}
			else if (mflag == 2)
			{
				if (ok_text[checkingIndex++] == read)
				{
					if (checkingIndex == 4 /*sizeof(ok_text)*/)
					{
						cmdState = OK;

						if (buffer_index > 4 /*sizeof(ok_text)*/)
							buffer_index -= (4 /*sizeof(ok_text)*/+ 1);
						else
							buffer_index = 0;

						rxBuffer[buffer_index] = '\0';
						break;
					}
				}
				else
					mflag = 0;

			}
			else if (mflag == 3)
			{
				if (error_text[checkingIndex++] == read)
				{
					if (checkingIndex == 7 /*sizeof(error_text)*/)
					{
						cmdState = ERROR;

						if (buffer_index > 7 /*sizeof(error_text)*/)
							buffer_index -= (7 /*sizeof(error_text)*/+ 1);
						else
							buffer_index = 0;

						rxBuffer[buffer_index] = '\0';
						break;
					}
				}
				else
					mflag = 0;

			}
			else if (mflag == 4)
			{
				if (ok_text2[checkingIndex++] == read)
				{
					if (checkingIndex == 9 /*sizeof(ok_text2)*/)
					{
						cmdState = OK;

						if (buffer_index > 9 /*sizeof(ok_text2)*/)
							buffer_index -= (9 /*sizeof(ok_text2)*/+ 1);
						else
							buffer_index = 0;

						rxBuffer[buffer_index] = '\0';
						break;
					}
				}
				else
					mflag = 0;

			}
			if (read == 10 && last == 13 && mflag == 0)
			{
				mflag = 1;
			}
			last = readData();
			if (buffer_index == RX_BUFFER)
				sim800c_onError("BUFFER OVERFLOW");
			rxBuffer[buffer_index++] = last;

		}
		if (millis() - mMillis > timeout)
		{
			cmdState = TIMEOUT;
			rxBuffer[buffer_index] = '\0';
			return;
		}
	}

}
static void anyOtherData()
{
	if (appendingData != NULL)
	{
		delay(5);
		sim800c_strTransmit(appendingData);
		appendingData = NULL;
		sim800c_transmit(0x1aU);
	}
}
 void begin_ignore()
{
	ignore = true;
}
// response +CMN: xxxx\r\n
void sim800c_sendTestCommand(const char *cmd)
{
	begin_ignore();
	sim800c_strTransmit("AT");
	sim800c_strTransmit((char*) cmd);
	sim800c_strTransmit("=?\n");
	anyOtherData();
	readResponse(cmd);

	sim_dbg("{cmd: AT%s=? time: %lu retCode: %d res: %s}", cmd,(cmdState==TIMEOUT?timeout:(millis()-mMillis)), cmdState,
			(sim800c_getBuffer()[0] == '\0' ? "null" : sim800c_getBuffer()));

}
void sim800c_sendReadCommand(const char *cmd)
{
	begin_ignore();
	sim800c_strTransmit("AT");
	sim800c_strTransmit((char*) cmd);
	sim800c_strTransmit("?\n");
	anyOtherData();
	readResponse(cmd);

	sim_dbg("{cmd: AT%s? time: %lu retCode: %d res: %s}", cmd,(cmdState==TIMEOUT?timeout:(millis()-mMillis)), cmdState,
			(sim800c_getBuffer()[0] == '\0' ? "null" : sim800c_getBuffer()));
}
void sim800c_sendExeCommand(const char *cmd)
{
	begin_ignore();
	sim800c_strTransmit("AT");
	sim800c_strTransmit((char*) cmd);
	sim800c_strTransmit("\n");
	anyOtherData();
	readResponse(cmd);

	sim_dbg("{cmd: AT%s time: %lu retCode: %d res: %s}", cmd,(cmdState==TIMEOUT?timeout:(millis()-mMillis)), cmdState,
			(sim800c_getBuffer()[0] == '\0' ? "null" : sim800c_getBuffer()));
}

void sim800c_sendWriteCommand(const char *cmd, const char *wrt)
{
	begin_ignore();
	sim800c_strTransmit("AT");
	sim800c_strTransmit(cmd);
	sim800c_strTransmit("=");
	sim800c_strTransmit(wrt);
	sim800c_strTransmit("\n");
	anyOtherData();
	readResponse(cmd);

	sim_dbg("{cmd: AT%s=%s time: %lu retCode: %d res: %s}", cmd, wrt,(cmdState==TIMEOUT?timeout:(millis()-mMillis)), cmdState,
			(sim800c_getBuffer()[0] == '\0' ? "null" : sim800c_getBuffer()));
}
