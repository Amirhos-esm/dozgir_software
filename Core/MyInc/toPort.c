#include "mainCodes.h"


extern UART_HandleTypeDef huart1;
void sim800c_transmit(uint8_t data)
{
	swap(0);
	HAL_UART_Transmit(&huart1, &data, 1, 10);

}
void sim800c_onError(const char *str)
{
	logln((char* )str);
	HAL_NVIC_SystemReset();
}
void USART1_IRQHandler(void)
{
	sim800c_onDataReceive(USART1->RDR);
}

void init_uart()
{
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);


	CLEAR_BIT(USART1->CR1, USART_CR1_RXNEIE);
	SET_BIT(USART1->CR1, USART_CR1_RXNEIE);


}
