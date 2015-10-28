/*
 * dds.h
 *
 *  Created on: 23 Oct 2015
 *      Author: tims
 */

#ifndef DDS_H_
#define DDS_H_

#include <avr/io.h>

/**
 * DDS Step constant
 *
 * This value should be defined in your Makefile (alongside F_CPU)
 *
 * This value is used to calculate the step size required to generate a
 * waveform at the specified frequency. The value is defined as:
 *
 * 	DDS_STEP_CONSTANT = (256^3 * ASM_LOOP_CYCLES) / F_CPU
 *
 * The value of 256 ^ 3 is derived from the length of the waveform table and
 * the offset of the bits used to index into the waveform table. The offset is
 * 256 << 16 (or 256 ^ 2).
 *
 * ASM_LOOP_CYCLES is the number of clock cycles required to complete one cycle
 * of the DDS generation loop, the current value is 9 clock cycles.
 *
 * F_CPU (this is defined in the Makefile as it is also used to scale
 * operations in <util/delay.h>) is the clock speed in Hz.
 *
 * It is easier to pre-calculate this value, this Google Sheets document can be
 * used to calculate this value:
 *
 * https://docs.google.com/spreadsheets/d/1Oab6_2TZ5SzAQHbTxMHLZFGgJnOQwO2SQYt80mpjkCc/edit?usp=sharing
 */
#ifndef DDS_STEP_CONSTANT
/* prevent compiler error by supplying a default based on a 30Mhz clock */
# warning "DDS_STEP_CONSTANT not defined for \"dac.h\""
#define DDS_STEP_CONSTANT 5.0331648
#endif

// Define DDS port (defaults to PORTD)
#ifndef DDS_PORT
#define DDS_PORT PORTD
#endif

// Define DDS DDR (defaults to DDRD)
#ifndef DDS_DDR
#define DDS_DDR DDRD
#endif

// Register used to break the DDS cycle
#ifndef DDS_BREAK_REGISTER
#define DDS_BREAK_REGISTER GPIOR0
#endif

// Bit within the Break register
#ifndef DDS_BREAK_BIT
#define DDS_BREAK_BIT GPIOR00
#endif

// Minimum frequency
#ifndef DDS_MIN_FREQ
#define 1
#endif

// Maximum frequency (supported by hardware), default is 1Mhz
#ifndef DDS_MAX_FREQ
#define 1000000
#endif

// Check that the DDS is enabled
#define DDS_IS_ENABLED 	((DDS_BREAK_REGISTER >> DDS_BREAK_BIT) & 1)
#define DDS_ENABLE 		DDS_BREAK_REGISTER |= _BV(DSA_BREAK_BIT)
#define DDS_DISABLE 	DDS_BREAK_REGISTER &= ~_BV(DDS_BREAK_BIT)
#define DDS_TOGGLE		DDS_BREAK_REGISTER ^= _BV(DDS_BREAK_BIT)

// Waveforms 
#define DDS_SIGN_WAVE 		0
#define DDS_TRIANGLE_WAVE 	1
#define DDS_SAWTOOTH_WAVE 	2
#define DDS_RSAWTOOTH_WAVE 	3
#define DDS_SQUARE_WAVE 	4

// Number of waveforms defined in the DDS waveform table.
#define DDS_WAVE_COUNT (DDS_SQUARE_WAVE + 1)

/**
 * Init DDS 
 */
void dds_init(void);

/**
 * Populate the waveform buffer with a waveform from the internally defined wave table.
 */
void dds_select_wave(uint8_t wave_idx);

/**
 * Start/break DDS waveform generation
 */
void dds_start(uint32_t frequency);

#endif /* DDS_H_ */