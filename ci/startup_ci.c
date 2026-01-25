#include <stdint.h>

// Forward declarations
int main(void);
void ResetISR(void);

// Minimal vector table + Reset handler for CI link proof.
// Not intended for flashing; just to make GCC link succeed in CI.

__attribute__((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void)) (0x20008000), // initial stack pointer (dummy)
    ResetISR                     // reset handler
};

void ResetISR(void)
{
    (void)main();
    while (1) {}
}
