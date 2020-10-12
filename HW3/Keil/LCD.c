#include <stm32f4xx.h>

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
}

int lcdInit(void)
{
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
    return;
}

void lcdPosition(int x, int y)
{
    return;
}

void lcdPutchar(unsigned char data)
{
    return;
}

void lcdPuts(const char *string)
{
    return;
}
