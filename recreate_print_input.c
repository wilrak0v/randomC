/* 17/02/2026
 *
 * Simple program coded by Wilrak0v
 *
 * The goal is to recreate a get string function and a put string function in C.
 *
 */

#include <stdio.h>
#include <string.h>

#define BUFFER_LENGTH 100

// A simple print function
void print(const char *s)
{
    while(*s)
        putchar(*s++);
}

void read(char *s, size_t size) {
    size_t idx = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\r' && c != '\n' && idx < size - 1) {
        s[idx++] = (char)c;
    }
    s[idx] = '\0';
}

int main()
{
    int value = 16;
    char buf[BUFFER_LENGTH];
    snprintf(buf, sizeof(buf), "The value is '%d'\n", value);
    print(buf);

    print("Can you introduce yourself?");
    char enter[BUFFER_LENGTH];
    read(enter, sizeof(enter));

    memset(buf, ' ', sizeof(buf));
    snprintf(buf, sizeof(buf), "You enter the description above right?\n'%s'\n", enter);
    print(buf);
    return 0;
}
