/**
 * Piscar a sequência de VM-AZ-VD dos leds
 *
 * PF1 - LED VERMELHO
 * PF2 - LED AZUL
 * PF3 - LED VERDE
 *
 */

#include <inttypes.h>
#include <tm4c123gh6pm.h>

int main(void)
{
    //configurações da GPIO
    uint32_t i;
    SYSCTL_RCGCGPIO_R = 0x20;               //habilitando clock. Valor 0x20 é referente ao PORTF
    GPIO_PORTF_DIR_R  = 0b00001110;         //habilitando pinos PF1, PF2 e PF3 como saída
    GPIO_PORTF_PUR_R  = 0x10;               //******
    GPIO_PORTF_DEN_R  = 0b00001110;         //habilitando a função digital dos pinos PF1, PF2 e PF3.

    //on/off dos led's usando um while e delay com for
    while(1)
    {
        for (i=0; i<1400000; i++){}
            GPIO_PORTF_DATA_R = 0b00000010;
        for (i=0; i<1400000; i++){}
            GPIO_PORTF_DATA_R = 0b00000100;
        for (i=0; i<1400000; i++){}
            GPIO_PORTF_DATA_R = 0b00001000;
    }
    return 0;
}
