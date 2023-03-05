#include "player.h"

#define segment_size 256

void playerll_init();
void playerll_pwm_set(uint8_t channel, uint32_t d);
uint32_t playerll_pwm_get(uint8_t channel);
void playerll_read(uint32_t address, uint32_t len, uint8_t *buffer);

void playerll_generate_software_it();

void playerll_dma_start(uint8_t channel, uint8_t *buff, uint32_t len);
void playerll_dma_stop(uint8_t channel);
void playerll_dma_pause(uint8_t channel);
void playerll_dma_unpause(uint8_t channel);

#if dac_freq  	 == 	32000 && audio_freq == 16000
uint8_t fileBuffer[prefetch_buffer_size / 2];

#elif dac_freq   == 	32000 && audio_freq == 8000
uint8_t fileBuffer[prefetch_buffer_size / 4];

#elif dac_freq   == 	16000 && audio_freq == 16000
uint8_t fileBuffer[prefetch_buffer_size];

#elif dac_freq   == 	16000 && audio_freq == 8000
uint8_t fileBuffer[prefetch_buffer_size/2];


#else
#error "only dac_freq 16k and 32k and audio_freq 8k and 16k supported"
#endif

static const int fileBufferSize = sizeof(fileBuffer);
volatile bool swHappend = false;

typedef struct {
	volatile audio_state state;
	uint8_t *audioBuffer; // next audio buffer
	uint8_t segment1[prefetch_buffer_size], segment2[prefetch_buffer_size];
	uint32_t address, length, inBuffer, laddress, llength;
	uint8_t channel_id;
	uint8_t dma_req_flag;
} channel_data;

//channel_data c1 = { .state = audio_idle, .channel_id = 0 }, c2 = { .state =
//		audio_idle, .channel_id = 1 };
channel_data c1, c2;

static channel_data* getChannel(uint8_t channel) {
	if (channel == 0)
		return &c1;
	else if (channel == 1)
		return &c2;
	return NULL;
}

void player_init() {
	playerll_init();
	c1.state = c2.state = audio_idle;
	c1.channel_id = 0;
	c2.channel_id = 1;

}
void player_pause(uint8_t channel) {
	channel_data *c = getChannel(channel);
	if (!c)
		return;
	if (c->state == audio_pause)
		return;
	c->state = audio_pause;
	playerll_generate_software_it();
	swHappend = false;
	while (!swHappend)
		;

}
void player_resume(uint8_t channel) {
	channel_data *c = getChannel(channel);
	if (!c)
		return;
	if (c->state == audio_playing)
		return;
	c->state = audio_playing;
	playerll_dma_unpause(c->channel_id);

}
void player_replay(uint8_t channel) {
	channel_data *c = getChannel(channel);
	if (!c)
		return;
	player_play(c->channel_id, c->laddress, c->llength);
}
void player_play(uint8_t channel, uint32_t address, uint32_t len) {
	channel_data *c = getChannel(channel);
	if (!c)
		return;
	player_abort(channel);

	c->laddress = c->address = address;
	c->llength = c->length = len;
	c->state = audio_starting;

	playerll_generate_software_it();
	swHappend = false;
	while (!swHappend)
		;

}
void player_abort(uint8_t channel) {
	channel_data *c = getChannel(channel);
	if (!c)
		return;
	if (c->state == audio_idle)
		return;
	c->state = audio_idle;

	playerll_generate_software_it();
	swHappend = false;
	while (!swHappend)
		;
}

bool player_isPlaying(uint8_t channel) {
	channel_data *c = getChannel(channel);
	if (!c)
		return false;
	return c->state == audio_playing;

}
audio_state player_getState(uint8_t channel) {
	channel_data *c = getChannel(channel);
	if (!c)
		return audio_playing;
	return c->state;

}

static void map(uint8_t *target) {

	for (int i = 0; i < fileBufferSize; i++) {
#if dac_freq  	 == 	32000 && audio_freq == 16000
		target[i * 2 + 1] = target[i * 2] = fileBuffer[i];

#elif dac_freq   == 	32000 && audio_freq == 8000
		target[i * 4 + 3] = target[i * 4 + 2] = target[i * 4 + 1] =
				target[i * 4] = fileBuffer[i];

#elif dac_freq   == 	16000 && audio_freq == 16000
		target[i]=fileBuffer[i];

#elif dac_freq   == 	16000 && audio_freq == 8000
		target[i*2+1]=target[i*2]=fileBuffer[i];

#else
#error "only dac_freq 16k and 32k and audio_freq 8k and 16k supported"
#endif
	}
}
static bool fillSegment(channel_data *data, uint8_t *segment) {

	uint32_t len_to_read = 0;
	bool isFinished;
	if (data->length == 0)
		return true;
	if (data->length >= fileBufferSize) {
		len_to_read = fileBufferSize;
		isFinished = false;
	} else {
		len_to_read = data->length;
		isFinished = true;
		;

	}
	data->length -= len_to_read;

	playerll_read(data->address, len_to_read, fileBuffer);
	map(segment);

	data->address += len_to_read;
	data->inBuffer = len_to_read * dac_freq / audio_freq;

	return isFinished;
}
static void handle_softwareitr(channel_data *data) {
	switch (data->state) {
	case audio_idle:
		playerll_dma_stop(data->channel_id);
		break;
	case audio_pause:
		playerll_dma_pause(data->channel_id);
		break;
	case audio_playing:

		if (!data->dma_req_flag)
			break;
		data->dma_req_flag = false;

		if (data->length == 0) {
			data->state = audio_idle;
			playerll_dma_stop(data->channel_id);

		} else {
			playerll_dma_start(data->channel_id, data->audioBuffer,
					data->inBuffer);

			if (data->audioBuffer == data->segment1)
				data->audioBuffer = data->segment2;
			else
				data->audioBuffer = data->segment1;
			fillSegment(data, data->audioBuffer);
		}
		break;
	case audio_resuming:
		break;
	case audio_starting:
		fillSegment(data, data->segment1);
		uint32_t inBuffer = data->inBuffer;

		fillSegment(data, data->segment2);

		data->state = audio_playing;
		data->audioBuffer = data->segment2;
		data->dma_req_flag = false;
		playerll_dma_start(data->channel_id, data->segment1, inBuffer);
		break;
	}
}

// called by ll when software interrupt happend
void player_on_sw_it() {
	swHappend = true;
	handle_softwareitr(&c1);
	handle_softwareitr(&c2);

}
// called by ll when dma finished
void player_on_dma_finish(uint8_t channel) {
	playerll_generate_software_it();
	if (channel == c1.channel_id)
		c1.dma_req_flag = true;
	if (channel == c2.channel_id)
		c2.dma_req_flag = true;
}
