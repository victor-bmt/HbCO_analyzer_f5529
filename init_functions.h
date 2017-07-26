#ifndef INIT_FUNCTIONS_H_
#define INIT_FUNCTIONS_H_

unsigned int DAC_660, DAC_800, DAC_940;

#define INIT_DAC        170
#define DAC_THRESH_HIGH 250
#define DISPLAY_PERIOD  25000

void init_Ports(void);
void init_LED_block(void);
void init_detection_block(void);
void init_PGA(void);
void init_ADC12(void);
void init_DMA0(void);
void init_display(void);

#endif /* INIT_FUNCTIONS_H_ */
