#include "debug.h"

extern UART_HandleTypeDef huart1;

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE {
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART1 and Loop until the end of transmission */
	swap(DBG);
	HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, 0xFFFF);

	return ch;
}

uint8_t mChannel=0;
void swap(uint8_t ch) {
	if (ch == 0 && mChannel==1) {
		delay(20);
		GPIOA->AFR[0] |= 0x100;   // go to channel pa2
		GPIOA->AFR[1] &= ~0xf0;
		mChannel=0;
	} else if (ch == 1 && mChannel==0) { //DBG
		GPIOA->AFR[0] &= ~0xf00;   // go to channel pa9
		GPIOA->AFR[1] |= 0x10;
		mChannel=1;
	}
}
