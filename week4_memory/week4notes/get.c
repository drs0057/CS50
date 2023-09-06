#include <stdio.h>

int main(void) // Gets an int from user using scanf
{
    int x;
    printf("x: ");
    scanf("%i", &x);
    printf("x: %i\n", x);
}

int main(void) // Dangerously gets a string from user using scanf
{
    char s[4];
    printf("s: ");
    scanf("%s", s);
    printf("s: %s\n", s);
}
