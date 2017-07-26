#include <msp430.h>
#include "init_functions.h"
#include "calc_functions.h"

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;       // Stop watchdog timer

    UCSCTL4 = SELA_5 + SELM_5;      // XT2CLK for ACLK and MCLK

    // select XT2CLK for ACLK

    init_Ports();
    init_LED_block();
    init_detection_block();
    init_PGA();
    init_ADC12();
    init_DMA0();
    init_display();

    unsigned int DC_660 = 0;
    unsigned int AC_660 = 0;
    unsigned int DC_800 = 0;
    unsigned int AC_800 = 0;
    unsigned int DC_940 = 0;
    unsigned int AC_940 = 0;

    __bis_SR_register(GIE);     // enable interrupts globally
    __no_operation();           // For debugger

    while(1)
    {
        get_DCs(&DC_660, &DC_800, &DC_940);
        get_ACs(&AC_660, &AC_800, &AC_940);

        SpCO = calc_SpCO(DC_660, AC_660,
                         DC_800, AC_800,
                         DC_940, AC_940);

        adjust_to_ADC(DC_660, AC_660,
                      DC_800, AC_800,
                      DC_940, AC_940);
    }
}
