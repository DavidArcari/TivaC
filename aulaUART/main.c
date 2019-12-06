/**
 * Aula de UART - 29/10/2019
 */

// U0Rx connected to PA0
// U0Tx connected to PA1

#include <inttypes.h>
#include <tm4c123gh6pm.h>

#define UART_FR_TXFF            0x00000020            // UART Transmite FIFO Full
#define UART_FR_RXFE            0x00000010            // UART Recebe FIFO Empty

void UART0_Init(void){
    SYSCTL_RCGCUART_R  |= 0x01;                       // ativa UART0  pagina 903
    SYSCTL_RCGCGPIO_R  |= 0x01;                       // ativa port A
    GPIO_PORTA_AFSEL_R |= 0x03;                       // habilita função alternativa nos pinos PA0 e PA1
    GPIO_PORTA_PCTL_R  |= 0x11;                       // habilita função UART nos pinos PA0 e PA1

    UART0_CTL_R  = 0x00;                              // desabilita UART
    UART0_IBRD_R = 8;                                 // IBRD = int(16.000.000 / (16 * 115.200)) = int(8,6805)
    UART0_FBRD_R = 44;                                // FBRD = int(0,6805 * 64 + 0.5) = 44
    UART0_LCRH_R = 0x70;                              // 8 bit word length (no parity bits, one stop bit, FIFOs)
    UART0_CTL_R  = 0x301;                             // inicializa UART

    GPIO_PORTA_DEN_R |= 0x03;                         // habilita sinal digital I/O on PA1-0

}

/*void UART0_OutString(char *c)
{
    int count = 0;
    for(; *c != 0; c++)
    {
        UART0_OutChar(*c);
    }
}*/

unsigned char UART0_InChar(void){
    while ((UART0_FR_R & UART_FR_RXFE) != 0) { }
    return((unsigned char)(UART0_DR_R&0xFF));
}

void UART0_OutChar(unsigned char data){
    while ((UART0_FR_R & UART_FR_TXFF) != 0) { }
    UART0_DR_R = data;
}

void main()
{
    char teste;
    //int sensor = 38;
    SYSCTL_RCGCGPIO_R = 0x20;
    GPIO_PORTF_DIR_R  = 0x0E;
    GPIO_PORTF_PUR_R  = 0x10;
    GPIO_PORTF_DATA_R = 0x00;
    GPIO_PORTF_DEN_R  = 0x1E;

    UART0_Init();
    //sensor
    //dezena = (sensor/10) + 48;
    //unidade = (sensor % 10) + 48;
    while(1)
    {
        //tratamento para gerar strings
        /*centena = UART0_InChar();
        dezena = UART0_InChar();
        unidade = UART0_InChar();
        centena = (centena - 48)* 100;
        dezena = (dezena - 48) * 10;
        unidade = unidade - 48;
        final = centena + dezena + unidade;*/

        if (teste == 'R' || teste == 'r')
            GPIO_PORTF_DATA_R = 0x02;
        if ((GPIO_PORTF_DATA_R & 0x10) == 0x00) {
            UART0_OutChar(66);
            UART0_OutChar(84);
            UART0_OutChar(78);
            UART0_OutChar(49);
            while ((GPIO_PORTF_DATA_R & 0x10) == 0x00) { }
        }
    }
}
