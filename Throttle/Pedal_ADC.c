#define PART_TM4C123GH6PM 1;

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/adc.h"

#include "Globals_and_Defines.h"


//Function to read the throttle input
uint32_t get_throttle_input(void)
{
	uint32_t ADC_value;
	ADCIntClear(ADC0_BASE, 1);
	
	//Start conversion sequence
	ADCProcessorTrigger(ADC0_BASE, 1);
	while(!ADCIntStatus(ADC0_BASE, 1, false))
	{
	}
	
	//Get the ADC value
	ADCSequenceDataGet(ADC0_BASE, 1, &ADC_value);
	return ADC_value;
}


void ADC0_Setup(void)
{
	//Enable PORTD
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD))
	{}

	//Configure PORTE pin 3 for analog input
	//PE3 pedal input CH0
	GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);
		
	//Enable ADC0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0))
	{}
	
	//Setup ADC0 sequencer 1 to read the throttle pedal input
	ADCSequenceConfigure(ADC0_BASE, 1, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_CH4|ADC_CTL_IE|ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 1);
}

