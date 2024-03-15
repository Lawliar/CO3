#include "midi_lowlevel.h"
#include "leds.h" 
#include "main.h"
#include "protocol.h"
extern char midiBuffer[MIDI_BUF_SIZE] __attribute__( ( aligned( next_power_of_2(MIDI_BUF_SIZE)  ) ) );    /* for debugging */ //buffer
extern UART_HandleTypeDef huart4;
/*

char redzone1[ext_power_of_2(MIDI_BUF_SIZE)] __attribute__( ( aligned( next_power_of_2(MIDI_BUF_SIZE)  ) ) );

char redzone2[ext_power_of_2(MIDI_BUF_SIZE)] __attribute__( ( aligned( next_power_of_2(MIDI_BUF_SIZE)  ) ) );

MIDIMsgBuilder midiMsgBuilder __attribute__( ( aligned( next_power_of_2(sizeof(MIDIMsgBuilder) )  ) ) );                   //buffers

char redzone3[next_power_of_2(sizeof(MIDIMsgBuilder))] __attribute__( ( aligned( next_power_of_2(sizeof(MIDIMsgBuilder) )  ) ) );

MIDI_Router_Standard midiRouter __attribute__( ( aligned( next_power_of_2(sizeof(MIDI_Router_Standard) )  ) ) );                //buffers

char redzone4[next_power_of_2(sizeof(MIDI_Router_Standard))] __attribute__( ( aligned( next_power_of_2(sizeof(MIDI_Router_Standard) )  ) ) );
*/



void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if(huart==&huart4)
    {
       HAL_UART_DMAStop(&huart4);

        while(HAL_UARTEx_ReceiveToIdle_DMA(&huart4, (uint8_t*) midiBuffer, MIDI_BUF_SIZE) != HAL_OK)
        {
            HAL_UART_Abort(&huart4);
            HAL_UART_DeInit(&huart4);
            HAL_UART_Init(&huart4);
       }
    }
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
#if DUALCOREFUZZ == 0

  //Fuzzer_t *pAFLfuzzer = (Fuzzer_t *)AFLfuzzerRegion;
#endif

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if(huart==&huart4)
    {
        // notify with the buffer size
        xTaskNotifyIndexedFromISR(AFLfuzzer.xTaskMonitor,4,1,eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

}


