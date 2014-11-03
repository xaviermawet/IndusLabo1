#include "common.h"

int getNumber(void)
{
    char c;
    int number = 0;
    int sign = 1;

    if ((c = getchar()) == '-')
    {
        c = '0';
        sign = -1;
    }

    while (c >= '0' && c <= '9')
    {
        // Code ASCII de (c - 48) correspond au nb entré
        number = number * 10 + (c - 48);
        c = getchar();
    }

    // Vide le buffer
    if (c != '\n')
        FREE_BUFF

    return (number * sign);
}

int getRandomNumber(int min, int max)
{
    int range, result, cutoff;

    if (min >= max)
        return min; // only one outcome possible, or invalid parameters

    range = max - min + 1;
    cutoff = (RAND_MAX / range) * range;

    // Rejection method, to be statistically unbiased.
    do
    {
        result = rand();
    } while (result >= cutoff);

    return result % range + min;
}

void read_stdin(char *string, short length)
{
    char  c;
    short i = 0;

    c = getchar();

    while (c != '\n' && i++ < length - 1)
    {
        *string++ = c;
        c = getchar();
    }

    // caratère de fin de chaine
    *string = '\0';

    if (c != '\n')
        FREE_BUFF
}

bool kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return TRUE;
  }

  return FALSE;
}

int mygetch (void)
{
  int ch;
  struct termios oldt, newt;

  tcgetattr ( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );

  return ch;
}

int reconfigurerTerminal(struct termios* prev)
{
    struct termios newConfig;

    if(tcgetattr(fileno(stdin), prev) == -1)
        return -1;

    newConfig.c_iflag = prev->c_iflag;
    newConfig.c_oflag = prev->c_oflag;
    newConfig.c_cflag = prev->c_cflag;
    newConfig.c_lflag = 0;
    newConfig.c_cc[VMIN]  = 1;
    newConfig.c_cc[VTIME] = 0;

    if(tcsetattr(fileno(stdin), TCSANOW, &newConfig) == -1)
        return -1;

    return 0;
}

int restaurerTerminal(const struct termios *prev)
{
    return tcsetattr(fileno(stdin), TCSANOW, prev);
}
