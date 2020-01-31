/**
 * Hello World - Aula Pisca Led.
 * Continua incrementando o botão para acionar o led
 * PF2 - LED AZUL
 * PF4 - BOTÃO - ENTRADA
 */

#include <inttypes.h>
#include <tm4c123gh6pm.h>

int main(void)
{
    //configurações da GPIO
    uint32_t i;
    SYSCTL_RCGCGPIO_R = 0x20;           //habilitando clock. Valor 0x20 é referente ao PORTF
    GPIO_PORTF_DIR_R  = 0x06;       	//habilitando pinos PF2 como saida e PF4 como entrada hexa p/ binário 0b00001000
    GPIO_PORTF_PUR_R  = 0x10;           //habilitando o PF4 como pull-up  hexa p/ binário 0b00010000
    GPIO_PORTF_DEN_R  = 0x16;     	    //habilitando a função digital dos pinos PF2 e PF4. hexa p/ binário 0b00010100

    //on/off dos led's usando um while e delay com for
    while(1)
    {
        if ((GPIO_PORTF_DATA_R & 0x10) == 0x00)
            GPIO_PORTF_DATA_R = 0x04;
        else
            GPIO_PORTF_DATA_R = 0x02;
    }
    return 0;
}
