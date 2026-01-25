#pragma once

// TI CCS intrinsic compatibility for GCC CI builds
#ifndef _delay_cycles
#define _delay_cycles(x) __asm__ volatile ("nop")
#endif

