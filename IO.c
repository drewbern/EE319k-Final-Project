// Written by: Zachary Chilton & Philip Tan
// Last Modified: 04/28/2017
// Initializes all IO

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Player.h"
#include "Projectile.h"

#define delayCounter 		320000
#define laserInput			((GPIO_PORTF_DATA_R&0x10) >> 4)

float xPos;
float yPos;

//converts ADC0 input
uint32_t ADC0In(void){  
	ADC0_PSSI_R = 0x08;							// initiate conversion
	
	while((ADC0_RIS_R&0x08) == 0){}	// while busy converting, wait
	ADC0_ISC_R = 0x08;							// done converting, indicate so
	
  return ADC0_SSFIFO3_R&0xFFF; 		// return conversion result
}

//converts ADC1 input
uint32_t ADC1In(void){  
	ADC1_PSSI_R = 0x08;							// initiate conversion
	
	while((ADC1_RIS_R&0x08) == 0){}	// while busy converting, wait
	ADC1_ISC_R = 0x08;							// done converting, indicate so
	
  return ADC1_SSFIFO3_R&0xFFF; 		// return conversion result
}

void gatherInputs(void){
	yPos = (((float)ADC0In())-2048)/2048;												//Percentage how far pushed away from center
	xPos = (((float)ADC1In())-2048)/2048;												//Percentage how far pushed away from center
}

//toggles red onboard LED for heartbeat
void beat(void) {
	GPIO_PORTF_DATA_R ^= 0x2;	//red LED toggled
}

//initializes PF1 (red onboard LED)
void IOInit(void) {
	SYSCTL_RCGCGPIO_R |= 0x20;
	uint8_t delay = 1;
	delay--;
	
	GPIO_PORTF_DIR_R |= 0x2;
	GPIO_PORTF_DEN_R |= 0x2;
}

//initializes ADC input
void ADCInit(void){
	uint32_t delay;
	SYSCTL_RCGCGPIO_R |= 0x18;   		// Port D and E clock initialized
  delay = 0;        						  // allow time for clocks
	delay++;
	
  GPIO_PORTE_DIR_R &= ~0x04;	 		// PE2 now input
  GPIO_PORTE_AFSEL_R |= 0x04;     // Ain1 set on PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // digital I/O cleared on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // analog function enabled on PE2
	
	GPIO_PORTD_DIR_R &= ~0x08;	 		// PD3 now input
  GPIO_PORTD_AFSEL_R |= 0x08;     // Ain1 set on PD3
  GPIO_PORTD_DEN_R &= ~0x08;      // digital I/O cleared on PD3
  GPIO_PORTD_AMSEL_R |= 0x08;     // analog function enabled on PD3
	
	SYSCTL_RCGCADC_R |= 0x03;				// ADC0 and ADC1 clock initialized
  delay = SYSCTL_RCGCADC_R;				// extra time to stabilize
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	
  ADC0_PC_R = 0x01; 							// 125 KHz sampling rate set
  ADC0_SSPRI_R = 0x0123;          // sq3 now highest priority
  ADC0_ACTSS_R &= ~0x0008;        // sq3 disabled during setup
  ADC0_EMUX_R &= ~0xF000;         // sq3 is software triggered
  ADC0_SSMUX3_R &= ~0x000F;       // SS3 channels cleared
  ADC0_SSMUX3_R += 4;             // Ain4 (PD3) set
  ADC0_SSCTL3_R = 0x0006;         // not measuring temperature, set END to prevent unintended behavior
	ADC0_IM_R &= ~0x0008;           // sq3 interrupts disabled
  ADC0_ACTSS_R |= 0x0008;       	// sq3 re-enabled
	
	ADC1_PC_R = 0x01; 							// 125 KHz sampling rate set
  ADC1_SSPRI_R = 0x0123;          // sq3 now highest priority
  ADC1_ACTSS_R &= ~0x0008;        // sq3 disabled during setup
  ADC1_EMUX_R &= ~0xF000;         // sq3 is software triggered
  ADC1_SSMUX3_R &= ~0x000F;       // SS3 channels cleared
  ADC1_SSMUX3_R += 1;             // Ain1 (PE2) set
  ADC1_SSCTL3_R = 0x0006;         // not measuring temperature, set END to prevent unintended behavior
	ADC1_IM_R &= ~0x0008;           // sq3 interrupts disabled
  ADC1_ACTSS_R |= 0x0008;       	// sq3 re-enabled
}

float getXPos(void) {
	return xPos;
}

float getYPos(void) {
	return yPos;																		
}
