#ifndef MYINC_PLAYER_H_
#define MYINC_PLAYER_H_

#include "main.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif


#define prefetch_buffer_size 256
// only in software. you change timer settings on your own
#define dac_freq 	32000
#define audio_freq	16000


typedef enum {
	audio_idle = 0, audio_pause, audio_playing,audio_starting,audio_resuming
} audio_state;


void player_init();
void player_pause(uint8_t channel);
void player_resume(uint8_t channel);
void player_play(uint8_t channel,uint32_t address,uint32_t len);
void player_replay(uint8_t channel);
void player_abort(uint8_t channel);
bool player_isPlaying(uint8_t channel);
audio_state player_getState(uint8_t channel);



#ifdef __cplusplus
}
#endif

#endif /* MYINC_PLAYER_H_ */
