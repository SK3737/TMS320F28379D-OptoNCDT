#include "driverlib.h"
#include "device.h"
#include <stdio.h>
#include <string.h>

char msg[64];

void main(void)
{
    uint16_t adcValue;
    uint32_t voltage_mV, distance_mm;

    Device_init();
    Device_initGPIO();

    // ==== SCIA RX ====
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCIRXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCIRXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCIRXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_QUAL_ASYNC);

    // ==== SCIA TX ====
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCITXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCITXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCITXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_QUAL_ASYNC);

    // ==== SCIA INIT ====
    SCI_performSoftwareReset(SCIA_BASE);
    SCI_setConfig(SCIA_BASE, DEVICE_LSPCLK_FREQ, 9600,
                  (SCI_CONFIG_WLEN_8 | SCI_CONFIG_STOP_ONE | SCI_CONFIG_PAR_NONE));
    SCI_resetChannels(SCIA_BASE);
    SCI_resetRxFIFO(SCIA_BASE);
    SCI_resetTxFIFO(SCIA_BASE);
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_TXFF | SCI_INT_RXFF);
    SCI_enableFIFO(SCIA_BASE);
    SCI_enableModule(SCIA_BASE);
    SCI_performSoftwareReset(SCIA_BASE);

    // ==== ADC SETUP ====
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_4_0);
    ADC_setMode(ADCA_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
    ADC_enableConverter(ADCA_BASE);
    DEVICE_DELAY_US(1000);

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, 15);
    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER0);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER1);

    SCI_writeCharArray(SCIA_BASE, (uint16_t *)"Distance Output (156 Ohm, ILD1220-50, mm)\r\n", 44);

    while(1)
    {
        ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER0);
        while(!ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1));
        ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER1);

        adcValue = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);

        // Convert ADC to millivolts (3.3V reference, 12-bit)
        voltage_mV = ((uint32_t)adcValue * 3300UL) / 4095UL;

        // Convert voltage to millimeters:
        // 0.624V (624mV) = 0mm
        // 3.12V (3120mV) = 50mm
        if (voltage_mV > 624UL)
            distance_mm = ((voltage_mV - 624UL) * 50UL) / (3120UL - 624UL);
        else
            distance_mm = 0;

        snprintf(msg, sizeof(msg), "Distance: %lu mm, ADC: %u\r\n", distance_mm, adcValue);
        SCI_writeCharArray(SCIA_BASE, (uint16_t *)msg, strlen(msg));

        DEVICE_DELAY_US(500000); // 0.5s
    }
}
