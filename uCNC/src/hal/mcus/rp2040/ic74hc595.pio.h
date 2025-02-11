// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// --------- //
// ic74hc595 //
// --------- //

#define ic74hc595_wrap_target 0
#define ic74hc595_wrap 5

static const uint16_t ic74hc595_program_instructions[] = {
            //     .wrap_target
    0x80a0, //  0: pull   block                      
    0xe03f, //  1: set    x, 31                      
    0xe000, //  2: set    pins, 0                    
    0x7001, //  3: out    pins, 1         side 0     
    0x1843, //  4: jmp    x--, 3          side 1     
    0xe002, //  5: set    pins, 2                    
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program ic74hc595_program = {
    .instructions = ic74hc595_program_instructions,
    .length = 6,
    .origin = -1,
};

static inline pio_sm_config ic74hc595_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + ic74hc595_wrap_target, offset + ic74hc595_wrap);
    sm_config_set_sideset(&c, 2, true, false);
    return c;
}

#include "hardware/clocks.h"
static inline void ic74hc595_program_init(PIO pio, uint sm, uint offset, uint pin_data, uint pin_clk, uint pin_latch, uint freq) {
	pio_sm_config c = ic74hc595_program_get_default_config(offset);
	sm_config_set_out_pins(&c, pin_data, 1); // define one pin to respond to the out instruction
	sm_config_set_set_pins(&c, pin_clk, 2);	 // define all pins to respond to the set instruction
	sm_config_set_sideset_pins(&c, pin_clk); // define clock as the side set base pin (and only)
	// Only support MSB-first in this example code (shift to left, no auto push/pull, threshold=nbits)
	sm_config_set_out_shift(&c, false, false, 32);
	// All pins output
	pio_sm_set_consecutive_pindirs(pio, sm, pin_data, 3, true);
	pio_gpio_init(pio, pin_data);
	pio_gpio_init(pio, pin_clk);
	pio_gpio_init(pio, pin_latch);
	// We only need TX, so get an 8-deep FIFO!
	sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
	// SM transmits 1 bit per 8 execution cycles.
	float div = (float)clock_get_hz(clk_sys) / (freq << 1);
	sm_config_set_clkdiv(&c, div);
	pio_sm_init(pio, sm, offset, &c);
	pio_sm_set_enabled(pio, sm, true);
}
static inline void ic74hc595_program_write(PIO pio, uint sm, uint out) {
    pio_sm_put_blocking(pio, sm, out);
}

#endif
