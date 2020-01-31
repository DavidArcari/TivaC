#include <inttypes.h>
#include <tm4c123gh6pm.h>

#define LED_RED     0x2
#define LED_BLUE    0x4
#define LED_GREEN   0x8
#define LEDS_MASK   0xE

void delay()
{
    unsigned int i;
    for (i = 0; i < 1000000; i++);
}

void set_led(int led)
{
    GPIO_PORTF_DATA_R &= ~LEDS_MASK;
    GPIO_PORTF_DATA_R |= led;
}

void main(void)
{
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R5;
    GPIO_PORTF_DIR_R = LED_RED | LED_BLUE | LED_GREEN;
    GPIO_PORTF_DEN_R = LED_RED | LED_BLUE | LED_GREEN;

    while(1)
    {
        set_led(LED_RED);
        delay();
        set_led(LED_BLUE);
        delay();
        set_led(LED_GREEN);
        delay();
    }
}
