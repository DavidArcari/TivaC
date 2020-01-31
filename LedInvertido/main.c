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

uint8_t flag = 0;

void seq1(){
    GPIO_PORTF_DATA_R = 0x08;       //0b0001000
    delay();
    GPIO_PORTF_DATA_R = 0x04;       //0b0000100
    delay();
    GPIO_PORTF_DATA_R = 0x02;       //0b0000010
    delay();
}

void seq2(){
    GPIO_PORTF_DATA_R = 0x02;       //0b0000010
    delay();
    GPIO_PORTF_DATA_R = 0x04;       //0b0000100
    delay();
    GPIO_PORTF_DATA_R = 0x08;       //0b0001000
    delay();
}

void delay(){

   uint32_t i;
   for (i=0; i<1400000; i++){}
}

void trataPortf(){
    if ((GPIO_PORTF_RIS_R & 0x10) == 0x10)
    {
        delay();
        flag ^= 1;                            //xor inverte o bit da flag
        GPIO_PORTF_ICR_R    = 0x10;           //habilitado o registrador que limpa a flag da interrupção. Usar o mesmo bit do IM
    }
}

int main(void)
{
    //configurações das GPIOs
    SYSCTL_RCGCGPIO_R   = 0x20;           //habilitando clock. Valor 0x20 é referente ao PORTF
    GPIO_PORTF_DIR_R    = 0x0E;           //habilitando pinos PF1, PF2 e PF3 como saida e PF4 como entrada hexa p/ binário 0b00001110
    GPIO_PORTF_PUR_R    = 0x10;           //habilitando o PF4 como pull-up  hexa p/ binário 0b00010000

    GPIO_PORTF_IS_R     = 0x00;           //habilitando como borda a sensibilidade não como nível.
    GPIO_PORTF_IEV_R    = 0x00;           //habilitando borda de descida com nível logico no bit 4.
    GPIO_PORTF_IBE_R    = 0x00;           //desabilitando o uso de borda de subida e descida ao mesmo tempo
    GPIO_PORTF_IM_R     = 0x10;           //habilitando interrupçãp no bit 4

    GPIO_PORTF_DEN_R    = 0x1E;           //habilitando a função digital dos pinos PF2 e PF4. hexa p/ binário 0b00011110

    NVIC_EN0_R          = 1 << 30;        //habilita as interrupções através da NVIC bit 30

    while(1)
    {
        if (flag == 0)
        {
            seq1();
        }
        else
        {
            seq2();
        }
        return 0;
    }
}
