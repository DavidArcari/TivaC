#include <tm4c123gh6pm.h>
#include <inttypes.h>

#define EEPROM_INIT_OK 0
#define EEPROM_INIT_ERROR 2

delay(){
    uint32_t aux;
    for(aux=0;aux<160000;aux++){}
}
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

uint32_t EEPROM_read(uint8_t block, uint8_t offset){
    while(((EEPROM_EEDONE_R&0x20)==0x20) || ((EEPROM_EEDONE_R&0x08)==0x08) || ((EEPROM_EEDONE_R&0x04)==0x04)){}
    EEPROM_EEBLOCK_R = (uint8_t)(1<<block);
    EEPROM_EEOFFSET_R = (uint8_t)offset;
    return (uint32_t)EEPROM_EERDWR_R;
}

void EEPROM_write(uint8_t block, uint8_t offset, uint32_t word) {
    while(((EEPROM_EEDONE_R&0x20)==0x20) || ((EEPROM_EEDONE_R&0x08)==0x08) || ((EEPROM_EEDONE_R&0x04)==0x04)){}
    EEPROM_EEBLOCK_R =  (uint8_t)(1<<block);
    EEPROM_EEOFFSET_R = (uint8_t)offset;
    EEPROM_EERDWR_R = (uint32_t)word;
}

void main() {
    /*uint32_t = volume;
    if(EInit()==0){
        volume = EEPROM_read(0, 0);
        while(1){
            if (btn_mais == press() )
            {
                volume++;
            }
            if (btn_menos == press() )
            {
                volume--;
            }
            if (btn_ok == press() )
            {
                EEPROM_write(0, 0, volume);
            }
        }
    }*/
}
