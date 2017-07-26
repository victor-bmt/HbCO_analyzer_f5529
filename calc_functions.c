#include <msp430.h>
#include "calc_functions.h"
#include "init_functions.h"

unsigned int get_average(unsigned int* buf, unsigned int length)
{
    unsigned long sum = 0;
    unsigned int i = 0;

    for (i = 0; i < length; i++)
        sum += buf[i];

    // the less ADC result, the more is DC
    return (unsigned int)(DIVIDER * sum / length);
}

void get_DCs(unsigned int* pDC_660,
             unsigned int* pDC_800,
             unsigned int* pDC_940)
{
    unsigned int buf_1[BUF_LENGTH] = { 0 };
    unsigned int buf_2[BUF_LENGTH] = { 0 };

    // reset 15-12
    ADC12CTL1 &= ~(15 << 12);

    // set A0 - DC_660
    ADC12CTL1 |= ADC12CSTARTADD_0;
    // enable conversion
    ADC12CTL0 |= ADC12ENC;

    // define src and dst for DMA0
    __data16_write_addr((unsigned short)&DMA0SA,
                        (unsigned long)&ADC12MEM0);
    __data16_write_addr((unsigned short)&DMA0DA,
                        (unsigned long)buf_1);

    // define DMA size
    DMA0SZ = BUF_LENGTH;

    // start conversion
    ADC12CTL0 |= ADC12SC;

    // enter LPM3
    __bis_SR_register(LPM3_bits + GIE);

    // disable conversion to change registers
    ADC12CTL0 &= ~ADC12ENC;

    // reset 15-12
    ADC12CTL1 &= ~(15 << 12);

    // set A2 - DC_800
    ADC12CTL1 |= ADC12CSTARTADD_2;

    // enable conversion
    ADC12CTL0 |= ADC12ENC;

    // define src and dst for DMA0
    __data16_write_addr((unsigned short)&DMA0SA,
                        (unsigned long)&ADC12MEM2);
    __data16_write_addr((unsigned short)&DMA0DA,
                        (unsigned long)buf_2);

    // define DMA size
    DMA0SZ = BUF_LENGTH;

    // start conversion
    ADC12CTL0 |= ADC12SC;

    // get DC_660
    *pDC_660 = 0xFFFF - get_average(buf_1, BUF_LENGTH);

    // enter LPM3
    __bis_SR_register(LPM3_bits + GIE);

    // disable conversion to change registers
    ADC12CTL0 &= ~ADC12ENC;


    // reset 15-12
    ADC12CTL1 &= ~(15 << 12);

    // set A4 - DC_940
    ADC12CTL1 |= ADC12CSTARTADD_4;

    // enable conversion
    ADC12CTL0 |= ADC12ENC;

    // define src and dst for DMA0
    __data16_write_addr((unsigned short)&DMA0SA,
                        (unsigned long)&ADC12MEM4);
    __data16_write_addr((unsigned short)&DMA0DA,
                        (unsigned long)buf_1);

    // define DMA size
    DMA0SZ = BUF_LENGTH;

    // start conversion
    ADC12CTL0 |= ADC12SC;

    // get DC_800
    *pDC_800 = 0xFFFF - get_average(buf_2, BUF_LENGTH);

    // enter LPM3
    __bis_SR_register(LPM3_bits + GIE);

    // get DC_940
    *pDC_940 = 0xFFFF - get_average(buf_1, BUF_LENGTH);

    return;
}

void get_ACs(unsigned int* pAC_660,
             unsigned int* pAC_800,
             unsigned int* pAC_940)
{
    unsigned int buf_1[BUF_LENGTH];
    unsigned int buf_2[BUF_LENGTH];
    unsigned int buf_3[BUF_LENGTH];

    unsigned int AC_current = 0;

    unsigned int AC_660_min = ~0;
    unsigned int AC_660_max = 0;
    unsigned int AC_800_min = ~0;
    unsigned int AC_800_max = 0;
    unsigned int AC_940_min = ~0;
    unsigned int AC_940_max = 0;

    // reset 15-12
    ADC12CTL1 &= ~(15 << 12);

    // set A1 - AC_660
    ADC12CTL1 |= ADC12CSTARTADD_1;

    // enable conversion
    ADC12CTL0 |= ADC12ENC;

    // define src and dst for DMA0
    __data16_write_addr((unsigned short)&DMA0SA,
                        (unsigned long)&ADC12MEM1);
    __data16_write_addr((unsigned short)&DMA0DA,
                        (unsigned long)buf_1);

    // define DMA size
    DMA0SZ = BUF_LENGTH;

    // start conversion
    ADC12CTL0 |= ADC12SC;

    // enter LPM3
    __bis_SR_register(LPM3_bits + GIE);

    unsigned int i = 0;
    while (i++ < NUMBER_OF_AC_POINTS)
    {
        // disable conversion to change registers
        ADC12CTL0 &= ~ADC12ENC;

        // reset 15-12
        ADC12CTL1 &= ~(15 << 12);

        // set A3 - AC_800
        ADC12CTL1 |= ADC12CSTARTADD_3;

        // enable conversion
        ADC12CTL0 |= ADC12ENC;

        // define src and dst for DMA0
        __data16_write_addr((unsigned short)&DMA0SA,
                            (unsigned long)&ADC12MEM3);
        __data16_write_addr((unsigned short)&DMA0DA,
                            (unsigned long)buf_2);

        // define DMA size
        DMA0SZ = BUF_LENGTH;

        // start conversion
        ADC12CTL0 |= ADC12SC;

        // get AC_660
        AC_current = get_average(buf_1, BUF_LENGTH);
        if (AC_current < AC_660_min)
            AC_660_min = AC_current;
        if (AC_current > AC_660_max)
            AC_660_max = AC_current;

        // enter LPM3
        __bis_SR_register(LPM3_bits + GIE);

        // disable conversion to change registers
        ADC12CTL0 &= ~ADC12ENC;

        // reset 15-12
        ADC12CTL1 &= ~(15 << 12);

        // set A5 - AC_940
        ADC12CTL1 |= ADC12CSTARTADD_5;

        // enable conversion
        ADC12CTL0 |= ADC12ENC;

        // define src and dst for DMA0
        __data16_write_addr((unsigned short)&DMA0SA,
                            (unsigned long)&ADC12MEM5);
        __data16_write_addr((unsigned short)&DMA0DA,
                            (unsigned long)buf_3);

        // define DMA size
        DMA0SZ = BUF_LENGTH;

        // start conversion
        ADC12CTL0 |= ADC12SC;

        // get AC_800
        AC_current = get_average(buf_2, BUF_LENGTH);
        if (AC_current < AC_800_min)
            AC_800_min = AC_current;
        if (AC_current > AC_800_max)
            AC_800_max = AC_current;

        // enter LPM3
        __bis_SR_register(LPM3_bits + GIE);

        // disable conversion to change registers
        ADC12CTL0 &= ~ADC12ENC;

        // reset 15-12
        ADC12CTL1 &= ~(15 << 12);

        // set A1 - AC_660
        ADC12CTL1 |= ADC12CSTARTADD_1;

        // enable conversion
        ADC12CTL0 |= ADC12ENC;

        // define src and dst for DMA0
        __data16_write_addr((unsigned short)&DMA0SA,
                            (unsigned long)&ADC12MEM1);
        __data16_write_addr((unsigned short)&DMA0DA,
                            (unsigned long)buf_1);

        // define DMA size
        DMA0SZ = BUF_LENGTH;

        // start conversion
        ADC12CTL0 |= ADC12SC;

        // get AC_940
        AC_current = get_average(buf_3, BUF_LENGTH);
        if (AC_current < AC_940_min)
            AC_940_min = AC_current;
        if (AC_current > AC_940_max)
            AC_940_max = AC_current;

        // enter LPM3
        __bis_SR_register(LPM3_bits + GIE);
    }

    *pAC_660 = AC_660_max - AC_660_min;
    *pAC_800 = AC_800_max - AC_800_min;
    *pAC_940 = AC_940_max - AC_940_min;

    return;
}

unsigned int calc_SpCO(unsigned int DC_660,
                       unsigned int AC_660,
                       unsigned int DC_800,
                       unsigned int AC_800,
                       unsigned int DC_940,
                       unsigned int AC_940)
{
    double R1 = (AC_800/(SCALE_FACTOR_800 * DC_800)) / (AC_940 / (SCALE_FACTOR_940 * DC_940));
    double R2 = (AC_660/(SCALE_FACTOR_660 * DC_660)) / (AC_940 / (SCALE_FACTOR_940 * DC_940));

    double SpCO_calc = 100 * ((A1 * R2 + B1 * R1 + C1) / (A2 * R2 + B2 * R1 + C2));

    return (unsigned int)SpCO_calc;
}

void adjust_to_ADC(unsigned int DC_660,
                   unsigned int AC_660,
                   unsigned int DC_800,
                   unsigned int AC_800,
                   unsigned int DC_940,
                   unsigned int AC_940)
{
    // probe disconnect
    if ((DAC_660 == 170) && (DC_660 > 2047))
    {
        SpCO = -1;
        return;
    }

    // low signal
    if (    ((AC_660 < THRESH_AC_LOW) && (DC_660 > THRESH_DC_HIGH)) ||
            ((AC_800 < THRESH_AC_LOW) && (DC_800 > THRESH_DC_HIGH)) ||
            ((AC_940 < THRESH_AC_LOW) && (DC_940 > THRESH_DC_HIGH))   )
    {
        SpCO = -2;
        return;
    }

    // low AC_660 signal
    if (AC_660 < THRESH_AC_LOW)
    {
        if (DAC_660 < DAC_THRESH_HIGH)
            DAC_660 += 10;
        else switch ((P1OUT & (3 << 3)) >> 3)
        {
        // set Gain = 128
        case  0:    P1OUT |= BIT3;
                    P1OUT &= ~BIT4;
                    DAC_660 = INIT_DAC;
                    SCALE_FACTOR_660 = 1;
                    break;

        // set Gain = 256
        case  1:    P1OUT &= ~BIT3;
                    P1OUT |= BIT4;
                    DAC_660 = INIT_DAC;
                    SCALE_FACTOR_660 = 4;
                    break;

        // low signal
        case  2:    SpCO = -2;
                    return;

        // some error - set Gain = 16
        default:    P1OUT &= ~(BIT3 + BIT4);
                    SCALE_FACTOR_660 = 1;
                    SpCO = -2;
                    return;
        }

        P3OUT &= ~BIT4;  // SYNC - enable DAC RX
        UCA0TXBUF = (DAC_660 & 0xFF00) >> 8;
        while ( !(UCA0IFG & UCTXIFG) );
        UCA0TXBUF = DAC_660 & 0xFF;
        while ( !(UCA0IFG & UCTXIFG) );
        P3OUT |= BIT4;  // SYNC - disable DAC RX
    }

    // high DC_660 signal
    if (DC_660 > THRESH_DC_HIGH)
    {
        if (DAC_660 > INIT_DAC)
            DAC_660 -= 10;
        else switch ((P1OUT & (3 << 3)) >> 3)
        {
        // high signal
        case  0:    SpCO = -2;
                    return;

        // set Gain = 16
        case  1:    P1OUT &= ~(BIT3 + BIT4);
                    DAC_660 = DAC_THRESH_HIGH;
                    SCALE_FACTOR_660 = 1;
                    break;

        // set Gain = 128
        case  2:    P1OUT |= BIT3;
                    P1OUT &= ~BIT4;
                    DAC_660 = DAC_THRESH_HIGH;
                    SCALE_FACTOR_660 = 1;
                    break;

        // some error - set Gain = 16
        default:    P1OUT &= ~(BIT3 + BIT4);
                    SCALE_FACTOR_660 = 1;
                    SpCO = -2;
                    return;
        }

        P3OUT &= ~BIT4;  // SYNC - enable DAC RX
        UCA0TXBUF = (DAC_660 & 0xFF00) >> 8;
        while ( !(UCA0IFG & UCTXIFG) );
        UCA0TXBUF = DAC_660 & 0xFF;
        while ( !(UCA0IFG & UCTXIFG) );
        P3OUT |= BIT4;  // SYNC - disable DAC RX
    }

    // low AC_800 signal
    if (AC_800 < THRESH_AC_LOW)
    {
        if (DAC_800 < DAC_THRESH_HIGH)
            DAC_800 += 10;
        else switch ((P1OUT & (3 << 5)) >> 5)
        {
        // set Gain = 128
        case  0:    P1OUT |= BIT5;
                    P1OUT &= ~BIT6;
                    DAC_800 = INIT_DAC;
                    SCALE_FACTOR_800 = 1;
                    break;

        // set Gain = 256
        case  1:    P1OUT &= ~BIT5;
                    P1OUT |= BIT6;
                    DAC_800 = INIT_DAC;
                    SCALE_FACTOR_800 = 4;
                    break;

        // low signal
        case  2:    SpCO = -2;
                    return;

        // some error - set Gain = 16
        default:    P1OUT &= ~(BIT5 + BIT6);
                    SCALE_FACTOR_800 = 1;
                    SpCO = -2;
                    return;
        }

        P3OUT &= ~BIT4;  // SYNC - enable DAC RX
        UCA0TXBUF = (DAC_800 & 0xFF00) >> 8;
        while ( !(UCA0IFG & UCTXIFG) );
        UCA0TXBUF = DAC_800 & 0xFF;
        while ( !(UCA0IFG & UCTXIFG) );
        P3OUT |= BIT4;  // SYNC - disable DAC RX
    }

    // high DC_800 signal
    if (DC_800 > THRESH_DC_HIGH)
    {
        if (DAC_800 > INIT_DAC)
            DAC_800 -= 10;
        else switch ((P1OUT & (3 << 5)) >> 5)
        {
        // high signal
        case  0:    SpCO = -2;
                    return;

        // set Gain = 16
        case  1:    P1OUT &= ~(BIT3 + BIT4);
                    DAC_800 = DAC_THRESH_HIGH;
                    SCALE_FACTOR_800 = 1;
                    break;

        // set Gain = 128
        case  2:    P1OUT |= BIT3;
                    P1OUT &= ~BIT4;
                    DAC_800 = DAC_THRESH_HIGH;
                    SCALE_FACTOR_800 = 1;
                    break;

        // some error - set Gain = 16
        default:    P1OUT &= ~(BIT5 + BIT6);
                    SCALE_FACTOR_800 = 1;
                    SpCO = -2;
                    return;
        }

        P3OUT &= ~BIT4;  // SYNC - enable DAC RX
        UCA0TXBUF = (DAC_800 & 0xFF00) >> 8;
        while ( !(UCA0IFG & UCTXIFG) );
        UCA0TXBUF = DAC_800 & 0xFF;
        while ( !(UCA0IFG & UCTXIFG) );
        P3OUT |= BIT4;  // SYNC - disable DAC RX
    }

    // low AC_940 signal
    if (AC_940 < THRESH_AC_LOW)
    {
        if (DAC_940 < DAC_THRESH_HIGH)
            DAC_940 += 10;
        else switch ((P3OUT & (3 << 0)) >> 0)
        {
        // set Gain = 128
        case  0:    P3OUT |= BIT0;
                    P3OUT &= ~BIT1;
                    DAC_940 = INIT_DAC;
                    SCALE_FACTOR_940 = 1;
                    break;

        // set Gain = 256
        case  1:    P3OUT &= ~BIT0;
                    P3OUT |= BIT1;
                    DAC_940 = INIT_DAC;
                    SCALE_FACTOR_940 = 4;
                    break;

        // low signal
        case  2:    SpCO = -2;
                    return;

        // some error - set Gain = 16
        default:    P2OUT &= ~(BIT0 + BIT1);
                    SCALE_FACTOR_940 = 1;
                    SpCO = -2;
                    return;
        }

        P3OUT &= ~BIT4;  // SYNC - enable DAC RX
        UCA0TXBUF = (DAC_940 & 0xFF00) >> 8;
        while ( !(UCA0IFG & UCTXIFG) );
        UCA0TXBUF = DAC_940 & 0xFF;
        while ( !(UCA0IFG & UCTXIFG) );
        P3OUT |= BIT4;  // SYNC - disable DAC RX
    }

    // high DC_940 signal
    if (DC_940 > THRESH_DC_HIGH)
    {
        if (DAC_940 > INIT_DAC)
            DAC_940 -= 10;
        else switch ((P3OUT & (3 << 0)) >> 0)
        {
        // high signal
        case  0:    SpCO = -2;
                    return;

        // set Gain = 16
        case  1:    P3OUT &= ~(BIT0 + BIT1);
                    DAC_940 = DAC_THRESH_HIGH;
                    SCALE_FACTOR_940 = 1;
                    break;

        // set Gain = 128
        case  2:    P3OUT |= BIT0;
                    P3OUT &= ~BIT1;
                    DAC_940 = DAC_THRESH_HIGH;
                    SCALE_FACTOR_940 = 1;
                    break;

        // some error - set Gain = 16
        default:    P3OUT &= ~(BIT0 + BIT1);
                    SCALE_FACTOR_940 = 1;
                    SpCO = -2;
                    return;
        }

        P3OUT &= ~BIT4;  // SYNC - enable DAC RX
        UCA0TXBUF = (DAC_940 & 0xFF00) >> 8;
        while ( !(UCA0IFG & UCTXIFG) );
        UCA0TXBUF = DAC_940 & 0xFF;
        while ( !(UCA0IFG & UCTXIFG) );
        P3OUT |= BIT4;  // SYNC - disable DAC RX
    }

    return;
}
