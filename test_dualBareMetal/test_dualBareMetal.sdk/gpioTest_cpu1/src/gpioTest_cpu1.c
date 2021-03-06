/*
 * gpioTest_cpu1.c
 *
 * This application programs CPU1 to check for
 * the shared memory flag set by CPU0. If the flag is
 * set to '1', then CPU1 shall write the value of two
 * board switches to shared memory, read back the value
 * of CPU0's switches from shared memory, and set its
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
#define GPIO_1_DEVICEID      XPAR_AXI_GPIO_1_DEVICE_ID

/***********************************************************/
uint8_t getMemFlag(uint32_t* flagAddr);
void    setMemFlag(uint32_t* flagAddr, uint8_t flagValue);

/***********************************************************/

int main()
{
    init_platform();
    usleep(500000);
    print("\n\rCPU1 initialized!!!\n\r");

    // Define variables
	XGpio XGpio_cpu1;
	int   sw_value;
	int   cpu0_sw_value;
	int   sharedMem_status;

	volatile uint32_t* sharedMem_flag;
	volatile uint32_t* sharedMem_cpu0_data;
	volatile uint32_t* sharedMem_cpu1_data;

	// Setup GPIO 1
	XGpio_Initialize(&XGpio_cpu1, GPIO_1_DEVICEID);
	XGpio_SetDataDirection(&XGpio_cpu1, 1, 0xFFFFFFF);
	XGpio_SetDataDirection(&XGpio_cpu1, 2, 0x0);

	// Setup shared memory
	sharedMem_flag      = (uint32_t*) SHAREDMEM_FLAG;
	sharedMem_cpu0_data = (uint32_t*) SHAREDMEM_CPU0_DATA;
	sharedMem_cpu1_data = (uint32_t*) SHAREDMEM_CPU1_DATA;

	// Super loop
	while(1)
	{
		// read switch values
		sw_value = XGpio_DiscreteRead(&XGpio_cpu1, 1);

		// access shared memory
		sharedMem_status = getMemFlag((uint32_t*)sharedMem_flag);
		if(sharedMem_status == 1)
		{
			// read the switch values of CPU0
			Xil_DCacheInvalidateRange((uint32_t)sharedMem_cpu0_data, 4);
			cpu0_sw_value = *sharedMem_cpu0_data;
			xil_printf("CPU1: CPU0 switch value = %d\n\r", cpu0_sw_value);

			// write CPU1 switch values to shared memory
			*sharedMem_cpu1_data = sw_value;
			Xil_DCacheFlushRange((uint32_t)sharedMem_cpu1_data, 4);

			// set the flag to allow CPU0 to write to shared memory
			setMemFlag((uint32_t*)sharedMem_flag, 0);
		}

		// set LEDs
		XGpio_DiscreteWrite(&XGpio_cpu1, 2, cpu0_sw_value);
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
