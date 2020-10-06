// #include <stm32f4xx.h>
#define RCC_AHB1ENR (*((volatile unsigned int *)0x40023830))
#define GPIOA_MODER (*((volatile unsigned int *)0x40020000))
#define GPIOA_IDR (*((volatile unsigned int *)0x40020010))
#define GPIOA_ODR (*((volatile unsigned int *)0x40020014))
#define GPIOB_MODER (*((volatile unsigned int *)0x40020400))
#define GPIOB_IDR (*((volatile unsigned int *)0x40020410))
#define GPIOB_ODR (*((volatile unsigned int *)0x40020414))

unsigned char firstNumber[] = {0x00, 0x00};
unsigned char operator = '0';
unsigned char secondNumber[] = {0x00, 0x00};
int firstRowInput;
int secondRowInput;
int thirdRowInput;
int fourthRowInput;
int buttonPressed = -1;
int firstNumberLength = 0;
int secondNumberLength = 0;

void delayMs(int n);
void init(void);
void initPortA(void);
void initPortB(void);
void print(void);
void checkFirstRow(void);
void checkSecondRow(void);
void checkThirdRow(void);
void checkFourthRow(void);

int main(void)
{
    init();
    initPortA();
    initPortB();

    while (1)
    {
        print();
        checkFirstRow();
        // checkSecondRow();
        // checkThirdRow();
        // checkFourthRow();
    }
}

void init(void)
{
    RCC_AHB1ENR |= 0x00000003; /* enable GPIOA & GPIOB clock */
}

void initPortA(void)
{
    GPIOA_MODER &= 0x0C000000; /* clear pin mode */
    GPIOA_MODER = 0x55555555;  /* set pin to output mode */
}

void initPortB(void)
{
    GPIOB_MODER &= 0x00000280; /* clear pin mode */
    GPIOB_MODER = 0x00005500;
}

void print()
{
    GPIOA_ODR = 0x0000;
    if (firstNumberLength != 3)
    {
        GPIOA_ODR |= firstNumber[0] << 4;
        GPIOA_ODR |= firstNumber[1];
    }
    else if (secondNumberLength == 0 || secondNumberLength == 1)
    {
        GPIOA_ODR |= secondNumber[0] << 4;
        GPIOA_ODR |= secondNumber[1];
    }
    // 1sec/25/4 = 10 milisec
    delayMs(10);
}

void checkFirstRow()
{
    // Read first row of keypad
    GPIOB_ODR = 0xFFEF;
    firstRowInput = GPIOB_IDR;
    switch (firstRowInput)
    {
    case 0xFFEE:
    {
        if (buttonPressed != 7)
        {
            if (firstNumberLength == 0)
            {
                firstNumber[0] = 0x07;
                firstNumberLength++;
            }
            else if (firstNumberLength == 1)
            {
                firstNumber[1] = 0x07;
                firstNumberLength++;
            }
            else
            {
            }
        }
        buttonPressed = 7;
        break;
    }
    case 0xFFED:
    {
        if (buttonPressed != 8)
        {
            if (firstNumberLength == 0)
            {
                firstNumber[0] = 0x08;
                firstNumberLength++;
            }
            else if (firstNumberLength == 1)
            {
                firstNumber[1] = 0x08;
                firstNumberLength++;
            }
            else
            {
            }
        }
        buttonPressed = 8;
        break;
    }
    case 0xFFEB:
    {
        if (buttonPressed != 9)
        {
            if (firstNumberLength == 0)
            {
                firstNumber[0] = 0x09;
                firstNumberLength++;
            }
            else if (firstNumberLength == 1)
            {
                firstNumber[1] = 0x09;
                firstNumberLength++;
            }
            else
            {
            }
        }
        buttonPressed = 9;
        break;
    }
    case 0xFFE7:
    {
        // Division = 10
        if (buttonPressed != 10)
        {
            if (firstNumberLength == 1 || firstNumberLength == 2)
            {
                firstNumberLength = 3;
                operator = '/';
            }
        }
        buttonPressed = 10;
        break;
    }
    default:
    {
        if (buttonPressed == 7 || buttonPressed == 8 || buttonPressed == 9 || buttonPressed == 10)
        {
            buttonPressed = -1;
        }
    }
    }
}

void checkSecondRow()
{
    // KPB = 0, Read second row of keypad
    GPIOB_ODR = 0xDFFF;
    secondRowInput = GPIOB_IDR;
    switch (secondRowInput)
    {
    case 0xDEFF:
    {
        if (buttonPressed != 4)
        {
            // shift
            int i;
            for (i = 3; i > 0; i--)
            {
                firstNumber[i] = firstNumber[i - 1];
            }
            firstNumber[0] = 0x66;
        }
        buttonPressed = 4;
        break;
    }
    case 0xDDFF:
    {
        if (buttonPressed != 5)
        {
            // shift
            int i;
            for (i = 3; i > 0; i--)
            {
                firstNumber[i] = firstNumber[i - 1];
            }
            firstNumber[0] = 0x6D;
        }
        buttonPressed = 5;
        break;
    }
    case 0xDBFF:
    {
        if (buttonPressed != 6)
        {
            // shift
            int i;
            for (i = 3; i > 0; i--)
            {
                firstNumber[i] = firstNumber[i - 1];
            }
            firstNumber[0] = 0x7D;
        }
        buttonPressed = 6;
        break;
    }
    default:
    {
        if (buttonPressed == 4 || buttonPressed == 5 || buttonPressed == 6)
        {
            buttonPressed = -1;
        }
    }
    }
}

void checkThirdRow()
{
    // KPC = 0, Read first row of keypad
    GPIOB_ODR = 0xBFFF;
    thirdRowInput = GPIOB_IDR;
    switch (thirdRowInput)
    {
    case 0xBEFF:
    {
        if (buttonPressed != 7)
        {
            // shift
            int i;
            for (i = 3; i > 0; i--)
            {
                firstNumber[i] = firstNumber[i - 1];
            }
            firstNumber[0] = 0x07;
        }
        buttonPressed = 7;
        break;
    }
    case 0xBDFF:
    {
        if (buttonPressed != 8)
        {
            // shift
            int i;
            for (i = 3; i > 0; i--)
            {
                firstNumber[i] = firstNumber[i - 1];
            }
            firstNumber[0] = 0x7F;
        }
        buttonPressed = 8;
        break;
    }
    case 0xBBFF:
    {
        if (buttonPressed != 9)
        {
            // shift
            int i;
            for (i = 3; i > 0; i--)
            {
                firstNumber[i] = firstNumber[i - 1];
            }
            firstNumber[0] = 0x6F;
        }
        buttonPressed = 9;
        break;
    }
    default:
    {
        if (buttonPressed == 7 || buttonPressed == 8 || buttonPressed == 9)
        {
            buttonPressed = -1;
        }
    }
    }
}

void checkFourthRow()
{
    // KPD = 0, Read first row of keypad
    GPIOB_ODR = 0x7FFF;
    fourthRowInput = GPIOB_IDR;
    switch (fourthRowInput)
    {
    case 0x7DFF:
    {
        if (buttonPressed != 0)
        {
            // shift
            int i;
            for (i = 3; i > 0; i--)
            {
                firstNumber[i] = firstNumber[i - 1];
            }
            firstNumber[0] = 0x3F;
        }
        buttonPressed = 0;
        break;
    }
    default:
    {
        if (buttonPressed == 0)
        {
            buttonPressed = -1;
        }
    }
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
