// #include <stm32f4xx.h>
#define RCC_AHB1ENR (*((volatile unsigned int *)0x40023830))
#define GPIOA_MODER (*((volatile unsigned int *)0x40020000))
#define GPIOA_IDR (*((volatile unsigned int *)0x40020010))
#define GPIOA_ODR (*((volatile unsigned int *)0x40020014))
#define GPIOB_MODER (*((volatile unsigned int *)0x40020400))
#define GPIOB_IDR (*((volatile unsigned int *)0x40020410))
#define GPIOB_ODR (*((volatile unsigned int *)0x40020414))

unsigned char firstNumber[] = {0x00, 0x00};
unsigned char operator= '0';
unsigned char secondNumber[] = {0x00, 0x00};
unsigned char result[] = {0x00, 0x00};
unsigned char currentNumber[] = {0x00, 0x00};

int equalIsPressed = 0;
int initIsPressed = 0;
int firstRowInput;
int secondRowInput;
int thirdRowInput;
int fourthRowInput;
int buttonPressed = -1;
int firstNumberLength = 0;
int secondNumberLength = 0;
int currentNumberLength = 0;

void delayMs(int n);
void init(void);
void initPortA(void);
void initPortB(void);
void print(unsigned char digit1, unsigned char digit2);
void checkFirstRow(void);
void checkSecondRow(void);
void checkThirdRow(void);
void checkFourthRow(void);
int strToInt(unsigned char number[]);
void initVariables(void);

int main(void)
{
    init();
    initPortA();
    initPortB();

    while (1)
    {
        initIsPressed = 0;
        while (operator== '0')
        {
            checkFirstRow();
            checkSecondRow();
            checkThirdRow();
            checkFourthRow();
        }

        firstNumber[0] = currentNumber[0];
        firstNumber[1] = currentNumber[1];
        firstNumberLength = currentNumberLength;
        currentNumber[0] = 0x00;
        currentNumber[1] = 0x00;
        currentNumberLength = 0;

        while (!equalIsPressed)
        {
            checkFirstRow();
            checkSecondRow();
            checkThirdRow();
            checkFourthRow();

            if (initIsPressed)
            {
                break;
            }
        }

        if (initIsPressed)
        {
            continue;
        }

        secondNumber[0] = currentNumber[0];
        secondNumber[1] = currentNumber[1];
        secondNumberLength = currentNumberLength;
        currentNumber[0] = 0x00;
        currentNumber[1] = 0x00;
        currentNumberLength = 0;

        switch (operator)
        {
        case '*':
        {
            int first = strToInt(firstNumber);
            int second = strToInt(secondNumber);
            int res = (first * second) % 100;
            result[0] = res % 10;
            result[1] = res / 10;
            currentNumber[0] = result[0];
            currentNumber[1] = result[1];
            print(result[0], result[1]);
            break;
        }
        case '/':
        {
            int first = strToInt(firstNumber);
            int second = strToInt(secondNumber);
            int res = (first / second) % 100;
            result[0] = res % 10;
            result[1] = res / 10;
            currentNumber[0] = result[0];
            currentNumber[1] = result[1];
            print(result[0], result[1]);
            break;
        }
        case '-':
        {
            int first = strToInt(firstNumber);
            int second = strToInt(secondNumber);
            int res = (first - second) % 100;
            result[0] = res % 10;
            result[1] = res / 10;
            currentNumber[0] = result[0];
            currentNumber[1] = result[1];
            print(result[0], result[1]);
            break;
        }
        case '+':
        {
            int first = strToInt(firstNumber);
            int second = strToInt(secondNumber);
            int res = (first + second) % 100;
            result[0] = res % 10;
            result[1] = res / 10;
            currentNumber[0] = result[0];
            currentNumber[1] = result[1];
            print(result[0], result[1]);
            break;
        }
        }

        while (!initIsPressed)
        {
            checkFourthRow();
        }
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

void print(unsigned char digit1, unsigned char digit2) // digit1=yekan, digit2=dahgan
{
    GPIOA_ODR = 0x0000;
    GPIOA_ODR |= digit2 << 4;
    GPIOA_ODR |= digit1;
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
            if (currentNumberLength == 0)
            {
                currentNumber[0] = 0x07;
                currentNumberLength++;
            }
            else if (currentNumberLength == 1)
            {
                currentNumber[1] = currentNumber[0];
                currentNumber[0] = 0x07;
                currentNumberLength++;
            }
        }
        buttonPressed = 7;
        break;
    }
    case 0xFFED:
    {
        if (buttonPressed != 8)
        {
            if (currentNumberLength == 0)
            {
                currentNumber[0] = 0x08;
                currentNumberLength++;
            }
            else if (currentNumberLength == 1)
            {
                currentNumber[1] = currentNumber[0];
                currentNumber[0] = 0x08;
                currentNumberLength++;
            }
        }
        buttonPressed = 8;
        break;
    }
    case 0xFFEB:
    {
        if (buttonPressed != 9)
        {
            if (currentNumberLength == 0)
            {
                currentNumber[0] = 0x09;
                currentNumberLength++;
            }
            else if (currentNumberLength == 1)
            {
                currentNumber[1] = currentNumber[0];
                currentNumber[0] = 0x09;
                currentNumberLength++;
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
            if (currentNumberLength == 1 || currentNumberLength == 2)
            {
                currentNumberLength = 3;
                operator= '/';
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
    print(currentNumber[0], currentNumber[1]);
}

void checkSecondRow()
{
    // Read second row of keypad
    GPIOB_ODR = 0xFFDF;
    secondRowInput = GPIOB_IDR;
    switch (secondRowInput)
    {
    case 0xFFDE:
    {
        if (buttonPressed != 4)
        {
            if (currentNumberLength == 0)
            {
                currentNumber[0] = 0x04;
                currentNumberLength++;
            }
            else if (currentNumberLength == 1)
            {
                currentNumber[1] = currentNumber[0];
                currentNumber[0] = 0x04;
                currentNumberLength++;
            }
        }
        buttonPressed = 4;
        break;
    }
    case 0xFFDD:
    {
        if (buttonPressed != 5)
        {
            if (currentNumberLength == 0)
            {
                currentNumber[0] = 0x05;
                currentNumberLength++;
            }
            else if (currentNumberLength == 1)
            {
                currentNumber[1] = currentNumber[0];
                currentNumber[0] = 0x05;
                currentNumberLength++;
            }
        }
        buttonPressed = 5;
        break;
    }
    case 0xFFDB:
    {

        if (buttonPressed != 6)
        {
            if (currentNumberLength == 0)
            {
                currentNumber[0] = 0x06;
                currentNumberLength++;
            }
            else if (currentNumberLength == 1)
            {
                currentNumber[1] = currentNumber[0];
                currentNumber[0] = 0x06;
                currentNumberLength++;
            }
        }
        buttonPressed = 6;
        break;
    }
    case 0xFFD7:
    {
        // Multiply = 11
        if (buttonPressed != 11)
        {
            if (currentNumberLength == 1 || currentNumberLength == 2)
            {
                currentNumberLength = 3;
                operator= '*';
            }
        }
        buttonPressed = 11;
        break;
    }
    default:
    {
        if (buttonPressed == 4 || buttonPressed == 5 || buttonPressed == 6 || buttonPressed == 11)
        {
            buttonPressed = -1;
        }
    }
    }
    print(currentNumber[0], currentNumber[1]);
}

void checkThirdRow()
{
    // Read third row of keypad
    GPIOB_ODR = 0xFFBF;
    thirdRowInput = GPIOB_IDR;
    switch (thirdRowInput)
    {
    case 0xFFBE:
    {
        if (buttonPressed != 1)
        {
            if (currentNumberLength == 0)
            {
                currentNumber[0] = 0x01;
                currentNumberLength++;
            }
            else if (currentNumberLength == 1)
            {
                currentNumber[1] = currentNumber[0];
                currentNumber[0] = 0x01;
                currentNumberLength++;
            }
        }
        buttonPressed = 1;
        break;
    }
    case 0xFFBD:
    {
        if (buttonPressed != 2)
        {
            if (currentNumberLength == 0)
            {
                currentNumber[0] = 0x02;
                currentNumberLength++;
            }
            else if (currentNumberLength == 1)
            {
                currentNumber[1] = currentNumber[0];
                currentNumber[0] = 0x02;
                currentNumberLength++;
            }
        }
        buttonPressed = 2;
        break;
    }
    case 0xFFBB:
    {
        if (buttonPressed != 3)
        {
            if (currentNumberLength == 0)
            {
                currentNumber[0] = 0x03;
                currentNumberLength++;
            }
            else if (currentNumberLength == 1)
            {
                currentNumber[1] = currentNumber[0];
                currentNumber[0] = 0x03;
                currentNumberLength++;
            }
        }
        buttonPressed = 3;
        break;
    }
    case 0xFFB7:
    {
        if (buttonPressed != 12)
        {
            if (currentNumberLength == 1 || currentNumberLength == 2)
            {
                currentNumberLength = 3;
                operator= '-';
            }
        }
        buttonPressed = 12;
        break;
    }
    default:
    {
        if (buttonPressed == 1 || buttonPressed == 2 || buttonPressed == 3 || buttonPressed == 12)
        {
            buttonPressed = -1;
        }
    }
    }
    print(currentNumber[0], currentNumber[1]);
}

void checkFourthRow()
{
    // Read fourth row of keypad
    GPIOB_ODR = 0xFF7F;
    fourthRowInput = GPIOB_IDR;
    switch (fourthRowInput)
    {
    case 0xFF7E: // ON/C
    {
        if (buttonPressed != 15)
        {
            initIsPressed = 1;
            initVariables();
        }
        buttonPressed = 15;
        break;
    }
    case 0xFF7D: // 0
    {
        if (buttonPressed != 0)
        {
            if (currentNumberLength == 0)
            {
                currentNumber[0] = 0x00;
                currentNumberLength++;
            }
            else if (currentNumberLength == 1)
            {
                currentNumber[1] = currentNumber[0];
                currentNumber[0] = 0x00;
                currentNumberLength++;
            }
        }
        buttonPressed = 0;
        break;
    }
    case 0xFF7B: // =
    {
        if (buttonPressed != 13)
        {
            if ((currentNumberLength == 1 || currentNumberLength == 2) && operator!= '0')
            {
                equalIsPressed = 1;
            }
        }
        buttonPressed = 14;
        break;
    }
    case 0xFF77: // +
    {
        if (buttonPressed != 13)
        {
            if (currentNumberLength == 1 || currentNumberLength == 2)
            {
                currentNumberLength = 3;
                operator= '+';
            }
        }
        buttonPressed = 13;
        break;
    }
    default:
    {
        if (buttonPressed == 0 || buttonPressed == 13 || buttonPressed == 14 || buttonPressed == 15)
        {
            buttonPressed = -1;
        }
    }
    }
    print(currentNumber[0], currentNumber[1]);
}

int strToInt(unsigned char number[])
{
    return number[1] * 10 + number[0];
}

void initVariables(void)
{
    firstNumber[0] = 0x00;
    firstNumber[1] = 0x00;
    operator= '0';
    secondNumber[0] = 0x00;
    secondNumber[1] = 0x00;
    result[0] = 0x00;
    result[1] = 0x00;
    currentNumber[0] = 0x00;
    currentNumber[1] = 0x00;

    equalIsPressed = 0;
    firstNumberLength = 0;
    secondNumberLength = 0;
    currentNumberLength = 0;
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
