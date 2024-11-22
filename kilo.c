/*** Includes ** */
#include <stdlib.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>

/*** Data ***/
struct termios orig_termios;

/*** Terminal ***/
void die(const char *s)
{
    perror(s);
    exit(1);
}

void disableRawMode(void)
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode(void)
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");

    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;

    // Turn off ctrl-s, ctrl-q, ctrl-m
    raw.c_iflag &= ~(ICRNL | IXON);
    // Turn off ctrl-c, ctrl-v, ctrl-z
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    // Turn off output processing
    raw.c_oflag &= ~(OPOST);
    // Turn othe misc flags
    raw.c_cflag |= (CS8);
    raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP);
    // A timeout for read()
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

/*** Init ***/
int main(void)
{
    enableRawMode();
    char c;
    while (1)
    {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
            die("read");
        if (iscntrl(c))
        {
            // We have to write "\r\n" whenever we want to start a new line
            printf("%d\r\n", c);
        }
        else
        {
            // We have to write "\r\n" whenever we want to start a new line
            printf("%d ('%c')\r\n", c, c);
        }
        // Exit the program
        if (c == 'q')
            break;
    }
    return 0;
}
