#include <stdint.h>

__attribute__((naked)) void pspset(uint32_t v)
{
  (void)v;
  __asm volatile ("bx lr");
}

__attribute__((naked)) void swpush(void)
{
  __asm volatile ("bx lr");
}

__attribute__((naked)) void swpop(void)
{
  __asm volatile ("bx lr");
}

uint32_t get_sv_val(void)
{
  return 0;
}

uint32_t getr0(void)
{
  return 0;
}
