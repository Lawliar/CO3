/* Includes ------------------------------------------------------------------*/
#include <stdlib.h> 
#include "main.h"
#include "leds.h" 
#include "midi_lowlevel.h" 
#include "mm_midimsgbuilder.h" 
#include "mm_midirouter_standard.h" 
//#include "afl.h"
//#include "fuzzing.h"
//#include "McuASAN.h"
#include "protocol.h"



#define MIDI_MSG_DEBUG 
#define NUM_MIDI_MSGS 10 

char midiBuffer[MIDI_BUF_SIZE] __attribute__( ( aligned( next_power_of_2(MIDI_BUF_SIZE)  ) ) );    /* for debugging */ //buffer
MIDIMsgBuilder midiMsgBuilder __attribute__( ( aligned( next_power_of_2(sizeof(MIDIMsgBuilder) )  ) ) );                   //buffers
MIDI_Router_Standard midiRouter __attribute__( ( aligned( next_power_of_2(sizeof(MIDI_Router_Standard) )  ) ) );                //buffers

//extern MIDIMsgBuilder_State_t lastState; //global variable



void MIDI_note_on_do(void *data, MIDIMsg *msg)
{
    if ((MIDIMSG_GET_CHANNEL(msg->data[0]) == 1)
            && (msg->data[1] == 60)) {
        LEDs_greenSet();
    }
    if ((MIDIMSG_GET_CHANNEL(msg->data[0]) == 1)
            && (msg->data[1] == 67)) {
        LEDs_redSet();
    }
    MIDIMsg_free(msg); //Bug identified by DICE
    //free(msg);
}

void MIDI_note_off_do(void *data, MIDIMsg *msg)
{
    if ((MIDIMSG_GET_CHANNEL(msg->data[0]) == 1)
            && (msg->data[1] == 60)) {
        LEDs_greenReset();
    }
    if ((MIDIMSG_GET_CHANNEL(msg->data[0]) == 1)
            && (msg->data[1] == 67)) {
        LEDs_redReset();
    }
    MIDIMsg_free(msg); //Bug identified by DICE
    //free(msg);
}

void MIDI_entry(int numItems){
	int MIDIlastIndex = 0;
	while (numItems--) {
		MIDI_process_byte(midiBuffer[MIDIlastIndex]);
		MIDIlastIndex = (MIDIlastIndex + 1) % MIDI_BUF_SIZE;
	}
}

int main_midi(int numItems)
{
    int dummy;
    MIDIMsgBuilder_init(&midiMsgBuilder);
    /* Initialize the MIDI router */
    MIDI_Router_Standard_init(&midiRouter);
    MIDI_Router_addCB(&midiRouter.router, MIDIMSG_NOTE_ON, 1, MIDI_note_on_do, &dummy); 
    MIDI_Router_addCB(&midiRouter.router, MIDIMSG_NOTE_OFF, 1, MIDI_note_off_do, &dummy); 
    MIDI_entry(numItems);
    return 0;
}






void do_stuff_with_msg(MIDIMsg *msg)
{
    MIDI_Router_handleMsg(&midiRouter.router, msg);
}

void MIDI_process_byte(char byte)
{

	switch (MIDIMsgBuilder_update(&midiMsgBuilder,byte)) {

        case MIDIMsgBuilder_Action_NONE:
            break;
        case MIDIMsgBuilder_Action_MSG_COMPLETE:
            do_stuff_with_msg(midiMsgBuilder.msg);
            MIDIMsgBuilder_init(&midiMsgBuilder); /* reset builder */
            break;
        default:
            break;
    }
    //lastState = midiMsgBuilder.state;
}


