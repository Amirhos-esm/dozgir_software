#ifndef _SIM800C_SMS_H_
#define _SIM800C_SMS_H_

#ifdef __cplusplus
extern "C" {
#endif



void setError(char * er);
void resetError();
bool isErrorOccured();
char * getError();


/* convert ut8 to hex string utf16/ucs2
 * output example => 0633064406270645
 * */
void write_utf8(uint8_t * utf8_d);
/*
 * convert hex string utf16 string to utf8 one
 * input example: 0633064406270645 => سلام
 * this function write output in input buffer.
 * */
void utf16_to_utf8(uint8_t * input);


bool sim800c_sendSms(const char *number, const char *utf8_payload);
/*
 *  index = index of SMS to read
 *  buffer= buffer to store data. length should be bigger than 500
 *  ret[0] => number
 *  ret[1] => payload
 */
char** sim800c_readSms(uint32_t index ,char *buffer);


#ifdef __cplusplus
}
#endif


#endif
