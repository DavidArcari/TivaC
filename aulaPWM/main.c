#include <inttypes.h>
#include <tm4c123gh6pm.h>

/*
Lembrando que o elemento 0 do modulo M0PWM0 é o pino PB6, por isso se deve inicializar este pino em especifico para o funcionamento do elemento PWM
pagina 1230 - PWM datasheet
vamos usar o PB6
*/

delay(){
    uint32_t i;
    for(i=0; i<20000; i++){}
}
int main(void) {

    /*
    //Ligar o PortB
    SYSCTL_RCGCGPIO_R = 0x02;
    //Habilitar função alternativa do PB6 - pg 671
    GPIO_PORTB_AFSEL_R = 0x40;
    //Definir que a função alternativa do PB6 é PWM - pg 688 / tabela pg 1351
    GPIO_PORTB_PCTL_R = 0x04000000;
    //Habilita PB6 digitalmente
    GPIO_PORTB_DEN_R = 0x40;

    //Ativa o modulo de PWM 0
    SYSCTL_RCGCPWM_R = 0x01;
    //Nivel baixo em LOAD / Nivel alto em CMPA - pg 1282
    PWM0_0_GENA_R = 0xC8;
    //Valor de frequencia do PWM, em ciclos de clock
    PWM0_0_LOAD_R = 16000;                                  //qtde de ciclos de clock 16000 = 1K
    //Valor de porcentagem do PWM, em ciclos de clock
    PWM0_0_CMPA_R = (16000 * 50)/100;                       //(Freq * porcentagem)/100
    //Ativa o controle do gerador - pg 1265
    PWM0_0_CTL_R = 0x01;
    //Inicializa o PWM0 - Saida PWM - pg 1247
    PWM0_ENABLE_R = 0x01;
    */


    //TESTANDO COM O LED DA PLACA PF2 PWM6M1

    uint16_t freq = 16000, porcentagem;
    //Ligar o PortF
    SYSCTL_RCGCGPIO_R = 0x20;
    //Habilitar função alternativa do PF2 - pg 671
    GPIO_PORTF_AFSEL_R = 0x04;
    //Definir que a função alternativa do PF2 é PWM - pg 688 / tabela pg 1351
    GPIO_PORTF_PCTL_R = 0x00000500;
    //Habilita PF2 digitalmente
    GPIO_PORTF_DEN_R = 0x04;

    //Ativa o modulo de PWM 0
    SYSCTL_RCGCPWM_R = 0x02;
    //delay();
    //Nivel baixo em LOAD / Nivel alto em CMPA - pg 1282
    PWM1_3_GENA_R = 0xC8;
    //Valor de frequencia do PWM, em ciclos de clock
    PWM1_3_LOAD_R = freq;                           //qtde de ciclos de clock 16000 = 1K

    //Ativa o controle do gerador - pg 1265
    PWM1_3_CTL_R = 0x01;
    //dalay();
    //Inicializa o PWM0 - Saida PWM - pg 1247
    PWM1_ENABLE_R = 0x40;

    while(1) {
        uint8_t i;
        for(i=0; i<100; i++){
            porcentagem = i;
            //Valor de porcentagem do PWM, em ciclos de clock
            PWM1_3_CMPA_R = (freq * porcentagem)/100;         //(Freq * porcentagem)/100
            delay();
        }
        __asm("WFI");

    }
}
