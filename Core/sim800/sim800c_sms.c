#include "sim800c.h"
#include "stdbool.h"
#include "stdint.h"
#include "string.h"
//#include "debug.h"

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)
#define logHere() __FILE__ " " STRINGIZE(__LINE__)

static bool errorOccured = false;
static char *error;
void setError(char *er) {
	errorOccured = true;
	error = er;

}
void resetError() {
	errorOccured = false;

}
bool isErrorOccured() {
	return errorOccured;
}
char* getError() {
	if (isErrorOccured())
		return error;
	else
		return "";
}

static void createContinuationByte(uint8_t count, uint32_t *uniCode,
		uint8_t *buffer) {
	for (uint8_t i = 0; i < count; i++) {
		buffer[count - i] = (*uniCode & 0b00111111) | 0x80;
		*uniCode = *uniCode >> 6;
	}
}
static uint8_t uniCode_to_Utf8(uint32_t uniCode, uint8_t *buffer) {
	if (uniCode < 128U) {
		buffer[0] = uniCode;
		return 1;
	} else if (uniCode < 2048U) {
		createContinuationByte(1, &uniCode, buffer);
		buffer[0] = (uniCode & 0b00011111) | 0b11000000;
		return 2;
	} else if (uniCode < 65536U) {
		createContinuationByte(2, &uniCode, buffer);
		buffer[0] = (uniCode & 0b00001111) | 0b11100000;
		return 3;
	} else if (uniCode < 2097152U) {
		createContinuationByte(3, &uniCode, buffer);
		buffer[0] = (uniCode & 0b00000111) | 0b11110000;
		return 4;
	}
	// node need to this section because unicode max is 1,114,112
//    else if(uniCode<134217728U){
//        createContinuationByte(4,&uniCode,buffer);
//        buffer[0]= (uniCode & 0b00000011) | 0b11111000;
//        return 5;
//    }
//    else if(uniCode<2147483648U){
//        createContinuationByte(5,&uniCode,buffer);
//        buffer[0]= (uniCode & 0b00000001) | 0b11111100;
//        return 6;
//    }
	return 0;
}

static bool isSurrogate(uint16_t firstByte) {
	if (firstByte < 55296 || firstByte > 56319) { // no surrogate
		return false;
	}
	return true;
}
// 0x0633 or 0x06330000 -> 1587
static uint32_t utf16_to_unicode(uint32_t input) {
	uint16_t firstByte = input >> 16;
	uint16_t secByte = input & 0xffff;
	if (firstByte == 0U) // for 0x0633
		return secByte;

	if (!isSurrogate(firstByte)) // for 0x06330000
		return firstByte;

	uint16_t highSurrogate = firstByte - 55296U; // 0xDC800 = 55296
	uint16_t lowSurrogate = secByte - 56320U;  // 0xDC00U =56320
	uint16_t sub = lowSurrogate;
	sub |= highSurrogate << 10;
	return 65536 + sub;
}

// 'a' -> 10
// 'e' | 'E' -> 14
static uint8_t hex_to_uint8(uint8_t hex) {
	if (hex >= '0' && hex <= '9')
		return hex - '0';
	if (hex >= 'a' && hex <= 'f')
		return hex - 'a' + 10;
	if (hex >= 'A' && hex <= 'F')
		return hex - 'A' + 10;

	return 0;
}

static uint32_t read_next(uint8_t *buffer, uint32_t *len) {
	uint32_t ret = 0;
	uint32_t i;
	for (i = 0; i < 8; i++) {
		if (buffer[i] == '\0') {
			setError(logHere());
			return 0;
		}
		ret = ret << 4;
		ret |= hex_to_uint8(buffer[i]);
		if (isErrorOccured())
			return 0;
		if (i == 4 - 1) {
			if (!isSurrogate(ret)) {
				i++; // for len
				break;
			}

		}
	}
	*len = i;
	return utf16_to_unicode(ret);
}
void utf16_to_utf8(uint8_t *input) {
	resetError();

	uint32_t index = 0;
	uint32_t len = 0;
	uint32_t uniCode;

	uint8_t utf8_filled = 0;
	while (input[index]) {
		uniCode = read_next(&input[index], &len); // len is 4 or 8 meaning 2 or 4 bytes
		index += len;
		if (isErrorOccured())
			return;
		uint8_t utf8Len = uniCode_to_Utf8(uniCode, &input[utf8_filled]); // utf8_len is from 1 to 4 . 0 mean error
		if (utf8Len == 0) {
			setError(logHere());
			return;
		}
		utf8_filled += utf8Len;
	}
	input[utf8_filled] = '\0';
}

static void readContinuationByte(uint8_t count, uint32_t *uniCode,
		uint8_t *buffer) {
	for (uint8_t i = 0; i < count; i++) {
		if (buffer[i] == '\0') {
			setError(logHere());
			return;
		}
		if (buffer[i] >> 6 != 0x02) {
			setError(logHere());
			return;
		}
		*uniCode = *uniCode << 6;
		*uniCode |= buffer[i] & 0b00111111;
	}
}
static uint8_t utf8_get_len(uint8_t first) {
	uint8_t ret = 0;
	if (first < 128U)
		return 1;
	for (uint8_t i = 7; i >= 0; i--) {

		if (first >> i & 0x01)
			ret++;
		else if (i >= 6) {
			setError(logHere()); // incorect first
			return 0;
		} else
			break;
	}
	return ret;
}
static uint32_t utf8_to_unicode(uint8_t *buffer, uint32_t *len) {
	*len = 0;
	uint32_t ret = 0;
	if (buffer[0] == '\0') {
		setError(logHere());
		return 0;
	}
	*len = utf8_get_len(buffer[0]);
	if (*len > 4) {
		setError(logHere());
		return 0;
	}
	if (*len == 1)
		return buffer[0];
	ret = buffer[0] & ((2 << (7 - *len)) - 1); // read first byte
	readContinuationByte(*len - 1, &ret, buffer + 1);
	if (isErrorOccured())
		return 0;
	return ret;
}
// buffer len at least 12 byte
static uint8_t unicode_to_utf16(uint32_t unicode, uint8_t *buffer) {
	if (unicode < 0x10000U) {
		sprintf((char*) buffer, "%04X", (uint16_t) unicode);
		return 2;
	}
	unicode -= 0x10000U;
	uint16_t highSurrogate = 55296U + (unicode >> 10 & 1023U);
	uint16_t lowSurrogate = 56320U + (unicode & 1023U);
	sprintf((char*) buffer, "%04X%04X", (uint16_t) highSurrogate,
			(uint16_t) lowSurrogate);
	return 4;
}
void write_utf8(uint8_t *utf8_d) {
	resetError();
	uint8_t mbuffer[12];
	uint32_t index = 0;
	uint32_t uniCode;
	uint32_t len = 0;
	while (utf8_d[index]) {
		uniCode = utf8_to_unicode(&utf8_d[index], &len);
		if (isErrorOccured())
			return;
		index += len;
		unicode_to_utf16(uniCode, mbuffer);
		if (isErrorOccured())
			return;
		sim800c_strTransmit((char*) mbuffer);

	}

}

extern void readResponse(const char *cmd);
extern void begin_ignore();

bool sim800c_sendSms(const char *number, const char *utf8_payload) {

	sim800c_sendWriteCommand("+CSMP", "49,167,0,8");
	if (sim800c_getResult() != OK) {
		return false;
	}
	sim800c_sendWriteCommand("+CMGF", "1");
	if (sim800c_getResult() != OK) {
		return false;
	}
	sim800c_sendWriteCommand("+CSCS", "\"HEX\"");
	if (sim800c_getResult() != OK) {
		return false;
	}

	begin_ignore();
	sim800c_strTransmit("AT+CMGS=\"");
	sim800c_strTransmit(number);
	sim800c_strTransmit("\"\n");
	delay(5);
	write_utf8((uint8_t*) utf8_payload);
	sim800c_transmit(0x1aU);
	readResponse("+CMGS");
	return sim800c_getResult() == OK;
}
char** sim800c_readSms(uint32_t index, char *buffer) {
	static char *ret[2];
	ret[0] = ret[1] = buffer;
	buffer[0] = '\0';
	sim800c_sendWriteCommand("+CMGF", "1");
	if (sim800c_getResult() != OK) {
		return ret;
	}
	sim800c_sendWriteCommand("+CSCS", "\"UCS2\"");
	if (sim800c_getResult() != OK) {
		return ret;
	}

	sprintf(buffer, "%lu", index);

	sim800c_sendWriteCommand("+CMGR", buffer);
	if (sim800c_getResult() != OK) {
		buffer[0] = '\0';
		return ret;
	}
	if (sscanf(sim800c_getBuffer(), "%*[^,],\"%80[^\"]", buffer) != 1) {
		buffer[0] = '\0';
		return ret;
	}
	utf16_to_utf8((uint8_t*) ret[0]);
	ret[1] = &ret[0][strlen(ret[0]) + 1];
	*ret[1] = '\0';

	char * str = strchr(sim800c_getBuffer(), '\n');
	if (str == NULL)
		return ret;
	str++;

	// terminate the string
	char *end =  strchr(str, '\r');
	if(end!=NULL)
		*end='\0';
	//

	utf16_to_utf8((uint8_t*) str);
	if(isErrorOccured())
		;
	strcpy(ret[1],str);


	return ret;

}
