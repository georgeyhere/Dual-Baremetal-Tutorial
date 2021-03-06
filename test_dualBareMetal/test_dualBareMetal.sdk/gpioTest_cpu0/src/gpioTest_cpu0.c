/*
 * gpioTest_cpu0.c
 *
 * This application programs CPU0 to check for
 * the shared memory flag set by CPU1. If the flag is
 * set to '0', then CPU0 shall write the value of two
 * board switches to shared memory, read back the value
 * of CPU1's switches from shared memory, and set its
 * LEDs accordingly.
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"
#include "sleep.h"
#include "xil_cache.h"

/***********************************************************/
#define SHAREDMEM_FLAG       0x3000001
#define SHAREDMEM_CPU0_DATA  0x3000010
#define SHAREDMEM_CPU1_DATA  0x3000100
#define GPIO_0_DEVICEID      XPAR_AXI_GPIO_0_DEVICE_ID

/***********************************************************/
uint8_t getMemFlag(uint32_t* flagAddr);
void    setMemFlag(uint32_t* flagAddr, uint8_t flagValue);

/***********************************************************/
int main()
{
    init_platform();
    print("\n\rCPU0 initialized!!!\n\r");

    // Define variables
    XGpio XGpio_cpu0;
    int   sw_value;
    int   cpu1_sw_value;
    int   sharedMem_status;

    volatile uint32_t* sharedMem_flag;
    volatile uint32_t* sharedMem_cpu0_data;
    volatile uint32_t* sharedMem_cpu1_data;

    // Setup GPIO 0
    XGpio_Initialize(&XGpio_cpu0, GPIO_0_DEVICEID);
    XGpio_SetDataDirection(&XGpio_cpu0, 1, 0xFFFFFFF);
    XGpio_SetDataDirection(&XGpio_cpu0, 2, 0x0);

    // Setup shared memory
    sharedMem_flag      = (uint32_t*) SHAREDMEM_FLAG;
    sharedMem_cpu0_data = (uint32_t*) SHAREDMEM_CPU0_DATA;
    sharedMem_cpu1_data = (uint32_t*) SHAREDMEM_CPU1_DATA;

    // Initialize the memory flag to 0
    setMemFlag((uint32_t*)sharedMem_flag, 0);
    sharedMem_status = getMemFlag((uint32_t*)sharedMem_flag);
    xil_printf("CPU0: Initial memory flag value: %d\n\r", sharedMem_status);

    // Initialize the shared memory data addresses to 0
    *sharedMem_cpu0_data = 0;
    Xil_DCacheFlushRange((uint32_t)sharedMem_cpu0_data, 4);
    *sharedMem_cpu1_data = 0;
    Xil_DCacheFlushRange((uint32_t)sharedMem_cpu1_data, 4);

    // Super loop
    while(1)
    {
    	// read switch values
    	sw_value = XGpio_DiscreteRead(&XGpio_cpu0, 1);

    	// access shared memory
    	sharedMem_status = getMemFlag((uint32_t*)sharedMem_flag);
    	if(sharedMem_status == 0)
    	{
    		// read the switch values of CPU1
    		Xil_DCacheInvalidateRange((uint32_t)sharedMem_cpu1_data, 4);
    		cpu1_sw_value = *sharedMem_cpu1_data;
    		xil_printf("CPU0: CPU1 switch value = %d\n\r", cpu1_sw_value);

    		// write CPU0 switch values to shared memory
    		*sharedMem_cpu0_data = sw_value;
    		Xil_DCacheFlushRange((uint32_t)sharedMem_cpu0_data, 4);

    		// set the flag to allow CPU1 to write to shared memory
    		setMemFlag((uint32_t*)sharedMem_flag, 1);
    	}

    	// set LEDs
    	XGpio_DiscreteWrite(&XGpio_cpu0, 2, cpu1_sw_value);
    	usleep(100000);
    }

    cleanup_platform();
    return 0;
}


uint8_t getMemFlag(uint32_t* flagAddr)
{
	Xil_DCacheInvalidateRange((uint32_t)flagAddr, 1);
	return *flagAddr;
}

void setMemFlag(uint32_t* flagAddr, uint8_t flagValue)
{
	*flagAddr = flagValue;
	Xil_DCacheFlushRange((uint32_t)flagAddr, 1);
}






