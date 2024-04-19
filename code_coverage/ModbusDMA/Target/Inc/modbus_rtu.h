#ifndef MODBUS_RTU_H_INCLUDED
#define MODBUS_RTU_H_INCLUDED

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "modbus_rtu_conf.h"

typedef struct
{
	uint16_t modbusRxCountV;
	xSemaphoreHandle xFrameReadySemaphoreV;
	xTaskHandle xModbusSlaveTaskV;
	xTaskHandle xFuzzerTaskV;
	uint8_t modbusSlaveFrameReadyFlagV;
}modbus_t;



void modbusSlaveStartTread(void);
xTaskHandle modbusSlaveGetTaskHandle(void);
void * modbusSlaveGetGata(uint16_t address);
void modbusSlavePutData(uint16_t address, void * value, uint16_t len);


void modbusSlaveHandler();
uint8_t modbusSlaveHardwareInit(void);
void modbusSlaveStartTransmitting(uint16_t bytesNum);
void modbusSlaveStartReceiving(void);
void SytemCall_1_code();
void cleanInitShadow();
extern uint8_t modbusRxTxBuffer[MODBUS_MAX_FRAME_SIZE];
void SytemCall_2_code();
#endif /* MODBUS_RTU_H_INCLUDED */
