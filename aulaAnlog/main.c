/**
 * aula conversor analogico - digital
 */

#include <inttypes.h>
#include <tm4c123gh6pm.h>

uint32_t leituraAtual;

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

    while(1){
        leituraAtual = ADC0_SSFIFO0_R /*>> 4*/;                    //aqui é o DATA, valores do analógico
    }
}
