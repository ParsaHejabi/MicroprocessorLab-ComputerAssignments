#define RCC_AHB1ENR (*((volatile unsigned int *)0x40023830))
#define GPIOA_MODER (*((volatile unsigned int *)0x40020000))
#define GPIOA_BSRR (*((volatile unsigned int *)0x40020018))

void delayMs(int n);

int main(void)
{
    RCC_AHB1ENR = 0x00000001; /* enable GPIOA clock */
    GPIOA_MODER &= 0x0C000000; /* clear pin mode */
    GPIOA_MODER = 0x55555555; /* set pin to output mode */

    while (1)
    {
        GPIOA_BSRR = 0x0000FFFF; /* turn on LED */
        delayMs(1000);
        GPIOA_BSRR = 0xFFFF0000; /* turn off LED */
        delayMs(1000);
    }
}

/* 16 MHz SYSCLK */
void delayMs(int n)
{
    int i;
    for (; n > 0; n--)
    {
        for (i = 0; i < 6000; i++)
        {
        }
    }
}
