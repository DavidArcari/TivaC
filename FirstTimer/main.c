#include<inttypes.h>
#include<tm4c123gh6pm.h>

uint32_t segundo = 0, deuCerto = 0;

void Trata_ST(){
    segundo++;
    if(segundo == 10){
        segundo = 0;
        deuCerto++;
    }
}

int main(void)
{
    NVIC_ST_RELOAD_R = 4000000;
    NVIC_ST_CTRL_R   = 0x03;

    while(1){
        __asm("WFI");
    }
	return 0;
}
