#include <inttypes.h>
#include <tm4c123gh6pm.h>

#define EEPROM_INIT_OK 0
#define EEPROM_INIT_ERROR 2
#define UART_FR_TXFF            0x00000020                                  // UART Transmite FIFO Full
#define UART_FR_RXFE            0x00000010                                  // UART Recebe FIFO Empty

uint32_t leituraAtual = 0, segundos = 0, freq = 16000, porcent = 0, verfpulso = 0, pulso = 0;

//delay para as funções de inicio do PWM e da UART
delay(){
    uint32_t i;
    for(i=0; i<160000; i++){}
}

//Função que inicia a EEPROM
uint32_t EInit(void)
{
    uint8_t aux;
    SYSCTL_RCGCEEPROM_R = 0x01;
    for(aux=0;aux<2;aux++){}
    while((EEPROM_EEDONE_R & 0x01) == 0x01){}
    if(((EEPROM_EESUPP_R & 0x08) == 0x08) || ((EEPROM_EESUPP_R & 0x04) == 0x04))
    {
        return(EEPROM_INIT_ERROR);
    }

    SYSCTL_SREEPROM_R = 0x01;
    SYSCTL_SREEPROM_R = 0x00;
    for(aux=0;aux<2;aux++){}
    while((EEPROM_EEDONE_R & 0x01) == 0x01){}
    if(((EEPROM_EESUPP_R & 0x08) == 0x08) || ((EEPROM_EESUPP_R & 0x04) == 0x04))
    {
        return(EEPROM_INIT_ERROR);
    }

    return(EEPROM_INIT_OK);

}

//Leitura de dado EEPROM
uint32_t EEPROM_read(uint8_t block, uint8_t offset){
    while(((EEPROM_EEDONE_R&0x20)==0x20) || ((EEPROM_EEDONE_R&0x08)==0x08) || ((EEPROM_EEDONE_R&0x04)==0x04)){}
    EEPROM_EEBLOCK_R = (uint8_t)(1<<block);
    EEPROM_EEOFFSET_R = (uint8_t)offset;
    return (uint32_t)EEPROM_EERDWR_R;
}

//Escreve dado na EEPROM
void EEPROM_write(uint8_t block, uint8_t offset, uint32_t word) {
    while(((EEPROM_EEDONE_R&0x20)==0x20) || ((EEPROM_EEDONE_R&0x08)==0x08) || ((EEPROM_EEDONE_R&0x04)==0x04)){}
    EEPROM_EEBLOCK_R =  (uint8_t)(1<<block);
    EEPROM_EEOFFSET_R = (uint8_t)offset;
    EEPROM_EERDWR_R = (uint32_t)word;
}

//Função que inicia a UART
void UART0_Init(void){
    SYSCTL_RCGCUART_R  |= 0x01;                                 // ativa UART0  pagina 903
    delay();
    SYSCTL_RCGCGPIO_R  |= 0x01;                                 // ativa port A
    GPIO_PORTA_AFSEL_R |= 0x03;                                 // habilita função alternativa nos pinos PA0 e PA1
    GPIO_PORTA_PCTL_R  |= 0x11;                                 // habilita função UART nos pinos PA0 e PA1

    UART0_CTL_R  = 0x00;                                        // desabilita UART
    UART0_IBRD_R = 8;                                           // IBRD = int(16.000.000 / (16 * 115.200)) = int(8,6805)
    UART0_FBRD_R = 44;                                          // FBRD = int(0,6805 * 64 + 0.5) = 44
    UART0_LCRH_R = 0x70;                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
    UART0_CTL_R  = 0x301;                                       // inicializa UART

    GPIO_PORTA_DEN_R |= 0x03;                                   // habilita sinal digital I/O on PA1-0
}

//Saida das infos na UART
void UART0_OutChar(unsigned char data){
    while ((UART0_FR_R & UART_FR_TXFF) != 0) { }
    UART0_DR_R = data;
}

/*Contador Systick, conta até Dez e ai chama a função de escrita da EEPROM
e escreve o pulso atual. Ou seja, a cada 10s é atualizado o valor do pulso,
logo apos chama a função de saida da UART para avisar o usuario que o valor
do pulso foi salvo.*/
void ContaDez(){
    segundos++;
    if (segundos == 10)
    {
        pulso = verfpulso * 6;                          //multiplica a variavel por 6 para ser o valor por minuto
        EEPROM_write(0,0,pulso);                        //Escreve o valor atual na posição [0,0] da EEPROM
        UART0_OutChar(76);                              //Envia mensagem para o usuário pela UART
        UART0_OutChar(69);
        UART0_OutChar(73);
        UART0_OutChar(84);
        UART0_OutChar(85);
        UART0_OutChar(82);
        UART0_OutChar(65);
        UART0_OutChar(32);
        UART0_OutChar(83);
        UART0_OutChar(65);
        UART0_OutChar(76);
        UART0_OutChar(86);
        UART0_OutChar(65);
        UART0_OutChar(10);
        segundos = 0;
        pulso = 0;
    }
}

int main(void) {

    /*aqui estou setando os pinos que vão ser usados como
     * como conversor analógico digital. Nesse caso será o PD0 do portD */

    SYSCTL_RCGCGPIO_R  = 0x08;                            //habilitando o portD
    SYSCTL_RCGCADC_R   = 0x01;                            //habilitando em modulo ADC
    GPIO_PORTD_AFSEL_R = 0x01;                            //indicando que uma função alternativa vai ser usada no pino 0 (nesse caso ADC)
    GPIO_PORTD_AMSEL_R = 0x01;                            //habilitando a função alternativa utilizada (ADC) no pino 0

    //Configuração do ADC
    ADC0_ACTSS_R  = 0x00;                                 //habilitar as filas
    ADC0_EMUX_R   = 0x0F;                                 //mutiplexador de eveto, ele vai definir a taxa de amostragem na fila selecionada. Ele é em 4 em 4 bits.
    ADC0_SSMUX0_R = 0x07;                                 //sequencializador da fila 0. Coloca o valor numero do canal que esta sendo lido, nesse caso valor 7.
    ADC0_SSCTL0_R = 0x02;                                 //escolhe quem vai ser a ultima posição da fila, no caso sera o bit 1 em nivel logico alto.
    ADC0_ACTSS_R  = 0x01;                                 //habilita exclusivamente a fila 0.
    ADC0_PSSI_R   = 0x01;                                 //inicializa a conversão das filas selecionadas. Nesse caso a fila 0.

    //LED DA PLACA PF2 PWM6M1
    SYSCTL_RCGCGPIO_R |= 0x20;                            //Ligar o PortF
    GPIO_PORTF_AFSEL_R = 0x04;                            //Habilitar função alternativa do PF2 - pg 671
    GPIO_PORTF_PCTL_R = 0x00000500;                       //Definir que a função alternativa do PF2 é PWM - pg 688 / tabela pg 1351
    GPIO_PORTF_DEN_R = 0x16;                              //Habilita PF2 digitalmente
    SYSCTL_RCGCPWM_R = 0x02;                              //Ativa o modulo de PWM 0
    delay();
    PWM1_3_GENA_R = 0xC8;                                 //Nivel baixo em LOAD / Nivel alto em CMPA - pg 1282
    PWM1_3_LOAD_R = freq;                                 //Valor de frequencia do PWM, em ciclos de clock - qtde de ciclos de clock 16000 = 1K
    PWM1_3_CTL_R = 0x01;                                  //Ativa o controle do gerador - pg 1265
    PWM1_ENABLE_R = 0x40;                                 //Inicializa o PWM0 - Saida PWM - pg 1247

    //Configutações do Systick
    NVIC_ST_RELOAD_R = 4000000;
    NVIC_ST_CTRL_R   = 0x03;

    //Chamando as funções da EEPROM e UART
    EInit();
    UART0_Init();

    while(1){
        leituraAtual = ADC0_SSFIFO0_R;                    //aqui a variável leituraAtual recebe o DATA do leitor analog
        porcent = (leituraAtual*100)/4096;                //Lógica de conversão do valor do sensor para %
        PWM1_3_CMPA_R = (freq * porcent)/100;             //Valor de porcentagem do PWM, em ciclos de clock.
        if(porcent > 95){                                 //Condição para incrementar o valor dos pulsos
            verfpulso++;
        }
     }
}
