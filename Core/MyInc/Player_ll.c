#include "main.h"
#include "w25qxx.h"
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;
extern DMA_HandleTypeDef hdma_tim16_ch1_up;
extern DMA_HandleTypeDef hdma_tim17_ch1_up;

void playerll_init() {
	// software itr at exti line 2
	EXTI->IMR |= 1 << 2;
	NVIC_EnableIRQ(EXTI2_3_IRQn);
	NVIC_SetPriority(EXTI2_3_IRQn, 3);

	// need 8mhz clock for timer , 256clk period
	// enable timer interrupt for PWM
	// timer timer should be higher priority than software interrupt
	// both interrupt should have higher priority than user interrupt

}

void playerll_pwm_set(uint8_t channel, uint32_t d) {
	if (channel == 0)
		htim16.Instance->CCR1 = d;
	else if (channel == 1)
		htim17.Instance->CCR1 = d;
}

uint32_t playerll_pwm_get(uint8_t channel) {
	if (channel == 0)
		return htim16.Instance->CCR1;
	else if (channel == 1)
		return htim17.Instance->CCR1;
	return 0;
}

void playerll_read(uint32_t address, uint32_t len, uint8_t *buffer) {
	W25qxx_ReadBytes(buffer, address, len);
}

// dma part
extern void player_on_dma_finish(uint8_t channel);
static void handle_dma_finish(DMA_HandleTypeDef *_hdma) {

	if (_hdma == &hdma_tim16_ch1_up)
		player_on_dma_finish(0);
	else if (_hdma == &hdma_tim17_ch1_up)
		player_on_dma_finish(1);

}
static bool dma1 = false, dma2 = false;

void playerll_dma_stop(uint8_t channel) {
	if (channel == 0 && dma1) {
		HAL_TIM_PWM_Stop_DMA(&htim16, TIM_CHANNEL_1);
		dma1 = false;
	} else if (channel == 1 && dma2) {
		HAL_TIM_PWM_Stop_DMA(&htim17, TIM_CHANNEL_1);
		dma2 = false;
	}
}

void playerll_dma_start(uint8_t channel, uint8_t *buff, uint32_t len) {
	if (channel == 0) {
		if (dma1 == false) {
			HAL_TIM_PWM_Start_DMA(&htim16, TIM_CHANNEL_1, (uint32_t*) buff,
					len + 1);
			hdma_tim16_ch1_up.XferCpltCallback = handle_dma_finish;
		} else {
			CLEAR_BIT(hdma_tim16_ch1_up.Instance->CCR, DMA_CCR_EN);
			hdma_tim16_ch1_up.Instance->CNDTR = len;
			hdma_tim16_ch1_up.Instance->CMAR = (uint32_t) buff;
			SET_BIT(hdma_tim16_ch1_up.Instance->CCR, DMA_CCR_EN);
		}
		dma1 = true;
	} else if (channel == 1) {
		if (dma2 == false) {
			HAL_TIM_PWM_Start_DMA(&htim17, TIM_CHANNEL_1, (uint32_t*) buff,
					len + 1);
			hdma_tim17_ch1_up.XferCpltCallback = handle_dma_finish;
		} else {
			CLEAR_BIT(hdma_tim17_ch1_up.Instance->CCR, DMA_CCR_EN);
			hdma_tim17_ch1_up.Instance->CNDTR = len;
			hdma_tim17_ch1_up.Instance->CMAR = (uint32_t) buff;
			SET_BIT(hdma_tim17_ch1_up.Instance->CCR, DMA_CCR_EN);
		}

		dma2 = true;
	}
}
void playerll_dma_pause(uint8_t channel){
	if (channel == 0 && dma1) {
		CLEAR_BIT(hdma_tim16_ch1_up.Instance->CCR, DMA_CCR_EN);
		playerll_pwm_set(0, 0);
		dma1 = false;
	} else if (channel == 1 && dma2) {
		CLEAR_BIT(hdma_tim17_ch1_up.Instance->CCR, DMA_CCR_EN);
		playerll_pwm_set(1, 0);
		dma2 = false;
	}

}
void playerll_dma_unpause(uint8_t channel){
	if (channel == 0 && !dma1) {
		SET_BIT(hdma_tim16_ch1_up.Instance->CCR, DMA_CCR_EN);
		dma1 = true;
	} else if (channel == 1 && !dma2) {
		SET_BIT(hdma_tim17_ch1_up.Instance->CCR, DMA_CCR_EN);
		dma2 = true;
	}
}
//*

//software interrupt
extern void player_on_sw_it();
void EXTI2_3_IRQHandler() {
	EXTI->PR |= 1 << 2;
	player_on_sw_it();

}
void playerll_generate_software_it() {
	EXTI->SWIER |= 1 << 2;
}
//*
