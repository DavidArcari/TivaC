/**
 * Binary Game por David Arcari
 *
 * PORTB E PORTF são entradas com interrupção. PF4 sera o botão de start (numRand1 e 2) com interrupção.
 *
 */

#include <inttypes.h>
#include <tm4c123gh6pm.h>

//variáveis globais
uint8_t numRand1, numRand2, op, resultado, resultadoUs, x1, x2, operador;
uint8_t flag1, flag2, flag3, flag4, flag5, flag6, flag7, flag8, flagSt;

//Função de delay curto
delayCurto(){

    uint32_t x;
    for(x=0;x<160000;x++){}
}

//Função de delay long
delayLongo(){

    uint32_t x;
    for(x=0;x<1600000;x++){}
}

//função que gera numeros aleatórios 1
numeroAle1(){

    uint8_t i;
    for(i=0;i<31;i++){
        x1 = i;
    }
}

//função que gera numeros aleatórios 2
numeroAle2(){

    uint8_t i;
    for(i=31;i>0;i--){
        x2 = i;
    }
}

//função que gera um valor para a variável op para definir uma operação
geraOp(){

    uint8_t i;
    for(i=0;i<4;i++){
        op = i;
    }
}

//função que trata interrupção do portF
trataPortF(){

    numRand1 = x1;
    numRand2 = x2;
    operador = op;
    if((GPIO_PORTF_RIS_R & 0x10)== 0x10){
        delayCurto();
        flagSt ^= 1;
        GPIO_PORTF_ICR_R = 0x10;
    }
}

//função que trata interrupção do portB
trataPortB(){

    if((GPIO_PORTB_RIS_R & 0x01)== 0x01){
        delayCurto();
        flag1 ^= 1;
        GPIO_PORTB_ICR_R = 0x01;
    }
    if((GPIO_PORTB_RIS_R & 0x02)== 0x02){
        delayCurto();
        flag2 ^= 1;
        GPIO_PORTB_ICR_R = 0x02;
    }
    if((GPIO_PORTB_RIS_R & 0x04)== 0x04){
        delayCurto();
        flag3 ^= 1;
        GPIO_PORTB_ICR_R = 0x04;
    }
    if((GPIO_PORTB_RIS_R & 0x08)== 0x08){
        delayCurto();
        flag4 ^= 1;
        GPIO_PORTB_ICR_R = 0x08;
    }
    if((GPIO_PORTB_RIS_R & 0x10)== 0x10){
        delayCurto();
        flag5 ^= 1;
        GPIO_PORTB_ICR_R = 0x10;
    }
    if((GPIO_PORTB_RIS_R & 0x20)== 0x20){
        delayCurto();
        flag6 ^= 1;
        GPIO_PORTB_ICR_R = 0x20;
    }
    if((GPIO_PORTB_RIS_R & 0x40)== 0x40){
        delayCurto();
        flag7 ^= 1;
        GPIO_PORTB_ICR_R = 0x40;
    }
    if((GPIO_PORTB_RIS_R & 0x80)== 0x80){
        delayCurto();
        flag8 ^= 1;
        GPIO_PORTB_ICR_R = 0x80;
    }
}

// flagSt é da interrupção do botão start para gerar os dois numeros aleatorios
//se o valor for 0 dataF é 0 em outro valor tenho que chamar a função que devolve
//os dois números aleatórios.
funcStart(){

    if (flagSt == 0)
    {
        GPIO_PORTF_DATA_R = 0x00;
    }
    else
    {
        GPIO_PORTF_DATA_R = 0x04;
    }
}

//função que trata os botões que o usuário aciona
tecladoResult(){

    if (flag1 == 0)
        GPIO_PORTA_DATA_R = 0x00;
    else
        GPIO_PORTA_DATA_R = 0x04;                                   //PA2

    if (flag2 == 0)
        GPIO_PORTA_DATA_R = 0x00;
    else
        GPIO_PORTA_DATA_R = 0x08;                                   //PA3

    if (flag3 == 0)
        GPIO_PORTA_DATA_R = 0x00;
    else
        GPIO_PORTA_DATA_R = 0x10;                                   //PA4

    if (flag4 == 0)
        GPIO_PORTA_DATA_R = 0x00;
    else
        GPIO_PORTA_DATA_R = 0x20;                                   //PA5

    if (flag5 == 0)
        GPIO_PORTA_DATA_R = 0x00;
    else
        GPIO_PORTA_DATA_R = 0x40;                                   //PA6

    if (flag6 == 0)
        GPIO_PORTA_DATA_R = 0x00;
    else
        GPIO_PORTA_DATA_R = 0x80;                                   //PA7

    if (flag7 == 0)
        GPIO_PORTA_DATA_R = 0x00;
    else
        GPIO_PORTA_DATA_R++;                                        //incrementa portA

    if (flag8 == 0)
        GPIO_PORTA_DATA_R = 0x00;
    else
        GPIO_PORTA_DATA_R--;                                        //decrementa portA
}

//Função que realiza a operação escolhida aleatóriamente
operacao(){

    if(operador == 0){
        resultado = numRand1 + numRand2;
        GPIO_PORTC_DATA_R = 0x00;
    }
    if(operador == 1){
        if (numRand2 != 0){
            resultado = numRand1 - numRand2;
            GPIO_PORTC_DATA_R = 0x80;
        }
        else{
            GPIO_PORTF_DATA_R = 0x02;
        }
    }
    if(operador == 2){
        resultado = numRand1 * numRand2;
        GPIO_PORTC_DATA_R = 0x40;
    }
    if(operador == 3){
        if (numRand2 != 0){
            resultado = numRand1 / numRand2;
            GPIO_PORTC_DATA_R = 0xC0;
        }
        else{
            GPIO_PORTF_DATA_R = 0x02;
        }
    }
}

int main(void)
{
    //configurações da GPIO PORTs
    SYSCTL_RCGCGPIO_R = 0b00000001;                                 //habilitando clock. portA.
    SYSCTL_RCGCGPIO_R|= 0b00000010;                                 //habilitando clock. portB.
    SYSCTL_RCGCGPIO_R|= 0b00000100;                                 //habilitando clock. portC.
    SYSCTL_RCGCGPIO_R|= 0b00001000;                                 //habilitando clock. portD.
    SYSCTL_RCGCGPIO_R|= 0b00010000;                                 //habilitando clock. portE.
    SYSCTL_RCGCGPIO_R|= 0b00100000;                                 //habilitando clock. portF.

    //Configuração do portA (RESULTADO) - 6 pinos SAIDAS
    GPIO_PORTA_DIR_R  = 0xFC;                                       //habilitando pinos 7,6,5,4,3 e 2 saida.
    GPIO_PORTA_DATA_R = 0x00;                                       //habilitando pinos 7,6,5,4,3 e 2 nivel logico baixo.
    GPIO_PORTA_DEN_R  = 0xFC;                                       //habilitando pinos 7,6,5,4,3 e 2 como pinos digitais.

    //Configuração do portB (RESULTADO) - 6 pinos ENTRADAS
    GPIO_PORTB_DIR_R  = 0x00;                                       //habilitando pinos como entrada.
    GPIO_PORTB_PUR_R  = 0xFF;                                       //habilitando pull-up em todos os pinos.
    GPIO_PORTB_DATA_R = 0x00;                                       //habilitando nivel logico baixo em todos os pinos.
    GPIO_PORTB_IS_R   = 0x00;                                       //habilitando como borda a sensibilidade não como nível.
    GPIO_PORTB_IEV_R  = 0x00;                                       //habilitando borda de descida com nível logico.
    GPIO_PORTB_IBE_R  = 0x00;                                       //desabilitando o uso de borda de subida e descida ao mesmo tempo
    GPIO_PORTB_IM_R   = 0xFF;                                       //habilitando interrupçãp em todos os pinos.
    GPIO_PORTB_DEN_R  = 0xFF;                                       //habilitando todos os pinos como digitais.

    //Configuração do portC (OPERAÇÃO) - 2 pinos SAIDAS
    GPIO_PORTC_DIR_R  = 0xC0;                                       //habilitando pinos 7 e 6 saida.
    GPIO_PORTC_DATA_R = 0x00;                                       //habilitando todos os pinos em nivel logico baixo.
    GPIO_PORTC_DEN_R  = 0xC0;                                       //habilitando pinos 7 e 6 como pinos digitais.

    //Configuração do portD (numRand2) - 5 pinos de entrada
    GPIO_PORTD_DIR_R  = 0xCF;                                       //habilitando pinos 0,1,2,3,6 e 7 como saída.
    GPIO_PORTD_DATA_R = 0x00;                                       //habilitando todos os pinos com nível logico baixo.
    GPIO_PORTD_DEN_R  = 0xCF;                                       //habilitando pinos 0,1,2,3,6 e 7 como pinos digitais.

    //Configuração do portE (numRand1) - 5 pinos de entrada
    GPIO_PORTE_DIR_R  = 0x3F;                                       //habilitando pinos 0,1,2,3,4 e 5 saida.
    GPIO_PORTE_DATA_R = 0x00;                                       //habilitando pinos 0,1,2,3,4 e 5 nivel logico baixo.
    GPIO_PORTE_DEN_R  = 0x3F;                                       //habilitando pinos 0,1,2,3,4 e 5 como pinos digitais.

    //Configuração do portF (botão start e indicação de resultado)
    GPIO_PORTF_DIR_R  = 0x02;                                       //habilitando pino 2 saida.
    GPIO_PORTF_PUR_R  = 0x10;                                       //habilitando pino 4 como pull up.
    GPIO_PORTF_DATA_R = 0x00;                                       //habilitando pinos como nível lógico baixo.
    GPIO_PORTF_IS_R   = 0x00;                                       //habilitando como borda a sensibilidade não como nível.
    GPIO_PORTF_IEV_R  = 0x00;                                       //habilitando borda de descida com nível logico.
    GPIO_PORTF_IBE_R  = 0x00;                                       //desabilitando o uso de borda de subida e descida ao mesmo tempo
    GPIO_PORTF_IM_R   = 0x10;                                       //habilitando interrupçãp no pino 4.
    GPIO_PORTF_DEN_R  = 0x12;                                       //habilitando pinos 2 e 4 como digitais.

    NVIC_EN0_R        = 0x02;                                       //habilita interrupção portB
    NVIC_EN0_R       |= 1 << 30;                                    //habilita interrupção portF


    while(1)
    {
        funcStart();
        tecladoResult();
        geraOp();
        operacao();
        numeroAle1();
        numeroAle2();

        GPIO_PORTE_DATA_R = numRand1;
        GPIO_PORTD_DATA_R = numRand2;

        resultadoUs = GPIO_PORTA_DATA_R;
        if(resultadoUs == resultado)                                    //condição para saber se o usuario acertou ou errou
        {
            GPIO_PORTF_DATA_R = 0x08;                                   //Correto VERDE
            delayLongo();
            GPIO_PORTF_DATA_R = 0x00;
            delayLongo();
        }
        else
        {
            GPIO_PORTF_DATA_R = 0x02;                                   //Errado VERMELHO
            delayLongo();
            GPIO_PORTF_DATA_R = 0x00;
            delayLongo();
        }
    }
    return 0;
}
