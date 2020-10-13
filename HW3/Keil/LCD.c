#include <stm32f4xx.h>
#include <string.h>

#define RS 0x20 /* PB5 mask for reg select */
#define RW 0x40 /* PB6 mask for read/write */
#define EN 0x80 /* PB7 mask for enable */

#define ROW_COUNT 2

int displayOn = 1;
int cursorOn = 1;

void delayMs(int n);
void portsInit(void);
int lcdInit(int row);
void lcdHome(void);
void lcdClear(void);
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

    lcdInit(ROW_COUNT);

    lcdPosition(7, 2);

    lcdPuts("Here");
    delayMs(1000);

    lcdHome();
    delayMs(1000);

    lcdPutchar('h');
    lcdPutchar('o');
    lcdPutchar('m');
    lcdPutchar('e');
    delayMs(1000);

    lcdClear();
    delayMs(2000);

    lcdPuts("Disp off");
    delayMs(1000);
    lcdDisplay();
    delayMs(1000);

    lcdPuts("NOOO");
    delayMs(1000);

    lcdDisplay();
    delayMs(1000);

    lcdClear();
    delayMs(2000);

    lcdPuts("blink");
    delayMs(1000);
    lcdCursorBlink(0);
    delayMs(4000);

    lcdClear();
    delayMs(2000);

    lcdPuts("cursor");
    delayMs(1000);
    lcdCursor();
    delayMs(4000);
}

/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n)
{
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++)
            ;
}

void portsInit(void)
{
    RCC->AHB1ENR = 0x00;
    RCC->AHB1ENR |= 0x06; /* enable GPIOB/C clock */

    /* PB5 for LCD R/S */
    /* PB6 for LCD R/W */
    /* PB7 for LCD EN */
    GPIOB->MODER = 0x00000280;  /* clear pin mode */
    GPIOB->MODER |= 0x00005400; /* set pin 5, 6, 7 output mode */
    GPIOB->OTYPER = 0x00000000;
    GPIOB->OSPEEDR = 0x000000C0;
    GPIOB->PUPDR = 0x00000100;
    GPIOB->BSRR = 0x00C00000; /* turn off EN and R/W */

    /* PC0-PC7 for LCD D0-D7, respectively. */
    GPIOC->MODER = 0x00000000;  /* clear pin mode */
    GPIOC->MODER |= 0x00005555; /* set pin output mode */
    GPIOC->OTYPER = 0x00000000;
    GPIOC->OSPEEDR = 0x00000000;
    GPIOC->PUPDR = 0x00000000;
    return;
}

int lcdInit(int row)
{
    delayMs(30); /* initialization sequence */
    if (row == 1)
    {
        lcdSendCommand(0x30); /* set 8-bit data, 1-line, 5x7 font */
    }
    else
    {
        lcdSendCommand(0x38); /* set 8-bit data, 2-line, 5x7 font */
    }

    lcdSendCommand(0x0F); /* turn on display, cursor blinking */

    lcdSendCommand(0x01); /* clear screen, move cursor to home */

    lcdSendCommand(0x06); /* move cursor right after each char */
    return 0;
}

void lcdHome(void)
{
    lcdSendCommand(0x02);
    return;
}

void lcdClear(void)
{
    lcdSendCommand(0x01);
    return;
}

void lcdDisplay(void)
{
    if (displayOn)
    {
        displayOn = 0;
        lcdSendCommand(0x08); /* display off, cursor off, blink off */
    }
    else
    {
        displayOn = 1;
        lcdSendCommand(0x0F);
    }
    return;
}

void lcdCursor(void)
{
    if (cursorOn)
    {
        cursorOn = 0;
        lcdSendCommand(0x0C);
    }
    else
    {
        cursorOn = 1;
        lcdSendCommand(0x0F);
    }
    return;
}

void lcdCursorBlink(int state)
{
    if (state)
    {
        lcdSendCommand(0x0F);
    }
    else
    {
        lcdSendCommand(0x0E);
    }
    return;
}

void lcdSendCommand(unsigned char command)
{
    GPIOB->BSRR = (RS | RW) << 16; /* RS = 0, R/W = 0 */
    GPIOC->ODR = command;          /* put command on data bus */
    GPIOB->BSRR = EN;              /* pulse E high */
    delayMs(0);
    GPIOB->BSRR = EN << 16; /* clear E */

    if (command < 4)
        delayMs(2); /* command 1 and 2 needs up to 1.64ms */
    else
        delayMs(1); /* all others 40 us */
    return;
}

void lcdPosition(int x, int y)
{
    int i = 0;
    switch (y)
    {
    case 1:
    {
        lcdSendCommand(0x80);
        break;
    }
    case 2:
    {
        lcdSendCommand(0xC0);
        break;
    }
    case 3:
    {
        lcdSendCommand(0x90);
        break;
    }
    case 4:
    {
        lcdSendCommand(0xD0);
        break;
    }
    }

    for (i = 0; i < x; i++)
    {
        lcdSendCommand(0x14);
    }
    return;
}

void lcdPutchar(unsigned char data)
{
    GPIOB->BSRR = RS;       /* RS = 1 */
    GPIOB->BSRR = RW << 16; /* R/W = 0 */
    GPIOC->ODR = data;      /* put data on data bus */
    GPIOB->BSRR = EN;       /* pulse E high */
    delayMs(0);
    GPIOB->BSRR = EN << 16; /* clear E */

    delayMs(1);
    return;
}

void lcdPuts(const char *string)
{
    int i = 0;
    for (i = 0; i < strlen(string); i++)
    {
        lcdPutchar(string[i]);
        delayMs(10);
    }
    return;
}
