/*
 * midi_main.h
 *
 *  Created on: Jan 30, 2022
 *      Author: alejandro
 */

#ifndef TARGET_MMMIDI_INC_MIDI_MAIN_H_
#define TARGET_MMMIDI_INC_MIDI_MAIN_H_
#include "mm_midimsg.h"

int main_midi(void);
void MIDI_note_on_do(void *data, MIDIMsg *msg);
void MIDI_note_off_do(void *data, MIDIMsg *msg);
#endif /* TARGET_MMMIDI_INC_MIDI_MAIN_H_ */
