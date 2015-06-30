/* Khiem
 * Dec 2, 2014
 * project:general
 * description: implement Assert feature --> help debug
 *
 * enable USE_FULL_ASSERT in stm32f4xx_conf.h, line 70
 */

#ifndef _Assert_h
#define _Assert_h

#include "STM32F4-Discovery/stm32f4_discovery.h"
#include "stm32f4xx_conf.h"
#include "myLib/cUart.h"
#include <stdio.h>
#include <stdarg.h>


#define DEBUGON
#define DEBUGLV		LOG_TEST
#define ASSERTLV	2

//typedef enum {
//	LOG_DEFAULT, LOG_INFO, LOG_ERROR, LOG_DEBUG
//} LOG_LEVEL;

#define LOG_ERROR	0
#define LOG_DEBUG	1
#define LOG_INFO	2
#define LOG_TEST	3

#if  ASSERTLV==0
#define	REQUIRE(test_)
#define	ENSURE(test_)
#define	INVARIANT(test_)
#define ERROR(test_)
#define ALLEGE(test_)
#elif ASSERTLV==1
#define	REQUIRE(test_)			assert_param(test_)//PRECONDITION
#define	ENSURE(test_)			assert_param(test_)//CONDITIONS MAY MEET, BUT NEED TO ENSURE
#define	INVARIANT(test_)
#define ERROR(test_)
#define ALLEGE(test_)
#elif ASSERTLV==2
#define	REQUIRE(test_)			assert_param(test_)//PRECONDITION
#define	ENSURE(test_)			assert_param(test_)//CONDITIONS MAY MEET, BUT NEED TO ENSURE
#define	INVARIANT(test_)		assert_param(test_)//MUST BE ALWAYS TRUE
#define ERROR(test_)			assert_param(test_)//IF IT HAS ANY ERROR
#define ALLEGE(test_)			assert_param(test_)//THIS CASE NOT ALLOW TO HAPPEN PRACTICALLY
#endif /* USE_FULL_ASSERT */

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line);
#endif

#ifdef DEBUGON
#define DEBUG(level, fmt, ...)\
		if (DEBUGLV >= level){\
			UartPrint("DB:%d:", level);\
			UartPrint(fmt, __VA_ARGS__);\
		}
#else
#define DEBUG(level, fmt, ...)
#endif

#endif
