#include <stm32f4xx.h>
#include <string.h>

#define LCD_DATA_PIN 8

struct GlcdDataStruct
{
    unsigned int rs;
    unsigned int rw;
    unsigned int e;
    unsigned int dataPins[LCD_DATA_PIN];
    unsigned int cs1;
    unsigned int cs2;
    unsigned int res;
};

void delayMs(int n);
void portsInit(void);
int GlcdInit(const int rows, const int cols, const int rw, const int res,
             const int RS_DI, const int di, const int CS1, const int CS2,
             const int d0, const int d1, const int d2, const int d3,
             const int d4, const int d5, const int d6, const int d7);

void set_x_address(unsigned char, unsigned char);
void set_line_no(unsigned char, unsigned char);
void update(void);
void cls(void);
void clearPixel(unsigned char, unsigned char);
void scrollUp(void);
void setGrCursor(unsigned char, unsigned char);
int glcdPutchar(const char c);
void glcdPuts(const char *string);
void glcdCommand(unsigned char command);
struct GlcdDataStruct GlcdData;

int main()
{
    int i = 0;
    portsInit();
    GlcdInit(0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1);
    // for (i = 0; i < 50; i++)
    // {
    //     glcdPutchar(0X1);
    // }

    while (1)
    {
    }
}

/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n)
{
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++)
            ;
}

void glcdCommand(unsigned char command)
{
    GPIOA->BSRR = 0x03000000; /* RS = 0, R/W = 0 */
    GPIOA->ODR = command;
    GPIOB->BSRR = 0x00000001;
    delayMs(0);
    GPIOB->BSRR = 0x00010000;
    return;
}

void portsInit(void)
{
    RCC->AHB1ENR = 0x00;
    RCC->AHB1ENR |= 0x03; /* enable GPIOA and GPIOB clock */

    GPIOA->MODER = 0x0C000000;  /* clear pin mode */
    GPIOA->MODER |= 0x00055555; /* set PA0-PA9 to output mode */
    GPIOA->OTYPER = 0x00000000;
    GPIOA->OSPEEDR = 0x0C000000;
    GPIOA->PUPDR = 0x64000000;

    GPIOB->MODER = 0x00000280;  /* clear pin mode */
    GPIOB->MODER |= 0x00000055; /* set PB0-PB3 to output mode */
    GPIOB->OTYPER = 0x00000000;
    GPIOB->OSPEEDR = 0x000000C0;
    GPIOB->PUPDR = 0x00000100;

    return;
}

int GlcdInit(const int rows, const int cols, const int rw, const int res,
             const int RS_DI, const int di, const int CS1, const int CS2,
             const int d0, const int d1, const int d2, const int d3,
             const int d4, const int d5, const int d6, const int d7)
{
    if (rows < 0 || rows > 63)
        return -1;
    if (cols < 0 || cols > 127)
        return -1;

    GlcdData.rw = rw;
    GlcdData.res = res;
    GlcdData.rs = RS_DI;
    GlcdData.cs1 = CS1;
    GlcdData.cs2 = CS2;
    GlcdData.dataPins[0] = d0;
    GlcdData.dataPins[1] = d1;
    GlcdData.dataPins[2] = d2;
    GlcdData.dataPins[3] = d3;
    GlcdData.dataPins[4] = d4;
    GlcdData.dataPins[5] = d5;
    GlcdData.dataPins[6] = d6;
    GlcdData.dataPins[7] = d7;

    GPIOB->BSRR = 0x00020000; /* ~RST=0 */
    delayMs(50);
    GPIOB->BSRR = 0x00000002; /* ~RST=1 */
    delayMs(50);
    glcdCommand(0x0000003E); /* display off */
    // glcdCommand(0x00000040); /* Y=0 */
    // glcdCommand(0x000000B8); /* X=0 */
    glcdCommand(0x000000C0); /* line=0 */
    glcdCommand(0x0000003F); /* display on */

    // GPIOB->BSRR = 0x0000000C;
    return 0;
}

void set_x_address(unsigned char cs_no, unsigned char pixle_no)
{
}
void set_line_no(unsigned char page, unsigned char cs_no)
{
}
void update()
{
}
void cls()
{
}
void clearPixel(unsigned char x, unsigned char y)
{
}
void scrollUp()
{
}
void setGrCursor(unsigned char x, unsigned char y)
{
}
int glcdPutchar(const char c)
{
    GPIOA->BSRR = 0x01000200; /* RS=1, RW=0 */
    GPIOA->ODR = c;
    GPIOB->BSRR = 0x00000003;
    delayMs(0);
    GPIOB->BSRR = 0x00010002;

    delayMs(1);
    return 1;
}
void glcdPuts(const char *string)
{
    int i = 0;
    for (i = 0; i < strlen(string); i++)
    {
        glcdPutchar(string[i]);
        delayMs(10);
    }
    return;
}
