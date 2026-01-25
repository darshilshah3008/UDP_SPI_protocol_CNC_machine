/* CI linker script (simple + stable for GCC link proof) */
ENTRY(ResetISR)

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
  SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 32K
}

SECTIONS
{
  .text :
  {
    KEEP(*(.isr_vector))
    KEEP(*(.intvecs))
    *(.text*)
    *(.rodata*)
    *(.ARM.exidx*)
    *(.ARM.extab*)
    KEEP(*(.init))
    KEEP(*(.fini))
    _etext = .;
  } > FLASH

  .data : AT(_etext)
  {
    _sdata = .;
    *(.data*)
    _edata = .;
  } > SRAM

  .bss :
  {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
    _end = .;   /* heap starts here (needed for _sbrk) */
  } > SRAM
}
