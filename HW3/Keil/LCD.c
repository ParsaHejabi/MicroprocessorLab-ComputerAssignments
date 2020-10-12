#include <stm32f4xx.h>

#define RS 0x20     /* PB5 mask for reg select */
#define RW 0x40     /* PB6 mask for read/write */
#define EN 0x80     /* PB7 mask for enable */

void delayMs(int n);

void portsInit(void);

int lcdInit(void);
void lcdHome(void);
void lcdDisplay(void);
void lcdCursor(void);
void lcdCursorBlink(int state);
void lcdSendCommand(unsigned char command);
void lcdPosition(int x, int y);
void lcdPutchar(unsigned char data);
void lcdPuts(const char *string);

int main()
{
    portsInit();
    /* initialize LCD controller */
    lcdInit();

    while(1) {
        /* Write "hello" on LCD */
        lcdPutchar('h');
        lcdPutchar('e');
        lcdPutchar('l');
        lcdPutchar('l');
        lcdPutchar('o');
        delayMs(1000);

        /* clear LCD display */
        lcdSendCommand(1);
        delayMs(500);
    }
}

/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++);
}

void portsInit(void)
{
    RCC->AHB1ENR |=  0x06;          /* enable GPIOB/C clock */

    /* PB5 for LCD R/S */
    /* PB6 for LCD R/W */
    /* PB7 for LCD EN */
    GPIOB->MODER &= ~0x0000FC00;    /* clear pin mode */
    GPIOB->MODER |=  0x00005400;    /* set pin output mode */
    GPIOB->BSRR = 0x00C00000;       /* turn off EN and R/W */

    /* PC0-PC7 for LCD D0-D7, respectively. */
    GPIOC->MODER &= ~0x0000FFFF;    /* clear pin mode */
    GPIOC->MODER |=  0x00005555;    /* set pin output mode */
    return;
}

int lcdInit(void)
{
    delayMs(30);            /* initialization sequence */
    lcdSendCommand(0x30);
    delayMs(10);
    lcdSendCommand(0x30);
    delayMs(1);
    lcdSendCommand(0x30);

    lcdSendCommand(0x38);      /* set 8-bit data, 2-line, 5x7 font */
    lcdSendCommand(0x06);      /* move cursor right after each char */
    lcdSendCommand(0x01);      /* clear screen, move cursor to home */
    lcdSendCommand(0x0F);      /* turn on display, cursor blinking */
    return 0;
}

void lcdHome(void)
{
    return;
}

void lcdDisplay(void)
{
    return;
}

void lcdCursor(void)
{
    return;
}

void lcdCursorBlink(int state)
{
    return;
}

void lcdSendCommand(unsigned char command)
{
    GPIOB->BSRR = (RS | RW) << 16;  /* RS = 0, R/W = 0 */
    GPIOC->ODR = command;           /* put command on data bus */
    GPIOB->BSRR = EN;               /* pulse E high */
    delayMs(0);
    GPIOB->BSRR = EN << 16;         /* clear E */

    if (command < 4)
        delayMs(2);         /* command 1 and 2 needs up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
    return;
}

void lcdPosition(int x, int y)
{
    return;
}

void lcdPutchar(unsigned char data)
{
    GPIOB->BSRR = RS;               /* RS = 1 */
    GPIOB->BSRR = RW << 16;         /* R/W = 0 */
    GPIOC->ODR = data;              /* put data on data bus */
    GPIOB->BSRR = EN;               /* pulse E high */
    delayMs(0);
    GPIOB->BSRR = EN << 16;         /* clear E */

    delayMs(1);
    return;
}

void lcdPuts(const char *string)
{
    return;
}
