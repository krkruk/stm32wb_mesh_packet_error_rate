/**
******************************************************************************
* @file    appli_test.h
* @author  BLE Mesh Team
* @brief   Header file for the serial interface file 
******************************************************************************
* @attention
*
* Copyright (c) 2018-2021 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
*
******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APPLI_TEST_H
#define __APPLI_TEST_H

/* Includes ------------------------------------------------------------------*/
#include "types.h"
#include "generic.h"

/* Exported macro ------------------------------------------------------------*/
#define     TEST_1_WAIT_PERIOD          2000
#define     TEST_2_WAIT_PERIOD          300
#define     TEST_3_WAIT_PERIOD          3000
#define     TEST_READ_PERIOD            2000
#define     CLOCK_FLAG_ENABLE           1 
#define     CLOCK_FLAG_DISABLE          0 
#define     DATA_BYTE_SEND              50

#define OP_NAME_SET05 "SET-05"
#define OP_NAME_GET05 "GET-05"
#define OP_NAME_SET06 "SET-06"
#define OP_NAME_GET06 "GET-06"

typedef enum {
	CMD_TYPE_NONE,
	CMD_TYPE_SET,
	CMD_TYPE_GET
} CommandType_t;

typedef enum {
	test_kill_subscription = 256,
	test_generic_subscription,
	test_calibrate_timer
} MeshTest_t;

typedef struct {
	char *name;
	uint8_t timer_subscription_id;
	uint8_t timer_kill_subscription_id;
	Generic_OnOffParam_t params;
	uint32_t counter;
	uint32_t remoteCounter;
	uint32_t startTimestamp;
} MeshTestParamters_t;


/* Exported variables  ------------------------------------------------------- */
/* Exported Functions Prototypes ---------------------------------------------*/
void SerialResponse_Process(char *rcvdStringBuff, uint16_t rcvdStringSize);
MOBLE_RESULT Test_ApplicationTest_Set01(MOBLEUINT32 testCount,MOBLE_ADDRESS src ,MOBLE_ADDRESS dst);
MOBLE_RESULT Packet_ResponseTimeStamp(MOBLEUINT32 rcvTimeStamp);
MOBLE_RESULT Test_ApplicationTest_Set02(MOBLEUINT32 testCount ,MOBLE_ADDRESS src ,MOBLE_ADDRESS dst);
MOBLE_RESULT Test_ApplicationTest_Set03(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst);
MOBLE_RESULT Test_ApplicationTest_Set05_GenericOnOff(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst);
MOBLE_RESULT Test_ApplicationTest_Set06_CalibrateTimer(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst);

MOBLE_RESULT Test_ApplicationTest_Get05_GenericOnOff(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst);

MOBLE_RESULT test_set05_generic_initialize(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst);
void test_set05_generic();
void test_set06_calibrate_timer();
void kill_subscription();
void process_set_commands();
void process_get_commands();
MOBLEUINT8 processDelay(MOBLEUINT16 waitPeriod);                                               


#endif /* __RESPONSE_TEST_H */



