#ifndef __PLATFORM_H_
#define __PLATFORM_H_

#include <arch/cc.h>
#ifdef __cplusplus
extern "C" {
#endif

extern char flag_timer;

void init_platform();
void cleanup_platform();
void init_timer();
void TimerIntrHandler(void *CallBackRef);
void platform_setup_timer();
void platform_enable_interrupts();
void update_timer(int freq);
u64_t get_time_ms();

#ifdef __cplusplus
}
#endif

#endif
