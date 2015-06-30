#include "stm32f1Utility.h"

void ChangeClockTo36Mhz(void) {
	/*Configure all clocks to max for best performance.
	 * If there are EMI, power, or noise problems, try slowing the clocks*/
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_OFF);
	RCC_HSICmd(ENABLE);

	/* First set the flash latency to work with our clock*/
	/*000 Zero wait state, if 0  MHz < SYSCLK <= 24 MHz
	 001 One wait state, if  24 MHz < SYSCLK <= 48 MHz
	 010 Two wait states, if 48 MHz < SYSCLK <= 72 MHz */
	FLASH_SetLatency(FLASH_Latency_1);

	/* Start with HSI clock (internal 8mhz), divide by 2 and multiply by 9 to
	 * get maximum allowed frequency: 36Mhz
	 * Enable PLL, wait till it's stable, then select it as system clock*/
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_9);
	RCC_PLLCmd(ENABLE);

	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
	}
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	/* Set HCLK, PCLK1, and PCLK2 to SCLK (these are default */
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	/* Set ADC clk to 9MHz (14MHz max, 18MHz default)*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);

}

void ChangeClockToXXMhz2(void) {
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_OFF);
	ErrorStatus HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (HSEStartUpStatus == SUCCESS) {
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1);

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);

		/* ADCCLK = PCLK2/4 */
		RCC_ADCCLKConfig(RCC_PCLK2_Div4);

		/* PLLCLK = 8MHz * 7 = 56 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_7);

		/* Enable PLL */
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while (RCC_GetSYSCLKSource() != 0x08) {
		}
	}
}

void Delay(u32 counter) {
	while (counter--) {

	}
}
