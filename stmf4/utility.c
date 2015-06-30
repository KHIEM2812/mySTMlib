#include "STM32F4-Discovery/stm32f4_discovery.h"
#include "utility.h"

static __IO uint32_t TM_TimingDelay = 0;
static __IO uint32_t TM_Time = 0, TM_Time2 = 0;
uint8_t TM_DELAY_Initialized = 0;

static inline int min(int a, int b);
static inline int max(int a, int b);
static inline float fpow(float f, int n);

void HardFault_Handler(void) {
	__asm volatile
	(
			" tst lr, #4                                                \n"
			" ite eq                                                    \n"
			" mrseq r0, msp                                             \n"
			" mrsne r0, psp                                             \n"
			" B hard_fault_handler_c                                    \n"
	);
}

void hard_fault_handler_c(unsigned int * hardfault_args) {
	unsigned int stacked_r0;
	unsigned int stacked_r1;
	unsigned int stacked_r2;
	unsigned int stacked_r3;
	unsigned int stacked_r12;
	unsigned int stacked_lr;
	unsigned int stacked_pc;
	unsigned int stacked_psr;

	stacked_r0 = ((unsigned long) hardfault_args[0]);
	stacked_r1 = ((unsigned long) hardfault_args[1]);
	stacked_r2 = ((unsigned long) hardfault_args[2]);
	stacked_r3 = ((unsigned long) hardfault_args[3]);

	stacked_r12 = ((unsigned long) hardfault_args[4]);
	stacked_lr = ((unsigned long) hardfault_args[5]);
	stacked_pc = ((unsigned long) hardfault_args[6]);
	stacked_psr = ((unsigned long) hardfault_args[7]);

	UartPrint("\n\n[Hard fault handler - all numbers in hex]\n");

	UartPrint("R0 = %x\n", (unsigned int) stacked_r0);
	UartPrint("R1 = %x\n", (unsigned int) stacked_r1);
	UartPrint("R2 = %x\n", (unsigned int) stacked_r2);
	UartPrint("R3 = %x\n", (unsigned int) stacked_r3);
	UartPrint("R12 = %x\n", (unsigned int) stacked_r12);
	UartPrint("LR [R14] = %x  subroutine call return address\n",
			(unsigned int) stacked_lr);
	UartPrint("PC [R15] = %x  program counter\n", (unsigned int) stacked_pc);
	UartPrint("PSR = %x\n", stacked_psr);
	UartPrint("BFAR = %x\n",
			(unsigned int) (*((volatile unsigned long *) (0xE000ED38))));
	UartPrint("CFSR = %x\n",
			(unsigned int) (*((volatile unsigned long *) (0xE000ED28))));
	UartPrint("HFSR = %x\n",
			(unsigned int) (*((volatile unsigned long *) (0xE000ED2C))));
	UartPrint("DFSR = %x\n",
			(unsigned int) (*((volatile unsigned long *) (0xE000ED30))));
	UartPrint("AFSR = %x\n",
			(unsigned int) (*((volatile unsigned long *) (0xE000ED3C))));
	UartPrint("SCB_SHCSR = %x\n", (unsigned int) SCB->SHCSR);

	while (1)
		;
}

int abs(int value) {
	if (value < 0) {
		return -value;
	} else {
		return value;
	}
}

void simpleDelay(int count) {
	while (count--)
		;
}

float LimitToRange(float input, float min, float max) {
	if (input > max)
		input = max;
	if (input < min)
		input = min;
	return input;
}

s16 BitConverterToInt16(uint8_t * byte, int index) {
	s16 ret = ((s16) byte[index]) + ((s16) byte[index + 1] << 8);
	return ret;
}
s32 BitConverterToInt32(uint8_t * byte, int index) {
	s32 ret = (((s32) byte[index]) + ((s32) byte[index + 1] << 8)
			+ ((s32) byte[index + 2] << 16) + ((s32) byte[index + 3] << 24));
	return ret;
}
float BitConverterToFloat(uint8_t * byte, int index) {
	float ret = (float) (((u32) byte[index]) + ((u32) byte[index + 1] << 8)
			+ ((u32) byte[index + 1] << 16) + ((u32) byte[index + 1] << 24));
	return ret;
}

static inline int min(int a, int b) {
	return a < b ? a : b;
}
static inline int max(int a, int b) {
	return a < b ? b : a;
}
static inline float fpow(float f, int n) {
	float rv = 1;
	while (n-- > 0) {
		rv *= f;
	}
	return rv;
}

void rcc_set_frequency(enum sysclk_freq freq) {
	/* PLL_VCO = (HSE_VALUE / PLL_M) * PLL_N */
	/* sysClk = clkSource/PLL_M*PLL_N/PLL_P => PPL_N/PLL_M/PLL_P = sysClk/clkSource
	 * USB OTG FS, SDIO and RNG Clock =  PLL_VCO / PLLQ
	 *
	 * 21/2
	 *
	 * */
	uint32_t PLL_P = 2;
	uint32_t PLL_N = 210;
	uint32_t PLL_M = 10;
	uint32_t PLL_Q = 7;
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_OFF);
	RCC_HSICmd(ENABLE);
	/* Wait till HSI is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET) {
	}
	RCC_PLLCmd(DISABLE);
	/* Configure PLL clock M, N, P, and Q dividers */
	RCC_PLLConfig(RCC_PLLSource_HSI, PLL_M, PLL_N, PLL_P, PLL_Q);
	/* Enable PLL clock */
	RCC_PLLCmd(ENABLE);
	/* Wait until PLL clock is stable */
	while ((RCC->CR & RCC_CR_PLLRDY) == 0)
		;
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	/* HCLK = SYSCLK */
	RCC_HCLKConfig(RCC_SYSCLK_Div1);

	/* Set APBx clock dividers */
	RCC_PCLK1Config(RCC_HCLK_Div4); /* 42MHz */
	RCC_PCLK2Config(RCC_HCLK_Div2); /* 84MHz */

	/* Update SystemCoreClock variable */
//	SystemCoreClockUpdate();
}
