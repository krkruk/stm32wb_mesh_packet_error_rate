/*
 * appli_generic_counter.h
 *
 *  Created on: Apr 4, 2022
 *      Author: krzysztof
 */

#ifndef APPLICATION_STM32_WPAN_APP_APPLI_GENERIC_COUNTER_H_
#define APPLICATION_STM32_WPAN_APP_APPLI_GENERIC_COUNTER_H_
#include "types.h"


typedef struct {
	MOBLEUINT32 counter;
} GenericOnOffCounter_t;

void generic_onoff_counter_initialize();
MOBLEUINT32 generic_onoff_counter();

#endif /* APPLICATION_STM32_WPAN_APP_APPLI_GENERIC_COUNTER_H_ */
