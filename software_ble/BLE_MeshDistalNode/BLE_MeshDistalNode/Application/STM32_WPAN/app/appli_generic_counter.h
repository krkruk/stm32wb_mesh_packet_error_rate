/*
 * appli_generic_counter.h
 *
 *  Created on: Apr 4, 2022
 *      Author: krzysztof
 */

#ifndef APPLICATION_STM32_WPAN_APP_APPLI_GENERIC_COUNTER_H_
#define APPLICATION_STM32_WPAN_APP_APPLI_GENERIC_COUNTER_H_
#include "types.h"

#define ENABLE_LED_BLINKING

/*
 * The purpose of this header is to provide an API to cover Packet Error Rate experiment.
 * The overall experiment is designed as follows:
 *  * appli_test (proximal node): Run periodically Generic OnOff client model (SET-05)
 *  * appli_test: Count the number of sent Generic OnOff requests
 *  * appli_generic_counter (remote node): Count the number of received messages
 *  * appli_vendor (remote node): Publish the number of received messages
 *  * appli_test: Collect the results. Make sure the nodes are close enough to get the results
 *
 *  Externally, plot the data.
 */

typedef struct {
	MOBLEUINT32 counter;
} GenericOnOffCounter_t;

void generic_onoff_counter_initialize();
MOBLEUINT32 generic_onoff_counter();

#endif /* APPLICATION_STM32_WPAN_APP_APPLI_GENERIC_COUNTER_H_ */

