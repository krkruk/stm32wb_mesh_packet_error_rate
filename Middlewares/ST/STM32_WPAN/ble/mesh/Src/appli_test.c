/**
******************************************************************************
* @file   appli_test.c
* @author  BLE Mesh Team
* @brief   Serial Interface file 
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
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "hal_common.h"
#include "mesh_cfg.h"
#include "common.h"
#include "appli_test.h"
#include <string.h>
#if ENABLE_SERIAL_CONTROL
#include "serial_ctrl.h"
#endif
#if ENABLE_UT
#include "serial_ut.h"
#endif
#include "generic.h"
/** @addtogroup BlueNRG_Mesh
*  @{
*/

/** @addtogroup Application
*  @{
*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define CMD_INDEX_RES_01                  1
#define CMD_INDEX_RES_02                  2
#define CMD_INDEX_RES_03                  3
#define CMD_INDEX_RES_04                  4

// user defined
#define CMD_INDEX_RES_05_GENERIC          5
#define CMD_INDEX_RES_06_TIMER_CALIBRATE  6

#define CMD_SET_OFFSET                    7
#define CMD_RES_OFFSET                    5
#define CMD_RES_COUNT                     7


/* Private variables ---------------------------------------------------------*/
MOBLEUINT8 TestNumber = 0;
MOBLEUINT32 TestCount = 0;
MOBLEUINT32 RecvCount = 0;
MOBLEUINT32 Totaltest = 0;
CommandType_t command = CMD_TYPE_NONE;

static MeshTestParamters_t meshTest;
extern MOBLEUINT8 AppliBuffer[DATA_BUFFER_LENGTH];

/* Private function prototypes -----------------------------------------------*/
static MOBLE_RESULT SerialResponse_doubleHexToHex(MOBLEUINT8* hexArray,MOBLEUINT8* outputArray, MOBLEUINT8 length);
static MOBLEUINT16 SerialResponse_GetFunctionIndex(char *text);
MOBLE_RESULT Read_CommandCount(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst);
MOBLEUINT32 timeStampSend = 0;
MOBLE_ADDRESS srcAddress;
MOBLE_ADDRESS destAddress;
MOBLEUINT8 ReadFlag = 0;
extern MOBLEUINT8 successCounter=0;
extern MOBLEUINT8 sendCounter=0;
/* Private functions ---------------------------------------------------------*/

static void run_timer(char const *setName, MeshTest_t subscriptionType, MOBLE_ADDRESS src ,MOBLE_ADDRESS dst, void (*callback)(void) ) {
//	 ATAP SET-dd 0000 0001 srcAddr destAddr
//	  where:
//	   dd - test function id. See: Test_Process() function
//	   0000 - triggerInterval - how frequently trigger Generic OnOff Set
//	   0001 - stop processing after time elapses
//	   srcAddr - source address
//	   destAddr - destiantion address
//  EXAMPLE: ATAP SET-05 04008000 C000 0004
//	  triggerInterval = 1024 (dec)
//	  killAfterTimeout = 32768 (dec)

	MOBLEUINT16 triggerInterval;
	MOBLEUINT16 killAfterTimeout;

	triggerInterval = Totaltest >> 16;
	killAfterTimeout = 0x0000ffff & Totaltest;

	TRACE_I(TF_SERIAL_CTRL,"%s triggerInterval=%d, killAfterTimeout=%d \r\n", setName, triggerInterval, killAfterTimeout);
	HW_TS_Create(subscriptionType, &(meshTest.timer_subscription_id), hw_ts_Repeated, callback);
	HW_TS_Create(test_kill_subscription, &(meshTest.timer_kill_subscription_id), hw_ts_SingleShot, kill_subscription);
	TRACE_I(TF_SERIAL_CTRL,"%s Created subscription timer=%d \r\n", setName, meshTest.timer_subscription_id);
	TRACE_I(TF_SERIAL_CTRL,"%s Created test_kill_subscription timer=%d \r\n", setName, meshTest.timer_kill_subscription_id);

	meshTest.startTimestamp = HAL_GetTick();
	HW_TS_Start(meshTest.timer_subscription_id, triggerInterval);
	HW_TS_Start(meshTest.timer_kill_subscription_id, killAfterTimeout);
}

void kill_subscription() {
	uint32_t stopTimestamp = HAL_GetTick();
	HW_TS_Delete(meshTest.timer_subscription_id);
	TRACE_I(TF_SERIAL_CTRL, "Deleted subscription timer = %d \r\n", meshTest.timer_subscription_id);
	TRACE_I(TF_SERIAL_CTRL,
			"{\"func\":\"%s\",\"addr\":\"%04x\",\"counter\":%u,\"HAL_TickFreq\":%d,\"startTick\":%u,\"stopTick\":%u,\"tick_diff\":%ld}\r\n",
			meshTest.name,
			BLEMesh_GetAddress(),
			meshTest.counter,
			((uint32_t) HAL_GetTickFreq()),
			meshTest.startTimestamp,
			stopTimestamp,
			(meshTest.startTimestamp-stopTimestamp));

	meshTest.timer_subscription_id = 0;
}


/**
* @brief  SerialResponse_Process: This function extracts the command and variables from
the recieved string and passes it to BluenrgMesh library.
* @param  rcvdStringBuff: Pointer to the recieved ascii character array from the user
* @param  rcvdStringSize: Size of the recieved array
* @retval void
*/ 
void SerialResponse_Process(char *rcvdStringBuff, uint16_t rcvdStringSize)
{
  MOBLEUINT8 testFunctionParm[10]= {'\0'} ;
  MOBLEUINT8 asciiFunctionParameter[16] = {'\0'} ;
  successCounter = 0;
  sendCounter = 0;
  TRACE_I(TF_SERIAL_CTRL, "Processing serial response: %s \n\r", rcvdStringBuff);
  
    MOBLEUINT16 commandIndex = SerialResponse_GetFunctionIndex(rcvdStringBuff+5);
    TRACE_I(TF_SERIAL_CTRL, "CommandIndex=%d, CommandType=%d\n\r", commandIndex, command);

    if(commandIndex != 0x00)
    {
		  sscanf(rcvdStringBuff + CMD_RES_OFFSET + CMD_SET_OFFSET, "%8s %4s %4s ", asciiFunctionParameter, asciiFunctionParameter+8,asciiFunctionParameter+12);
			TRACE_I(TF_SERIAL_CTRL, "asciiFuncParam=%s asciiFuncParam+8=%s\n\r", asciiFunctionParameter, asciiFunctionParameter+8);
		  /*SerialResponse_doubleHexToHex
		  Function will convert the asci string into orinal hex format.
		  eg- send-01 12 3456
		  full query: ATAP SET-dd 00000001 srcAddr destAddr
		  	  where:
		  	   dd - test function id. See: Test_Process() function
		  	   00000001 - expected value
		  	   srcAddr - source address
		  	   destAddr - destiantion address
		  return 0x12,0x34,0x56       
		  */
        if(MOBLE_RESULT_SUCCESS == SerialResponse_doubleHexToHex(asciiFunctionParameter,testFunctionParm,16))
        { 
			  TestCount = (MOBLEUINT32)(testFunctionParm[0] << 24);
			  TestCount |= (MOBLEUINT32)(testFunctionParm[1] << 16);
			  TestCount |= (MOBLEUINT32)(testFunctionParm[2] << 8);
			  TestCount |= (MOBLEUINT32)(testFunctionParm[3] << 0);
			  TestNumber = commandIndex;
			  Totaltest = TestCount;
			  srcAddress = (MOBLEUINT16)((testFunctionParm[4] << 8) | testFunctionParm[5]);
			  destAddress = (MOBLEUINT16)((testFunctionParm[6] << 8) | testFunctionParm[7]);
			  TRACE_I(TF_SERIAL_CTRL, "TestCount=%d, CommandIndex=%d, CommandType=%d, srcAddr=%04x, dstAddr=%04x\n\r",
					  TestCount, TestNumber, command, srcAddress, destAddress);
		}
        else
        {
          TRACE_I(TF_SERIAL_CTRL, "Wrong command typed %hx\n\r", testFunctionParm);
        }
    }
    else
    {
      TRACE_I(TF_SERIAL_CTRL, "Wrong command typed because commandIndex=0x00\n\r");
    }
}

/**
* @brief  SerialResponse_GetFunctionIndex: This function returns the calculated index 
of the command recieved by the user
* @param  rcvdStringBuff: Pointer to the recieved ascii character array from the user
* @param  rcvdStringSize: Size of the recieved array
* @retval MOBLEUINT16
*/  
static MOBLEUINT16 SerialResponse_GetFunctionIndex(char *text)
{
  MOBLEINT16 index = 0;

  if (!strncmp(text, "SET-",4))
  {
    sscanf(text, "SET-%hd", &index);
    index = (index<=CMD_RES_COUNT)? index : 0;
    command = CMD_TYPE_SET;
  }
  else if (!strncmp(text, "GET-",4)) {
	  sscanf(text, "GET-%hd", &index);
	  index = (index<=CMD_RES_COUNT)? index : 0;
	  command = CMD_TYPE_GET;
  }
  else
  {
    command = CMD_TYPE_NONE;
    return 0x00;
  }

  return index;
}

/**
* @brief  SerialResponse_doubleHexToHex: This function converts two 4-bit hex integers 
to one 8-bit hex integer 
* @param  hexArray: Pointer to input hex array 
* @param  outputArray: Pointer to output hex array 
* @param  length: length of the input hex array
* @retval void
*/ 
static MOBLE_RESULT SerialResponse_doubleHexToHex(MOBLEUINT8* hexArray,MOBLEUINT8* outputArray, MOBLEUINT8 length)
{
  MOBLE_RESULT status = MOBLE_RESULT_SUCCESS;
  MOBLEUINT8 counter =0,position = 0;
  MOBLEUINT8 msb = 0,lsb = 0;
  while (counter <length)
  {
    msb = Serial_CharToHexConvert(hexArray[counter]);
    lsb = Serial_CharToHexConvert(hexArray[counter + 1 ]);
    if((msb == 0xFF) && (lsb == 0xFF))
    {
		TRACE_I(TF_SERIAL_CTRL, "Return false %02hx %02hx\n\r", msb, lsb);
      return MOBLE_RESULT_FALSE;
    }
    outputArray[position] = msb<<4;
    outputArray[position] |= lsb;
    TRACE_I(TF_SERIAL_CTRL, "data scanned is  %02hx \n\r",outputArray[position]);
    counter+=2;
    position++;
  }
  return status;
}

/**
* @brief  Test_ApplicationTest_Set01: This function sends the vendor test 
  command till the testCount become 0.this test is used to calculate the round trip
  time for the packet send and received response.
* @param  testCount: Pointer to the test count value
* @param  src: source address
* @param  dst: destination address
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Test_ApplicationTest_Set01(MOBLEUINT32 testCount ,MOBLE_ADDRESS src ,MOBLE_ADDRESS dst)
{
  MOBLEUINT8 elementIndex = 0;

  /* The function will called untill the testcount will not become zero */     
  if(testCount != 0)
  {
    if(processDelay(TEST_1_WAIT_PERIOD) == 0x01)
    {
      /* Taking the timestamp while sending the command for vendor model */       
      timeStampSend = Clock_Time();      
      Vendor_TestRemoteData(src,dst,elementIndex);  

      TestCount--;                       
    }     
  }

  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Test_ApplicationTest_Set02: This function sends the vendor test 
  command till the testCount become 0.This function is used to send the bunch of 
  command to the receiver node and then called a function Read_CommandCount to get 
  the number of command received by receiver. 
* @param  testCount: Pointer to the test count value
* @param  src: source address
* @param  dst: destination address
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Test_ApplicationTest_Set02(MOBLEUINT32 testCount ,MOBLE_ADDRESS src ,MOBLE_ADDRESS dst)                                             
{
  MOBLEUINT8 elementIndex = 0;
   
  if(testCount != 0)
  {
    if(processDelay(TEST_2_WAIT_PERIOD) == 0x01)
    {  
      /* Sending the command one after one in defined interval of time */         
      Vendor_TestCounterInc(src,dst,elementIndex);  

      TestCount--;              
      TRACE_I(TF_SERIAL_CTRL, "Sending No Response command  \r\n");

      if(TestCount == 0)
      {
        /* Flag to call the function Read_CommandCount after some duration of time */          
        ReadFlag = 1;
      }
                
   }     
 }
   
 return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Read_CommandCount: This function read the number of 
  command received by the reciver successfully.
* @param  src: source address of the node
* @param  dst: destination address of the node
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Read_CommandCount(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst)
{
  MOBLEUINT8 readData[2];
  MODEL_MessageHeader_t msgParam;
   
  msgParam.elementIndex = 0;
  msgParam.peer_addr = src;
  msgParam.dst_peer = dst;
  msgParam.ttl = 0;
  msgParam.rssi = 0;
  msgParam.rcvdAppKeyOffset = 0;
  msgParam.rcvdNetKeyOffset = 0;

  readData[0] = APPLI_TEST_INC_COUNTER;

  if(processDelay(TEST_READ_PERIOD) == 0x01)
  {   
    TRACE_I(TF_SERIAL_CTRL, " NUMBER OF COMMANDS SEND     %d \r\n",Totaltest);
 
    BLEMesh_ReadRemoteData(&msgParam,APPLI_TEST_INC_COUNTER,readData,sizeof(readData));
      
    ReadFlag = 0;
  }     

  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Packet_ResponseTimeStamp: This function called in vendor_response
  it calculates all the packet sending ,receiving, average time.this function called
  to take the response received time stamp.
* @param  rcvTimeStamp: Pointer to the response time stamp received
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Packet_ResponseTimeStamp(MOBLEUINT32 rcvTimeStamp)
{
  static MOBLEUINT32 totalTimeStamp = 0;
  MOBLEUINT32 DiffTimeStamp;
  RecvCount ++;
  DiffTimeStamp = rcvTimeStamp - timeStampSend; 
      
  /* Adding the differnce time stamp for calculation of average time of packet 
    sending and receiving 
  */ 
  totalTimeStamp = totalTimeStamp + DiffTimeStamp;
  TRACE_I(TF_SERIAL_CTRL, "Response Got                        %d out of %d\r\n", RecvCount, Totaltest);
  TRACE_I(TF_SERIAL_CTRL, "Packet send time stamp              %d \r\n",timeStampSend);
  TRACE_I(TF_SERIAL_CTRL, "Response receive time stamp         %d \r\n",rcvTimeStamp);
  TRACE_I(TF_SERIAL_CTRL, "Round trip Time %d \r\n \r\n",DiffTimeStamp); 

  if(TestCount == 0)
  {   
    totalTimeStamp = totalTimeStamp/Totaltest; 
    TRACE_I(TF_SERIAL_CTRL, "Maximum Wait Time             %d ms\r\n", TEST_1_WAIT_PERIOD);
    TRACE_I(TF_SERIAL_CTRL, "AVERAGE ROUND TRIP TIME       %d \r\n",totalTimeStamp);
    TRACE_I(TF_SERIAL_CTRL, "PACKET SINGLE TRIP TIME       %d \r\n",totalTimeStamp/2);
    totalTimeStamp = 0;
  }
  return MOBLE_RESULT_SUCCESS;
}

/**
* @brief  Test_ApplicationTest_Set03: This function sends the number of byte
  and get back the same data in response.
* @param  timeStamp: Pointer to the response received time stamp
* @param  src: source address of the node
* @param  dst: destination address of the node
* @retval MOBLE_RESULT
*/
MOBLE_RESULT Test_ApplicationTest_Set03(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst)
{
  MOBLEUINT8 txDataArray[DATA_BYTE_SEND] = {0};
  txDataArray[0] = APPLI_TEST_ECHO; 
  MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
  MOBLEUINT16 idx;

  for (idx=1; idx<DATA_BYTE_SEND; idx++)
      {
    txDataArray[idx] = idx;
    TRACE_I(TF_SERIAL_CTRL,"data[%d]= %d \r\n",idx,txDataArray[idx]);             
      }

  result = BLEMesh_SetRemoteData(dst, 0,
                                 APPLI_TEST_CMD, txDataArray, 
                                 sizeof(txDataArray), MOBLE_TRUE, 
                                 MOBLE_TRUE);


      if(result)
      {
        TRACE_I(TF_SERIAL_CTRL,"Publication Error \r\n");
      }

  	TestNumber = 0;
  	command = CMD_TYPE_NONE;
  
  return MOBLE_RESULT_SUCCESS;
}


MOBLE_RESULT Test_ApplicationTest_Set05_GenericOnOff(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst)
{
	MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
	meshTest.params.Delay_Time = 0;
	meshTest.params.Generic_TID = 0;
	meshTest.params.TargetOnOffState = APPLI_LED_ON;
	meshTest.params.Transition_Time = NO_TRANSITION;

	meshTest.name = OP_NAME_SET05;
	meshTest.counter = 0;
	result = test_set05_generic_initialize(src, dst);
	if (!result)
	{
		run_timer(OP_NAME_SET05, test_generic_subscription, src, dst, test_set05_generic);
		result = MOBLE_RESULT_SUCCESS;
	}
	else
	{
		TRACE_I(TF_VENDOR_M,"%s Could not initialize the test due to error code=%d \r\n", OP_NAME_SET05, result);
		result = MOBLE_RESULT_FAIL;
	}

	TestNumber = 0; // kill command
	command = CMD_TYPE_NONE;
	return result;
}

MOBLE_RESULT test_set05_generic_initialize(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst)
{
    MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
    AppliBuffer[0] = APPLI_TEST_PACKET_ERROR_RATE_COUNTER;

    result = BLEMesh_SetRemotePublication(VENDORMODEL_STMICRO_ID1, src,
                                          APPLI_TEST_CMD,
                                          AppliBuffer, sizeof(AppliBuffer),
                                          MOBLE_TRUE, MOBLE_TRUE);

     if (result)
     {
          TRACE_I(TF_VENDOR_M, "%s Could not initialize the test due to error code=%d \r\n", OP_NAME_SET05, result);
     }
     return result;
}

void test_set05_generic() {
	MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
	meshTest.params.TargetOnOffState ^= (1<<0);		// flip LED bit
	meshTest.counter++;

	result = GenericClient_OnOff_Set_Unack(
			GENERIC_SERVER_MAIN_ELEMENT_INDEX,
			&(meshTest.params),
			sizeof(Generic_OnOffParam_t));

	TRACE_I(TF_SERIAL_CTRL, "%s publish result=%d \r\n", meshTest.name, result);
}


MOBLE_RESULT Test_ApplicationTest_Get05_GenericOnOff(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst) {
MOBLEUINT8 readData[2];
MODEL_MessageHeader_t msgParam;

TRACE_I(TF_SERIAL_CTRL, "%s srcAddr=%d, dstAddr=%d\r\n", OP_NAME_GET05, src, dst);
	msgParam.elementIndex = 0;
	msgParam.peer_addr = src;
	msgParam.dst_peer = dst;
	msgParam.ttl = 0;
	msgParam.rssi = 0;
	msgParam.rcvdAppKeyOffset = 0;
	msgParam.rcvdNetKeyOffset = 0;

	readData[0] = APPLI_TEST_PACKET_ERROR_RATE_COUNTER;

	BLEMesh_ReadRemoteData(&msgParam, APPLI_TEST_PACKET_ERROR_RATE_COUNTER, readData, sizeof(readData));
	TRACE_I(TF_SERIAL_CTRL, "%s Command triggered", OP_NAME_GET05);
	TestNumber = 0;
	command = CMD_TYPE_NONE;
	return MOBLE_RESULT_SUCCESS;
}

MOBLE_RESULT Test_ApplicationTest_Set06_CalibrateTimer(MOBLE_ADDRESS src ,MOBLE_ADDRESS dst) {
	MOBLE_RESULT result = MOBLE_RESULT_SUCCESS;
	meshTest.name = OP_NAME_SET06;
	meshTest.counter = 0;
	run_timer(OP_NAME_SET06, test_generic_subscription, src, dst, test_set06_calibrate_timer);
	TestNumber = 0; // kill command
	command = CMD_TYPE_NONE;
	return result;
}

void test_set06_calibrate_timer() {
	meshTest.counter++;
	TRACE_I(TF_SERIAL_CTRL,"{\"func\":\"%s\", \"counter\":%ld, \"status\":\"running\"}\r\n", meshTest.name, meshTest.counter);
}

/**
* @brief  Function used to calculate the delay.
* @param  MOBLEUINT16
* @retval MOBLEUINT8
*/
MOBLEUINT8 processDelay(MOBLEUINT16 waitPeriod)
{
  static MOBLEUINT8 Clockflag = 0;
  static MOBLEUINT32 Check_time;


  if(Clockflag == CLOCK_FLAG_DISABLE)
  {
    Check_time = Clock_Time();
    Clockflag = CLOCK_FLAG_ENABLE;
  } 
  /* The function will called untill the testcount will not become zero */     

  if(((Clock_Time()- Check_time) >= waitPeriod))
  {
    Clockflag = CLOCK_FLAG_DISABLE;
    return 0x01;
            
  }
  return 0x00;
}

/**
* @brief  Function used to run the testing function for Packet response time
* @param  void
* @retval void
*/
void Test_Process(void)
{
  if (!TestNumber) {
	  return;
  }
  MOBLEUINT8 testNumber = TestNumber;
  CommandType_t cmd = command;
  TRACE_I(TF_SERIAL_CTRL, "Processing command=%d, testNumber=%d\n\r", cmd, testNumber);
  switch (cmd) {
	  case CMD_TYPE_SET: {
		  process_set_commands(testNumber);
		  break;
	  }
	  case CMD_TYPE_GET: {
		  process_get_commands(testNumber);
		  break;
	  }
	  case CMD_TYPE_NONE:
	  default: {
		  TRACE_I(TF_SERIAL_CTRL, "Invalid Command=%d (either GET or SET)\n\r", command);
		  TestNumber = 0;
		  command = CMD_TYPE_NONE;
		  break;
	  }
  }

}

void process_set_commands(MOBLEUINT8 testNumber) {
	switch (testNumber)
		{
		case CMD_INDEX_RES_01:
		{
		  Test_ApplicationTest_Set01(TestCount,srcAddress,destAddress);
		  break;
		}
		case CMD_INDEX_RES_02:
		{
		  Test_ApplicationTest_Set02(TestCount,srcAddress,destAddress);
		  if(ReadFlag == 1)
		  {
			Read_CommandCount(srcAddress , destAddress);
		  }
		  break;
		}
		case CMD_INDEX_RES_03:
		{
		  Test_ApplicationTest_Set03(srcAddress,destAddress);
		  break;
		}
		case CMD_INDEX_RES_05_GENERIC:
		{
		  Test_ApplicationTest_Set05_GenericOnOff(srcAddress, destAddress);
		  break;
		}
		case CMD_INDEX_RES_06_TIMER_CALIBRATE:
		{
		  Test_ApplicationTest_Set06_CalibrateTimer(srcAddress, destAddress);
		  break;
		}
		default:
		{
		  TRACE_I(TF_SERIAL_CTRL, "Invalid SET Command=%d\n\r", testNumber);
		  TestNumber = 0;
		  command = CMD_TYPE_NONE;
		  break;
		}
	}
}

void process_get_commands(MOBLEUINT8 testNumber) {
	switch (testNumber) {
		case CMD_INDEX_RES_05_GENERIC:
		{
			Test_ApplicationTest_Get05_GenericOnOff(srcAddress, destAddress);
			break;
		}
		case CMD_INDEX_RES_02:
		{
			Read_CommandCount(srcAddress, destAddress);
			break;
		}
		default: {
			TRACE_I(TF_SERIAL_CTRL, "Invalid GET Command=%d\n\r", testNumber);
			TestNumber = 0;
			command = CMD_TYPE_NONE;
			break;
		}
	}
}

