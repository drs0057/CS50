#include <stdio.h>

int main(void)  // Stores and prints an integer's address
{
    int n = 50;
    int *p = &n; // syntax is (type of variable you want to point at) *(variable name). p is a pointer
    printf("%p\n", p);
}


int main(void) // Stores and prints an integer via its address
{
    int n = 50;
    int *p = &n;
    printf("%i\n", *p); // *p means go to address stored in p
}

int main(void) // Prints a string's address as well the addresses of its chars
{
    string s = "HI!";
    printf("%p\n", s);
    printf("%p\n", &s[0]);
    printf("%p\n", &s[1]);
    printf("%p\n", &s[2]);
    printf("%p\n", &s[3]);
}

int main(void) // Prints a string's chars
{
    char *s = "HI!";
    printf("%c\n", s[0]);
    printf("%c\n", s[1]);
    printf("%c\n", s[2]);
}

int main(void) // Prints a string's chars via pointer arithmetic
{
    char *s = "HI!";
    printf("%c\n", *s);
    printf("%c\n", *(s + 1));
    printf("%c\n", *(s + 2));
}

int main(void) // Prints substrings via pointer arithmetic
{
    char *s = "HI!";
    printf("%s\n", s);
    printf("%s\n", s + 1);
    printf("%s\n", s + 2);
}
