/**
 * main.c
 */

#include<inttypes.h>
#include<tm4c123gh6pm.h>

int flag;

seq1(){
    GPIO_PORTF_DATA_R = 0b00000010;
    delay();
    GPIO_PORTF_DATA_R = 0b00000100;
    delay();
    GPIO_PORTF_DATA_R = 0b00001000;
    delay();
}

seq2(){
    GPIO_PORTF_DATA_R = 0b00001000;
    delay();
    GPIO_PORTF_DATA_R = 0b00000100;
    delay();
    GPIO_PORTF_DATA_R = 0b00000010;
    delay();
}

delay(){
    uint32_t i;
    for(i=0;i<2000000;i++){}
}

void trataPortF(){
    if((GPIO_PORTF_RIS_R & 0x10)== 0x10){

        delay();
        flag ^= 1;
        GPIO_PORTF_ICR_R = 0x10;
    }
}


int main(void)
{

    SYSCTL_RCGCGPIO_R = 0x20;
    GPIO_PORTF_DIR_R  = 0x0E;
    GPIO_PORTF_PUR_R  = 0x10;
    GPIO_PORTF_DATA_R = 0x00;

    GPIO_PORTF_IS_R  = 0x00;
    GPIO_PORTF_IEV_R = 0x00;
    GPIO_PORTF_IBE_R = 0x00;
    GPIO_PORTF_IM_R  = 0x10;

    GPIO_PORTF_DEN_R  = 0x1E;

    NVIC_EN0_R = 1 << 30;

    while(1){

        if(flag == 0){
            seq1();
        }
        else{
            seq2();
        }
    }
    return 0;
}
