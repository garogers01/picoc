/*
    GNU linker script for STM32F103
*/

/* Specify the memory areas */
MEMORY
{
    rom (rx)      : ORIGIN = 0x08000000, LENGTH = 128K
    ram (xrw)     : ORIGIN = 0x20000000, LENGTH =  20K
}

INCLUDE opencm3/libopencm3/lib/libopencm3_stm32f1.ld
