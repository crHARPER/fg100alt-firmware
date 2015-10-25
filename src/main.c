#include <util/delay.h>
#include <avr/wdt.h>
#include <math.h>

#include "defines.h"
#include "waveforms.h"

#include "lcd.h"
#include "ui.h"
#include "dac.h"

State g_state = {500, 2, 0};
uint8_t g_buttonStatus[BUTTON_COUNT];

int main(void);
void loop(void);

void init(void) {
	wdt_disable();

	// Initialize the LCD and display splash message
	lcd_init();
	ui_show_splash();

	dac_init();

	// Configure strobe pin
	DDRB = DDRB & ~_BV(BUTTON_STROBE);
}

/**
 * Check the status of a button and raise events
 */
void raise_button_events(uint8_t pin, uint8_t button) {
	PORTB = (PORTB & 0xFF & ~(0x0F << PORTB2)) | _BV(pin);
	_delay_ms(10);

	if (PINB & _BV(BUTTON_STROBE)) {
		if (g_buttonStatus[button]) {
			ui_handle_event(button, BUTTON_REPEAT, &g_state);
		} else {
			ui_handle_event(button, BUTTON_PRESS, &g_state);
		}
		g_buttonStatus[button] = BUTTON_DOWN;
	} else {
		if (g_buttonStatus[button]) {
			ui_handle_event(button, BUTTON_RELEASE, &g_state);
		}
		g_buttonStatus[button] = BUTTON_UP;
	}
	_delay_ms(10);
}


void generate_ui_events() {
	raise_button_events(MODE_PIN, MODE_BUTTON);
	raise_button_events(CURSOR_PIN, CURSOR_BUTTON);
	raise_button_events(PLUS_PIN, PLUS_BUTTON);
	raise_button_events(MINUS_PIN, MINUS_BUTTON);
	_delay_ms(50);
}

int main(void) {
	init();

	dac_set_freq(500);
	dac_set_waveform(waves[0]);

	_delay_ms(2000);

	ui_redraw_display(&g_state);
	loop();

	return 1;
}

void loop(void) {
	while(1) {
		generate_ui_events();
	}

	//	dac_start();

}
