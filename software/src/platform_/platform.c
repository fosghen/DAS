#include "arch/cc.h"
#include "platform.h"
#include "xil_cache.h"
#include "xinterrupt_wrap.h"
#include "xscutimer.h"
#include "xparameters.h"


XScuTimer TimerInstance;
volatile int TcpFastTmrFlag = 0;
volatile int TcpSlowTmrFlag = 0;

volatile u64_t tickcntr = 0;

char flag_timer = 0;

void disable_caches()
{
	Xil_DCacheDisable();
	Xil_ICacheDisable();
}

void init_platform()
{
	init_timer();
}

void TimerIntrHandler(void *CallBackRef)
{
	static int Tcp_Fasttimer = 0;
    static int Tcp_Slowtimer = 0;

    XScuTimer *TimerInstancePtr = (XScuTimer *)CallBackRef;
    XScuTimer_ClearInterruptStatus(TimerInstancePtr);
    tickcntr++;

    Tcp_Fasttimer++;
    Tcp_Slowtimer++;

	if(Tcp_Fasttimer % 50 == 0)
	{
		TcpFastTmrFlag = 1;
	}

	if(Tcp_Slowtimer % 100 == 0)
	{
		TcpSlowTmrFlag = 1;
	}
    flag_timer = 1;
}

void init_timer()
{
    XScuTimer_Config *TimerConfig;
    int Status;
    
    TimerConfig = XScuTimer_LookupConfig(XPAR_SCUTIMER_BASEADDR);
    Status = XScuTimer_CfgInitialize(&TimerInstance, TimerConfig, TimerConfig->BaseAddr);
    if (Status != XST_SUCCESS) {
        xil_printf("Timer init failed!\r\n");
        return;
    }
    Status = XScuTimer_SelfTest(&TimerInstance);
    if (Status != XST_SUCCESS) {
        xil_printf("Timer self-test failed!\r\n");
        return;
    }
    Status = XSetupInterruptSystem(&TimerInstance, &TimerIntrHandler,
                                 TimerInstance.Config.IntrId,
                                 TimerInstance.Config.IntrParent,
                                 XINTERRUPT_DEFAULT_PRIORITY);
    if (Status != XST_SUCCESS) {
        xil_printf("Interrupt setup failed!\r\n");
        return;
    }

    XScuTimer_EnableAutoReload(&TimerInstance);
    XScuTimer_EnableInterrupt(&TimerInstance);
    
    u32 TimerFreq = XPAR_CPU_CORE_CLOCK_FREQ_HZ / 2;
    XScuTimer_LoadTimer(&TimerInstance, TimerFreq / 1000 - 1);
    
    XScuTimer_Start(&TimerInstance);
    
    xil_printf("Timer started successfully\r\n");
}

u64_t get_time_ms()
{
	return tickcntr;
}

void cleanup_platform()
{
	disable_caches();
}

void update_timer(int freq)
{
	XScuTimer_Stop(&TimerInstance);
	XScuTimer_LoadTimer(&TimerInstance, XPAR_CPU_CORE_CLOCK_FREQ_HZ / 2 / freq);
	XScuTimer_Start(&TimerInstance);
}