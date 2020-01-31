/**
 * Sequencia de led é invertida quando aperta o botão
 *
 * PF1 - LED VERMELHO
 * PF2 - LED AZUL
 * PF3 - LED VERDE
 * PF4 - BOTÃO - ENTRADA
 */

#include <inttypes.h>
#include <tm4c123gh6pm.h>

int main(void)
{
    //configurações da GPIO
    uint32_t i;
    SYSCTL_RCGCGPIO_R = 0x20;           //habilitando clock. Valor 0x20 é referente ao PORTF
    GPIO_PORTF_DIR_R  = 0b00001110;     //habilitando pinos PF1, PF2 e PF3 como saida e PF4 como entrada hexa p/ binário 0b00001110
    GPIO_PORTF_PUR_R  = 0x10;           //habilitando o PF4 como pull-up  hexa p/ binário 0b00010000
    GPIO_PORTF_DEN_R  = 0b00011110;     //habilitando a função digital dos pinos PF2 e PF4. hexa p/ binário 0b00011110

    //on/off dos led's usando um while e delay com for
    while(1)
    {
        for (i=0; i<1400000; i++){}
            GPIO_PORTF_DATA_R = 0b0000010;
        for (i=0; i<1400000; i++){}
            GPIO_PORTF_DATA_R = 0b0000100;
        for (i=0; i<1400000; i++){}
            GPIO_PORTF_DATA_R = 0b0001000;

        if ((GPIO_PORTF_DATA_R & 0x10) == 0x00)
        {
            for (i=0; i<140000; i++){}
                GPIO_PORTF_DATA_R = 0b00000010;     //VERMELHO
            for (i=0; i<140000; i++){}
                GPIO_PORTF_DATA_R = 0b00000100;     //AZUL
            for (i=0; i<140000; i++){}
                GPIO_PORTF_DATA_R = 0b00001000;     //VERDE
            for (i=0; i<140000; i++){}
                GPIO_PORTF_DATA_R = 0b00000110;     //ROXO
            for (i=0; i<140000; i++){}
                GPIO_PORTF_DATA_R = 0b00001100;     //AMARELO
            for (i=0; i<140000; i++){}
                GPIO_PORTF_DATA_R = 0b00001010;     //LARANJA
            for (i=0; i<140000; i++){}
                GPIO_PORTF_DATA_R = 0b00001110;     //BRANCO
            for (i=0; i<140000; i++){}
                GPIO_PORTF_DATA_R = 0b00000000;     //PRETO - DESLIGADO
        }
    }
    return 0;
}
