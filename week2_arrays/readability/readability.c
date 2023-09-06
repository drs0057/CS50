#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
int index(int l, int w, int s);

int main(void)
{
    string text = get_string("Text: ");
    int l = count_letters(text);
    int w = count_words(text);
    int s = count_sentences(text);
    int i = index(l, w, s); //the function takes the letters, words, and sentences and will compute the averages

    if (i >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (i <= 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", i);
    }
}

int count_letters(string text)
{
    int n = 0;
    int letters = 0;

    while (text[n] != 0)
    {
        if (islower(text[n]) || isupper(text[n]))
        {
            letters += 1;
        }

        n++;
    }

    return letters;
}

int count_words(string text)
{
    int n = 0;
    int words = 0;

    while (text[n] != 0)
    {
        if (isspace(text[n]))
        {
            words += 1;
        }

        n++;
    }

    words += 1; //above function will always be short by one word, as the very last word is ended with a period, not a space
    return words;
}

int count_sentences(string text)
{
    int n = 0;
    int sentences = 0;

    while (text[n] != 0)
    {
        if (text[n] == '.' || text[n] == '?' || text[n] == '!')
        {
            sentences += 1;
        }

        n++;
    }

    return sentences;
}

int index(l, w, s)
{
    float al = ((float) l / w) * 100;
    float as = ((float) s / w) * 100;
    float i = (0.0588 * al) - (0.296 * as) - 15.8;
    int index = round(i); //must round to nearest int, not round down

    return index;
}
