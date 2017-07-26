#include <msp430.h>
#include "init_functions.h"
#include "calc_functions.h"

void init_Ports(void)
{
    P1OUT = 0x00;
    P1DIR = 0xFF;
    P2OUT = 0x00;
    P2DIR = 0xFF;
    P3OUT = 0x00;
    P3DIR = 0xFF;
    P4OUT = 0x00;
    P4DIR = 0xFF;
    P5OUT = 0x00;
    P5DIR = 0xFF;
    P6OUT = 0x00;
    P6DIR = 0xFF;
    P7OUT = 0x00;
    P7DIR = 0xFF;
    P8OUT = 0x00;
    P8DIR = 0xFF;

    return;
}

void init_LED_block(void)
{
    // TB0 - TIMER FOR EMISSION BLOCK

    // ACLK
    TB0CTL = TBSSEL_1;

    // interrupts when count to CCR0-CCR6
    TB0CCTL0 = CCIE;
    TB0CCTL1 = CCIE;
    TB0CCTL2 = CCIE;
    TB0CCTL3 = CCIE;
    TB0CCTL4 = CCIE;
    TB0CCTL5 = CCIE;
    TB0CCTL6 = CCIE;

    // set CCR0-CCR6 values; T = 100 us = 2500
    TB0CCR0 = 175;  // 7 us - start TA0, TA1
    TB0CCR1 = 250;  // 10 us - LED1 switch off
    TB0CCR2 = 625;  // 25 us - LED2 switch on
    TB0CCR3 = 875;  // 35 us - LED2 switch off
    TB0CCR4 = 1250; // 50 us - LED3 switch on
    TB0CCR5 = 1500; // 60 us - LED3 switch off
    TB0CCR6 = 2500; // 100 us - LED1 switch on

    // P1.0 - P1.2 - for emission block
    P1DIR |= BIT0 + BIT1 + BIT2; // P1.0 - EN, P1.1 - A0, P1.2 - A1
    P1OUT &= ~(BIT0 + BIT1 + BIT2);

    // SPI Master-mode for DAC
    P3SEL |= BIT3;  // SIMO
    P2SEL |= BIT7;  // CLK
    P3DIR |= BIT4;  // SYNC
    P3OUT |= BIT4;  // active low

    // set reset state
    UCA0CTL1 |= UCSWRST;

    // master-mode, 3-wire, MSB first
    UCA0CTL0 =UCMST + UCSYNC + UCMSB;

    // ACLK
    UCA0CTL1 |= UCSSEL_1;

    // active state
    UCA0CTL1 &= ~UCSWRST;

    // no modulation
    UCA0MCTL = 0;

    // set init values

    // DAC_660
    DAC_660 = INIT_DAC << 4;
    DAC_660 |= 1 << 13; // active mode

    P3OUT &= ~BIT4;  // SYNC - enable DAC RX
    UCA0TXBUF = (DAC_660 & 0xFF00) >> 8;
    while ( !(UCA0IFG & UCTXIFG) );
    UCA0TXBUF = DAC_660 & 0xFF;
    while ( !(UCA0IFG & UCTXIFG) );
    P3OUT |= BIT4;  // SYNC - disable DAC RX

    // DAC_800
    DAC_800 = 1 << 14;
    DAC_800 |= INIT_DAC << 4;
    DAC_800 |= 1 << 13; // active mode

    P3OUT &= ~BIT4;  // SYNC - enable DAC RX
    UCA0TXBUF = (DAC_800 & 0xFF00) >> 8;
    while ( !(UCA0IFG & UCTXIFG) );
    UCA0TXBUF = DAC_800 & 0xFF;
    while ( !(UCA0IFG & UCTXIFG) );
    P3OUT |= BIT4;  // SYNC - disable DAC RX

    //DAC_940
    DAC_940 = 1 << 15;
    DAC_940 |= INIT_DAC << 4;
    DAC_940 |= 1 << 13; // active mode

    P3OUT &= ~BIT4;  // SYNC - enable DAC RX
    UCA0TXBUF = (DAC_940 & 0xFF00) >> 8;
    while ( !(UCA0IFG & UCTXIFG) );
    UCA0TXBUF = DAC_940 & 0xFF;
    while ( !(UCA0IFG & UCTXIFG) );
    P3OUT |= BIT4;  // SYNC - disable DAC RX

    return;
}

void init_detection_block(void)
{
    // TA0, TA1 - TIMERS FOR DETECTION BLOCK

    // ACLK and interrupt enable
    TA0CTL = TASSEL_1 + TAIE;
    TA1CTL = TASSEL_1 + TAIE;

    // interrupts when count to TA0CCR0-CCR4, TA1CCR0-CCR2
    TA0CCTL1 = SCCI + CCIE;
    TA0CCTL2 = SCCI + CCIE;
    TA0CCTL3 = SCCI + CCIE;
    TA0CCTL4 = SCCI + CCIE;

    TA1CCTL1 = SCCI + CCIE;
    TA1CCTL2 = SCCI + CCIE;

    // set TA0CCR0-CCR4, TA1CCR0-CCR2 values
    TA0CCR1 = 375;  // 15 us - switch off IN1_DA31
    TA0CCR2 = 625;  // 25 us - switch on IN3_DA31, IN4_DA31
    TA0CCR3 = 1000; // 40 us - switch off IN3_DA31
    TA0CCR4 = 1250; // 50 us - switch on IN1_DA32, IN2_DA32
    TA0CCR0 = 1625; // 65 us - switch off IN1_DA32, stop TA0, TA1

    TA1CCR1 = 50;   // 2 us - switch off IN2_DA31
    TA1CCR2 = 675;  // 27 us - switch off IN4_DA31
    TA1CCR0 = 1300; // 52 us - switch off IN2_DA32


    // P2.0 - P2.5 - for detection block

    // P2.0 - IN1_DA31, P2.1 - IN2_DA31, P2.2 - IN3_DA31, P2.3 - IN4_DA31
    // P2.4 - IN1_DA32, P2.5 - IN2_DA32
    P2DIR |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5;
    P2OUT &= ~(BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5);

    return;
}

void init_PGA(void)
{
    P1DIR |= BIT3 + BIT4;   // DA41, DA42
    P1DIR |= BIT5 + BIT6;   // DA51, DA52
    P3DIR |= BIT0 + BIT1;   // DA61, DA62

    // Gain = 16
    P1OUT &= ~(BIT3 + BIT4);
    P1OUT &= ~(BIT5 + BIT6);
    P3OUT &= ~(BIT0 + BIT1);

    SCALE_FACTOR_660 = 1;
    SCALE_FACTOR_800 = 1;
    SCALE_FACTOR_940 = 1;

    return;
}

void init_ADC12(void)
{
    // select A0-A5 inputs
    P6SEL |= BIT0 + BIT1 + BIT2 + BIT3 + BIT4 + BIT5;

    // 8 clocks for sampling, multiple sample and conversions, ADC on
    ADC12CTL0 = ADC12SHT0_1 + ADC12MSC + ADC12ON;

    // sample signal from timer, repeat-single-channel mode
    ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2;

    // external reference
    ADC12MCTL0 = ADC12SREF_2 + ADC12INCH_0; // A0 - DC_660
    ADC12MCTL1 = ADC12SREF_2 + ADC12INCH_1; // A1 - AC_660
    ADC12MCTL2 = ADC12SREF_2 + ADC12INCH_2; // A2 - DC_800
    ADC12MCTL3 = ADC12SREF_2 + ADC12INCH_3; // A3 - AC_800
    ADC12MCTL4 = ADC12SREF_2 + ADC12INCH_4; // A4 - DC_940
    ADC12MCTL5 = ADC12SREF_2 + ADC12INCH_5; // A5 - AC_940

    return;
}

void init_DMA0(void)
{
    // choose ADC as source
    DMACTL0 = DMA0TSEL_24;

    // start dma after current CPU instruction
    DMACTL4 = DMARMWDIS;

    // reset interrupt flag
    DMA0CTL &= ~DMAIFG;

    // repeated single transfer, dst incr, enable dma, enable interrupts
    DMA0CTL = DMADT_4 + DMADSTINCR_3 + DMAEN + DMAIE;

    return;
}

void init_display(void)
{
    P7DIR = ~0;
    P7OUT = 0;


    // ACLK
    TA2CTL = TASSEL_1;

    // interrupts when count to TA2CCR1, TA2CCR2
    TA2CCTL1 = SCCI + CCIE;
    TA2CCTL2 = SCCI + CCIE;

    // set TA2CCR1, TA2CCR2 values for 1 kHz
    TA2CCR1 = 25000;
    TA2CCR2 = 50000;

    return;
}
