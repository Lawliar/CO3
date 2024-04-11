/*
 * nonitor.c
 *
 *  Created on: May 2, 2022
 *      Author: alejandro
 */




#include "protocol.h"
//#include "main.h"
#include  "monitor.h"
#include  "ring.h"
#include "stdio.h"
#include "string.h"
#include "runtime.h"


#if (defined CO3_USE_STM32 && !defined CO3_USE_CHIBIOS) // ChibiOS has its own HAL
    #include "main.h" // STM32 HAL header
#elif defined CO3_USE_NXP
    #include "usb_device_config.h"
    #include "usb.h"
    #include "usb_device.h"
    #include "virtual_com.h"
    #include "usb_device_descriptor.h"
#elif defined CO3_USE_MICROCHIP
    #include "usb_start.h"
#endif



#if defined CO3_USE_FREERTOS
    #include "FreeRTOS.h"
    #include "task.h"

#elif defined CO3_USE_CHIBIOS
    #include "ch.h"
    #include "hal.h"
#endif


#if defined CO3_TEST_COMMANDLINE
    #include "sys_command_line.h"
#elif defined CO3_TEST_MODBUSDMA
    #include "modbus_rtu.h"
    extern UART_HandleTypeDef huart2;
#elif defined CO3_TEST_MIDIDMA
    #include "midi_lowlevel.h"
    #include "midi_main.h"
    extern UART_HandleTypeDef huart2;
    extern char midiBuffer[MIDI_BUF_SIZE] __attribute__( ( aligned( next_power_of_2(MIDI_BUF_SIZE)  ) ) );    /* for debugging */ //buffer
#elif defined CO3_TEST_SHELLYDIMMER
    #include "shelly.h"
#elif defined CO3_TEST_ATPARSER
    #include "ATParser.h"
#else
    #include "test.h"
#endif







#ifdef CO3_USE_CHIBIOS
//static StackType_t targetTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( configMINIMAL_STACK_SIZE * sizeof( StackType_t ) ) ) );
static THD_WORKING_AREA(targetTaskStack, configMINIMAL_STACK_SIZE*8);
//static StackType_t FuzzerTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( configMINIMAL_STACK_SIZE * sizeof( StackType_t ) ) ) );
static THD_WORKING_AREA(MonitorTaskStack, configMINIMAL_STACK_SIZE);
uint32_t CO3_TARGET_TIMEOUT;
#endif

void vStartMonitor( void );
void spawnNewTarget( void );
static void MonitorTask( void * pvParameters );
static void TargetTask( void * pvParameters );


void app_main( void )
{
    /* Start the MPU demo. */
    vStartMonitor();

#if defined CO3_USE_FREERTOS
    /* Start the scheduler. */
    vTaskStartScheduler();
#endif

    /* Should not get here. */
    for( ;; );
}


//creates the monitor task
void vStartMonitor( void )
{
#if defined CO3_USE_FREERTOS
    xTaskCreate(MonitorTask,
                "Monitor",
                configMINIMAL_STACK_SIZE,
                NULL,
                10,
                &AFLfuzzer.xTaskMonitor);
#elif defined CO3_USE_CHIBIOS

    CO3_TARGET_TIMEOUT  = chTimeMS2I(2000);
    AFLfuzzer.xTaskMonitor = chThdCreateStatic(MonitorTaskStack, sizeof(MonitorTaskStack), NORMALPRIO+1, MonitorTask, NULL);
#endif

}

#if defined CO3_USE_CHIBIOS
void killTarget(){
    eventmask_t events = 0;
    events |= TARGET_SHOULD_KILL_SELF;
    chEvtSignal(AFLfuzzer.xTaskTarget, events);
    return;
}
#endif

//creates a target task
void spawnNewTarget( void )
{
#if defined CO3_USE_FREERTOS
    xTaskCreate(TargetTask,
                    "Target",
#if defined CO3_USE_STM32_H743
                    configMINIMAL_STACK_SIZE * 8,
#elif defined CO3_USE_MICROCHIP_SAMD51
                    configMINIMAL_STACK_SIZE * 2,
#elif defined CO3_USE_NXP_K66F
                    configMINIMAL_STACK_SIZE,
#endif
                    NULL,
                    10,
                    &AFLfuzzer.xTaskTarget);
#elif defined CO3_USE_CHIBIOS
    AFLfuzzer.xTaskTarget = chThdCreateStatic(targetTaskStack, sizeof(targetTaskStack), NORMALPRIO+1, TargetTask, NULL);
#endif
}


static void MonitorTask( void * pvParameters )
{
#if defined CO3_USE_CHIBIOS
    AFLfuzzer.xTaskMonitor = chThdGetSelfX();
#endif

#if defined CO3_USE_NXP
    USB_DeviceApplicationInit();
#endif

    spawnNewTarget();  //spawn a new target

#if defined CO3_USE_FREERTOS
    uint32_t notificationvalue;
    ulTaskNotifyTakeIndexed(0,pdTRUE, TARGET_TIMEOUT/2);
#elif defined CO3_USE_CHIBIOS
    eventmask_t evt = chEvtWaitAny(ALL_EVENTS);
    while(!(evt & FUZZER_TARGET_READY)){
        killTarget();
        chThdWait(AFLfuzzer.xTaskTarget); // wait for the target to finish
        spawnNewTarget();
        evt = chEvtWaitAny(ALL_EVENTS);
    }
#endif
    // notification indexes Monitor
    // 0: notification from target when ready to execute
    // 1: notification from USB CDC ISR when fuzzing data has arrived
    // 2: notification from USB CDC ISR when TX completed
    // 3: notification from target to transmit function packet

    for(uint8_t j = 0; j<MAX_USB_FRAME; j++ )
    {
        AFLfuzzer.txbuffer[j]=0;
    }
    _sym_initialize();

#if defined CO3_USE_STM32
    //enable the cycle counter
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
#endif

    while(1)
    {
#if defined CO3_USE_SERIAL
        SerialReceiveInput();
#elif defined CO3_USE_USB
        ulTaskNotifyTakeIndexed(1,pdTRUE, portMAX_DELAY);
#endif
        AFLfuzzer.txCurrentIndex=REPORTING_BUFFER_STARTING_POINT; //Index Zero is reserved for the total length of the message, which  includes the first byte

#if (defined CO3_TEST_MODBUSDMA || defined CO3_TEST_MIDIDMA)
        // transmit data to huart
        HAL_UART_Transmit(&huart2,&AFLfuzzer.inputAFL.uxBuffer[AFL_BUFFER_STARTING_POINT] , AFLfuzzer.inputAFL.u32availablenopad-AFL_BUFFER_STARTING_POINT, 100);
        while(ulTaskNotifyTakeIndexed(4,pdTRUE, TARGET_TIMEOUT) != 1) //check if data was received by the target through USART on index 2
        {
            //if we do not receive  a confirmation we delete and recreate the target
            // the target will reconfigure USART4 in RX DMA mode
            vTaskDelete(AFLfuzzer.xTaskTarget);
            taskYIELD(); //lets the kernel clean the TCB
            //numberofcycles = 0;
            spawnNewTarget();
            // make sure the target is up
            ulTaskNotifyTakeIndexed(0,pdTRUE, TARGET_TIMEOUT/2);
            //send the payload through USART TX  pin PD5
            HAL_UART_Transmit(&huart2,&AFLfuzzer.inputAFL.uxBuffer[4] , AFLfuzzer.inputAFL.u32availablenopad-4, 100);
        }
#endif

#if defined CO3_USE_FREERTOS
        xTaskNotify(AFLfuzzer.xTaskTarget,0,eSetValueWithOverwrite);
#elif defined CO3_USE_CHIBIOS
        eventmask_t events = 0;
        events |= TARGET_GO_AHEAD;
        chEvtSignal(AFLfuzzer.xTaskTarget, events);
#endif

        // when we have around 64 bytes ready to transmit in the buffer
        // the target task will send a notification to this
#if defined CO3_USE_FREERTOS
        notificationvalue = ulTaskNotifyTakeIndexed(3,pdTRUE, TARGET_TIMEOUT);
        while(notificationvalue)
        {
            notiTarget = NOTI_TARGET;
            TransmitPack();
            notificationvalue = ulTaskNotifyTakeIndexed(3,pdTRUE, TARGET_TIMEOUT);
        }
#elif defined CO3_USE_CHIBIOS
        eventmask_t evt = chEvtWaitAnyTimeout(ALL_EVENTS, CO3_TARGET_TIMEOUT );
        while (evt == MORE_DATA_TO_COME){
            notiTarget = NOTI_TARGET;
            TransmitPack();
            evt = chEvtWaitAnyTimeout(ALL_EVENTS, CO3_TARGET_TIMEOUT );
        }
#endif
        notiTarget = NOTI_MONITOR;
        TransmitPack(); //transmit any remaining package in the buffer if any
#if defined CO3_USE_USB
        ulTaskNotifyTakeIndexed(2,pdTRUE, TARGET_TIMEOUT); // wait for the USB to finish transmission
#endif

#if defined CO3_USE_FREERTOS
        //delete the target
        vTaskDelete(AFLfuzzer.xTaskTarget);
        // lets the kernel clean artifacts
        taskYIELD();
#elif defined CO3_USE_CHIBIOS
        killTarget();
        chThdWait(AFLfuzzer.xTaskTarget);
        chThdYield();
#endif
        spawnNewTarget();
#if defined CO3_USE_FREERTOS
        ulTaskNotifyTakeIndexed(0,pdTRUE, TARGET_TIMEOUT/2);
#elif defined CO3_USE_CHIBIOS
        evt = chEvtWaitAny(ALL_EVENTS);
        while(!(evt & FUZZER_TARGET_READY)){
            killTarget();
            chThdWait(AFLfuzzer.xTaskTarget); // wait for the target to finish
            spawnNewTarget();
            evt = chEvtWaitAny(ALL_EVENTS);
        }
#endif
        _sym_initialize();


        //clean the buffers
        AFLfuzzer.txTotalFunctions=0;
        for(uint8_t i=1; i<8; i++)
        {
            AFLfuzzer.txbuffer[i]=0;
        }
        AFLfuzzer.txCurrentIndex=REPORTING_BUFFER_STARTING_POINT;
        for(uint8_t j = 0; j<MAX_USB_FRAME; j++ )
        {
            AFLfuzzer.txbuffer[j]=0;
        }
        AFLfuzzer.bRXcomplete = false;
        AFLfuzzer.inputLength = 0;
        RingZeroes(&AFLfuzzer.inputAFL);
    }
}


#ifdef CO3_TEST_CGC
extern unsigned int input_cur;
#elif defined CO3_TEST_SHELLYDIMMER
volatile int size_aux;
#endif


extern uint8_t GPSHandleRegion[];
static void TargetTask( void * pvParameters )
{
    uint32_t start_time_val, stop_time_val;
#if (defined CO3_TEST_MODBUSDMA || defined CO3_TEST_MIDIDMA)
    SytemCall_1_code(); //ERROR we need this line to receive data from serial port and it has to be called before it notifies to the monitor
#endif

#if defined CO3_USE_FREERTOS
    xTaskNotifyIndexed(AFLfuzzer.xTaskMonitor,0,1,eSetValueWithOverwrite); //notify the monitor task the target is ready
#elif defined CO3_USE_CHIBIOS
    AFLfuzzer.xTaskTarget = chThdGetSelfX();
    eventmask_t events = 0;
    events |= FUZZER_TARGET_READY;
    chEvtSignal(AFLfuzzer.xTaskMonitor, events);
    events = 0; // clears out the bits
#endif
    while(1){
#if defined CO3_USE_FREERTOS
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // wait for the notification coming from the Monitor task
#elif defined CO3_USE_CHIBIOS
        if(chThdShouldTerminateX()){
           break;
        }
        //ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // wait for the data coming from the fuzzer task
        eventmask_t evt = chEvtWaitAny(ALL_EVENTS);
        //check if the monitor tells me to exit
        if(evt & TARGET_SHOULD_KILL_SELF){
            break;
        }
        // check if monitor tells me to continue
        if(!(evt & TARGET_GO_AHEAD)){
            // something is wrong, but we cannot kill ourself
            // I'll just deadloop here, so that when attached to a debugger, everyone would know something is wrong
            while(1){}
        }
#endif

#if defined(CO3_USE_STM32)
        start_time_val = DWT->CYCCNT;
#endif

#if defined CO3_TEST_CGC
        input_cur = 0;
#elif defined CO3_TEST_COMMANDLINE
        cli_init(9600);
        QUEUE_INIT(cli_rx_buff);
#endif

        uint32_t input_len = AFLfuzzer.inputAFL.u32available - AFL_BUFFER_STARTING_POINT;
#if defined CO3_TEST_MODBUSDMA
        if (input_len >= MODBUS_MAX_FRAME_SIZE){
            input_len = MODBUS_MAX_FRAME_SIZE;
        }
         _sym_symbolize_memory((char*)modbusRxTxBuffer, input_len, false);
#elif defined CO3_TEST_MIDIDMA
        if (input_len >= MIDI_BUF_SIZE){
            input_len = MIDI_BUF_SIZE;
        }
        _sym_symbolize_memory((char*)midiBuffer,input_len, false);
#else
        _sym_symbolize_memory((char*)(AFLfuzzer.inputAFL.uxBuffer+AFL_BUFFER_STARTING_POINT),input_len, false);
#endif

#if defined CO3_TEST_CGC
        test();
#elif defined CO3_TEST_COMMANDLINE
        if(AFLfuzzer.inputAFL.u32availablenopad >0 && AFLfuzzer.inputAFL.u32availablenopad <1024)
        {
            for(int i=0; i<= AFLfuzzer.inputAFL.u32availablenopad-4; i++)
            {
                QUEUE_IN(cli_rx_buff, AFLfuzzer.inputAFL.uxBuffer[4+i]);
            }
                CLI_RUN();
        }
        else
        {
            if(AFLfuzzer.inputAFL.u32availablenopad == 0)DbgConsole_Printf("Zero target\n");
        }
#elif defined CO3_TEST_MODBUSDMA
        modbusSlaveHandler();
#elif defined CO3_TEST_MIDIDMA
        main_midi(input_len);
#elif defined CO3_TEST_SHELLYDIMMER
        size_aux = AFLfuzzer.inputAFL.u32availablenopad-AFL_BUFFER_STARTING_POINT;
        main_shelly_test((char*)(AFLfuzzer.inputAFL.uxBuffer+AFL_BUFFER_STARTING_POINT));
#elif defined CO3_TEST_ATPARSER
        mainparser(&AFLfuzzer.inputAFL.uxBuffer[AFL_BUFFER_STARTING_POINT], AFLfuzzer.inputAFL.u32availablenopad-AFL_BUFFER_STARTING_POINT);
#elif defined CO3_TEST_PLC
        modbusparsing(&AFLfuzzer.inputAFL.uxBuffer[AFL_BUFFER_STARTING_POINT], AFLfuzzer.inputAFL.u32availablenopad-AFL_BUFFER_STARTING_POINT );
#else
        //
        test(&AFLfuzzer.inputAFL.uxBuffer[4], AFLfuzzer.inputAFL.u32availablenopad-4);
        //HAL_UART_Transmit_test(&huart2,  (unsigned char*)(AFLfuzzer.inputAFL.uxBuffer+AFL_BUFFER_STARTING_POINT), AFLfuzzer.inputAFL.u32available - AFL_BUFFER_STARTING_POINT,  HAL_MAX_DELAY);
        //gps_init((gps_t*)GPSHandleRegion);
        //gps_process((gps_t*)GPSHandleRegion,&AFLfuzzer.inputAFL.uxBuffer[4], AFLfuzzer.inputAFL.u32availablenopad-4 );
        //memset(GPSHandleRegion,0, next_power_of_2(gps_handle_t_size));
#endif

        _sym_end();

#if defined CO3_USE_STM32
        stop_time_val = DWT->CYCCNT;
    printf("time:%u\n",stop_time_val - start_time_val);
#endif
    }
}


