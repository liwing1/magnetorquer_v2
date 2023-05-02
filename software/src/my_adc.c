/*
 * my_adc.c
 *
 *  Created on: 3 de mar de 2023
 *      Author: liwka
 */
#include "driverlib.h"
#include "my_adc.h"
#include "my_uart.h"


//******************************************************************************
//  MSP430FR59xx Demo - ADC12B, Sample A1, AVcc Ref, Set P1.0 if A1 > 0.5*AVcc
//
//   Description: A single sample is made on A1 with reference to AVcc.
//   Software sets ADC12BSC to start sample and conversion - ADC12BSC
//   automatically cleared at EOC. ADC12B internal oscillator times sample (16x)
//   and conversion. In Mainloop MSP430 waits in LPM0 to save power until ADC12B
//   conversion complete, ADC12_B_ISR will force exit from LPM0 in Mainloop on
//   reti. If A0 > 0.5*AVcc, P1.0 set, else reset. The full, correct handling of
//   and ADC12B interrupt is shown as well.
//
//
//                MSP430FR5969
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//        >---|P9.4/A12         |
//        >---|P9.5/A13         |
//        >---|P9.6/A14         |
//
//******************************************************************************


typedef struct{
    uint8_t flag[MAG_IDX_MAX];
    uint32_t raw[MAG_IDX_MAX];
} adc_handler_t;

adc_handler_t adc_handler = {0};

void adc_init(void)
{
    /*
    * Select Port 4
    * Set Pin 4, 5, 6 to output Primary Module Function
    */
    GPIO_setAsPeripheralModuleFunctionOutputPin(
        GPIO_PORT_P9,
        GPIO_PIN4 + GPIO_PIN5 + GPIO_PIN6,
        GPIO_PRIMARY_MODULE_FUNCTION
    );


    //Initialize the ADC12B Module
    /*
    * Base address of ADC12B Module
    * Use internal ADC12B bit as sample/hold signal to start conversion
    * USE MODOSC 5MHZ Digital Oscillator as clock source
    * Use default clock divider/pre-divider of 1
    * Not use internal channel
    */
    ADC12_B_initParam initParam = {
        .sampleHoldSignalSourceSelect = ADC12_B_SAMPLEHOLDSOURCE_SC,
        .clockSourceSelect = ADC12_B_CLOCKSOURCE_ADC12OSC,
        .clockSourceDivider = ADC12_B_CLOCKDIVIDER_1,
        .clockSourcePredivider = ADC12_B_CLOCKPREDIVIDER__1,
        .internalChannelMap = ADC12_B_NOINTCH
    };
    ADC12_B_init(ADC12_B_BASE, &initParam);

    //Enable the ADC12B module
    ADC12_B_enable(ADC12_B_BASE);

    /*
    * Base address of ADC12B Module
    * For memory buffers 0-7 sample/hold for 64 clock cycles
    * For memory buffers 8-15 sample/hold for 4 clock cycles (default)
    * Disable Multiple Sampling
    */
    ADC12_B_setupSamplingTimer(ADC12_B_BASE,
      ADC12_B_CYCLEHOLD_16_CYCLES,
      ADC12_B_CYCLEHOLD_4_CYCLES,
      ADC12_B_MULTIPLESAMPLESDISABLE);

    //Configure Memory Buffer
    /*
    * Base address of the ADC12B Module
    * Configure memory buffer 0
    * Map input A1 to memory buffer 0
    * Vref+ = AVcc
    * Vref- = AVss
    * Memory buffer 0 is not the end of a sequence
    */
    ADC12_B_configureMemoryParam configureMemoryParam = {
        .memoryBufferControlIndex = ADC12_B_MEMORY_0,
        .inputSourceSelect = ADC12_B_INPUT_A12,
        .refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS,
        .endOfSequence = ADC12_B_NOTENDOFSEQUENCE,
        .windowComparatorSelect = ADC12_B_WINDOW_COMPARATOR_DISABLE,
        .differentialModeSelect = ADC12_B_DIFFERENTIAL_MODE_DISABLE
    };
    uint8_t i = 0;
    for(i = 0; i < 3; i++){
        configureMemoryParam.inputSourceSelect = ADC12_B_INPUT_A12 + i;
        configureMemoryParam.memoryBufferControlIndex = ADC12_B_MEMORY_0 + (4*i);

        ADC12_B_configureMemory(ADC12_B_BASE, &configureMemoryParam);

        ADC12_B_clearInterrupt(ADC12_B_BASE,
            0,
            ADC12_B_IFG0 << 2*i
            );

        //Enable memory buffer 0 interrupt
        ADC12_B_enableInterrupt(ADC12_B_BASE,
          ADC12_B_IE0 << 2*i,
          0,
          0);
    }
}

uint8_t adc_read_mag(mag_idx_t mag_idx){
    ADC12_B_startConversion(
            ADC12_B_BASE,
            2 * mag_idx,
            ADC12_B_SINGLECHANNEL
            );

//    while(!adc_handler.flag[mag_idx]);
//    adc_handler.flag[mag_idx] = 0;

    return adc_handler.raw[mag_idx];
}

void adc_manager(void){
    uint32_t adc_mean = 0;
    uint8_t mag_i = 0;
    uint8_t mean_i = 0;

    for(mag_i = 0; mag_i < MAG_IDX_MAX; mag_i++){
        adc_mean = 0;

        for(mean_i = 0; mean_i < 10; mean_i++){
            adc_read_mag((mag_idx_t)mag_i);
            __delay_cycles(1000);

            if(adc_handler.flag[mag_i]){
                adc_handler.flag[mag_i] = 0;

                adc_mean += adc_handler.raw[mag_i];
                adc_handler.raw[mag_i] = 0;
            }
        }
        adc_mean /= 10;
        uart_tx("ADC[%d]: %d mV\r\n", mag_i, (adc_mean * 3300)/4095);
    }
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(ADC12_VECTOR)))
#endif
void ADC12_ISR(void)
{
  switch(__even_in_range(ADC12IV,12))
  {
    case ADC12IV_NONE: break;                               // Vector  0:  No interrupt
    case ADC12IV_ADC12OVIFG: break;                         // Vector  2:  ADC12BMEMx Overflow
    case ADC12IV_ADC12TOVIFG: break;                        // Vector  4:  Conversion time overflow
    case ADC12IV_ADC12HIIFG: break;                         // Vector  6:  ADC12BHI
    case ADC12IV_ADC12LOIFG: break;                         // Vector  8:  ADC12BLO
    case ADC12IV_ADC12INIFG: break;                         // Vector 10:  ADC12BIN
    case ADC12IV_ADC12IFG0:                                 // Vector 12:  ADC12BMEM0 Interrupt
        adc_handler.raw[0] = ADC12_B_getResults(ADC12_B_BASE, ADC12_B_MEMORY_0);
        if (adc_handler.raw[0] >= 0x4db)
        {
          adc_handler.flag[0] = 1;
        }

        break;
    case ADC12IV_ADC12IFG1: break;                         // Vector 14:  ADC12BMEM1
    case ADC12IV_ADC12IFG2:                                // Vector 16:  ADC12BMEM2
        adc_handler.raw[1] = ADC12_B_getResults(ADC12_B_BASE, ADC12_B_MEMORY_2);
        if (adc_handler.raw[1] >= 0x4db)
        {
          adc_handler.flag[1] = 1;
        }

        break;
    case ADC12IV_ADC12IFG3: break;                         // Vector 18:  ADC12BMEM3
    case ADC12IV_ADC12IFG4:                                // Vector 20:  ADC12BMEM4
        adc_handler.raw[2] = ADC12_B_getResults(ADC12_B_BASE, ADC12_B_MEMORY_4);
        if (adc_handler.raw[2] >= 0x4db)
        {
          adc_handler.flag[2] = 1;
        }
        break;
    case ADC12IV_ADC12IFG5: break;                         // Vector 22:  ADC12BMEM5
    case ADC12IV_ADC12IFG6: break;                         // Vector 24:  ADC12BMEM6
    case ADC12IV_ADC12IFG7: break;                         // Vector 26:  ADC12BMEM7
    case ADC12IV_ADC12IFG8: break;                         // Vector 28:  ADC12BMEM8
    case ADC12IV_ADC12IFG9: break;                         // Vector 30:  ADC12BMEM9
    case ADC12IV_ADC12IFG10: break;                         // Vector 32:  ADC12BMEM10
    case ADC12IV_ADC12IFG11: break;                         // Vector 34:  ADC12BMEM11
    case ADC12IV_ADC12IFG12: break;                         // Vector 36:  ADC12BMEM12
    case ADC12IV_ADC12IFG13: break;                         // Vector 38:  ADC12BMEM13
    case ADC12IV_ADC12IFG14: break;                         // Vector 40:  ADC12BMEM14
    case ADC12IV_ADC12IFG15: break;                         // Vector 42:  ADC12BMEM15
    case ADC12IV_ADC12IFG16: break;                         // Vector 44:  ADC12BMEM16
    case ADC12IV_ADC12IFG17: break;                         // Vector 46:  ADC12BMEM17
    case ADC12IV_ADC12IFG18: break;                         // Vector 48:  ADC12BMEM18
    case ADC12IV_ADC12IFG19: break;                         // Vector 50:  ADC12BMEM19
    case ADC12IV_ADC12IFG20: break;                         // Vector 52:  ADC12BMEM20
    case ADC12IV_ADC12IFG21: break;                         // Vector 54:  ADC12BMEM21
    case ADC12IV_ADC12IFG22: break;                         // Vector 56:  ADC12BMEM22
    case ADC12IV_ADC12IFG23: break;                         // Vector 58:  ADC12BMEM23
    case ADC12IV_ADC12IFG24: break;                         // Vector 60:  ADC12BMEM24
    case ADC12IV_ADC12IFG25: break;                         // Vector 62:  ADC12BMEM25
    case ADC12IV_ADC12IFG26: break;                         // Vector 64:  ADC12BMEM26
    case ADC12IV_ADC12IFG27: break;                         // Vector 66:  ADC12BMEM27
    case ADC12IV_ADC12IFG28: break;                         // Vector 68:  ADC12BMEM28
    case ADC12IV_ADC12IFG29: break;                         // Vector 70:  ADC12BMEM29
    case ADC12IV_ADC12IFG30: break;                         // Vector 72:  ADC12BMEM30
    case ADC12IV_ADC12IFG31: break;                         // Vector 74:  ADC12BMEM31
    case ADC12IV_ADC12RDYIFG: break;                        // Vector 76:  ADC12BRDY
    default: break;
  }
}
