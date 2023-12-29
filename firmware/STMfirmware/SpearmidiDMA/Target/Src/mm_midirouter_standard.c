#include "mm_midirouter_standard.h" 
#include "main.h"
#include "protocol.h"
#include "midi_lowlevel.h"

extern char midiBuffer[MIDI_BUF_SIZE] __attribute__( ( aligned( next_power_of_2(MIDI_BUF_SIZE)  ) ) );    /* for debugging */ //buffer
extern UART_HandleTypeDef huart4;

void MIDI_Router_Standard_init(MIDI_Router_Standard *mr)
{
    MIDI_Router_init(&mr->router);
    MIDIMsg_Byte_t chan;
    for (chan = 0; chan < MIDI_Router_NUM_CHANS; chan++) {
        MIDI_CC_CB_Router_init(&mr->cbRouters[chan]);
        /* Add control change routers */
        MIDI_Router_addCB(&mr->router, MIDIMSG_CNTRL_CHNG, chan, MIDI_CC_CB_Router_route,
                (void*)&mr->cbRouters[chan]);
    }
}



void SytemCall_1_code()
{
	MIDI_low_level_setup();
}

void MIDI_low_level_setup(void)
{
    while(HAL_UARTEx_ReceiveToIdle_DMA(&huart4, (uint8_t*) midiBuffer, MIDI_BUF_SIZE) != HAL_OK)
    {
        HAL_UART_Abort(&huart4);
        HAL_UART_DeInit(&huart4);
        HAL_UART_Init(&huart4);
    }
    //status = 1;
    //return status;
}

void SytemCall_2_code()
{
	while(HAL_UARTEx_ReceiveToIdle_DMA(&huart4, (uint8_t*) midiBuffer, MIDI_BUF_SIZE) != HAL_OK)
	{
		HAL_UART_Abort(&huart4);
		HAL_UART_DeInit(&huart4);
		HAL_UART_Init(&huart4);
	}
}

void MIDI_process_buffer(void)
{
    int MIDIlastIndex = 0;
    int numItems = ulTaskNotifyTake(pdTRUE, portMAX_DELAY); //wait for data coming from USART
    while (numItems--) {
        MIDI_process_byte(midiBuffer[MIDIlastIndex]);
        /* increase last index so the function starts at the correct point next interrupt */
        MIDIlastIndex = (MIDIlastIndex + 1) % MIDI_BUF_SIZE; 
    }

    //SytemCall_2_code();  // configure port to receive more data
}
