#include <msp430.h>
#include "init_functions.h"
#include "calc_functions.h"

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) TIMER0_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{

    const unsigned int period = 2500;

    TB0CCR0 += period;

    P2OUT |= BIT0 + BIT1;
    TA0CTL |= TACLR;
    TA0CTL |= MC_1;
    TA1CTL |= TACLR;
    TA1CTL |= MC_1;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{

  const unsigned int period = 2500;

  switch(__even_in_range(TB0IV,14))
  {
    case  0: break;                          // No interrupt

    // CCR1 reached, switch off LED1
    case  2: P1OUT &= ~BIT0;
             TB0CCR1 += period;
             break;

    // CCR2 reached, switch on LED2
    case  4: P1OUT |= BIT1;
             P1OUT &= ~BIT2;
             P1OUT |= BIT0;
             TB0CCR2 += period;
             break;

    // CCR3 reached, switch off LED2
    case  6: P1OUT &= ~BIT0;
             TB0CCR3 += period;
             break;

    // CCR4 reached, switch on LED3
    case  8: P1OUT &= ~BIT1;
             P1OUT |= BIT2;
             P1OUT |= BIT0;
             TB0CCR4 += period;
             break;

    // CCR5 reached, switch off LED3
    case 10: P1OUT &= ~BIT0;
             TB0CCR5 += period;
             break;

    // CCR6 reached, switch on LED1
    case 12: P1OUT &= ~(BIT1 + BIT2);
             P1OUT |= BIT0;
             TB0CCR6 += period;
             break;

    // TAIFG not used
    case 14: break;
    default: break;
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A1_VECTOR))) TIMER0_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{

  switch(__even_in_range(TA0IV,14))
  {
    case  0: break;                          // No interrupt

    // CCR1 reached, switch off IN1_DA31
    case  2: P2OUT &= ~BIT0;
             break;

    // CCR2 reached, switch on IN3_DA31, IN4_DA31
    case  4: P2OUT |= BIT2 + BIT3;
             break;

    // CCR3 reached, switch off IN3_DA31
    case  6: P2OUT &= ~BIT2;
             break;

    // CCR4 reached, switch on IN1_DA32, IN2_DA32
    case  8: P2OUT |= BIT4 + BIT5;
             break;

    // reserved
    case 10: break;

    // reserved
    case 12: break;

    // CCR0 reached, switch off IN1_DA32, stop timer
    case 14: TA0CTL &= ~MC_1;
             P2OUT &= ~BIT4;
             break;

    default: break;
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) TIMER1_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{

  switch(__even_in_range(TA1IV,14))
  {
    case  0: break;                          // No interrupt

    // CCR1 reached, switch off IN2_DA31
    case  2: P2OUT &= ~BIT1;
             break;

    // CCR2 reached, switch off IN4_DA31
    case  4: P2OUT &= ~BIT3;
             break;

    // reserved
    case  6: break;

    // reserved
    case  8: break;

    // reserved
    case 10: break;

    // reserved
    case 12: break;

    // CCR0 reached, switch off IN2_DA32, stop timer
    case 14: TA1CTL &= ~MC_1;
             P2OUT &= ~BIT5;
             break;

    default: break;
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(DMA_VECTOR))) DMA_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(DMAIV,16))
  {
    case 0: break;
    case 2:                                 // DMA0IFG = DMA Channel 0
      // stop ADC, enable CPU
      ADC12CTL0 &= ~ADC12SC;
      __bic_SR_register_on_exit(LPM3_bits);
      break;
    case 4: break;                          // DMA1IFG = DMA Channel 1
    case 6: break;                          // DMA2IFG = DMA Channel 2
    case 8: break;                          // DMA3IFG = DMA Channel 3
    case 10: break;                         // DMA4IFG = DMA Channel 4
    case 12: break;                         // DMA5IFG = DMA Channel 5
    case 14: break;                         // DMA6IFG = DMA Channel 6
    case 16: break;                         // DMA7IFG = DMA Channel 7
    default: break;
  }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER2_A1_VECTOR
__interrupt void TIMER2_A1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER2_A1_VECTOR))) TIMER2_A1_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(TA2IV,14))
  {
    case  0: break;                          // No interrupt

    // CCR1 reached, digit1 - on, digit2 - off
    case  2:    P7OUT &= BIT6;
                P7OUT |= BIT4; // lamp test disable
                P7OUT &= ~BIT7; // dot off
                P7OUT |= 0xF & (unsigned int)(SpCO / 10);
                P7OUT |= BIT5;
                TA2CCR1 += DISPLAY_PERIOD;
                break;

    // CCR2 reached, digit1 - off, digit 2 - on
    case  4:    P7OUT &= ~BIT5;
                P7OUT |= BIT4; // lamp test disable
                P7OUT &= ~BIT7; // dot off
                P7OUT |= 0xF & (SpCO % 10);
                P7OUT |= BIT6;
                TA2CCR2 += DISPLAY_PERIOD;
                break;

    // reserved
    case  6:    break;

    // reserved
    case  8:    break;

    // reserved
    case 10:    break;

    // reserved
    case 12:    break;

    // TAIFG not used
    case 14:    break;

    default:    break;
  }

  if (SpCO == -1)
  {
      P7OUT &= ~BIT7; // dot - off
      P7OUT &= ~BIT4; // all segments on
  }

  if ((SpCO < -2) || (SpCO > 100))
  {
      P7OUT |= BIT4; // lamp test disable
      P7OUT |= BIT0 + BIT1 + BIT2 + BIT3; // make segment blank
      P7OUT |= BIT7; // dot - on
  }
}
