#include <cs50.h>
#include <stdio.h>

int checksum(long card);

int main(void)
{
    long card = get_long("Number: "); //Get card number
    int x = checksum(card); //x is 1 if valid and 0 if invalid

    //Now check which type of card
    int length = 0;
    long count = card;
    while (count > 0) //Find the length of the card number
    {
        count /= 10;
        length++;
    }

    //Finally check the starting digits
    long amex = card;
    long mstr = card;
    long visa1 = card;
    long visa2 = card;

    amex /= 10000000000000; //collects first two digits of a potential 15-digit amex card
    mstr /= 100000000000000; //collects first two digits of a potential mastercard
    visa1 /= 1000000000000; //collects the first digit of a potential 13-digit visa card
    visa2 /= 1000000000000000; //collects the first digit of a potential 16-digit visa card

    if (x == 1 && length == 15 && (amex == 34 || amex == 37))
    {
        printf("AMEX\n");
    }
    else if (x == 1 && length == 16 && (mstr == 51 || mstr == 52 || mstr == 53 || mstr == 54 || mstr == 55))
    {
        printf("MASTERCARD\n");
    }
    else if (x == 1 && (length == 13 || length == 16) && (visa1 == 4 || visa2 == 4))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

int checksum(long card) //Where num1 is 2nd to last digit *2, num2 is 4th to last *2, etc.
{
    //Gather every second digit starting from 2nd to last and *2
    int num1 = ((card % 100) / 10) * 2;
    int num2 = ((card % 10000) / 1000) * 2;
    int num3 = ((card % 1000000) / 100000) * 2;
    int num4 = ((card % 100000000) / 10000000) * 2;
    int num5 = ((card % 10000000000) / 1000000000) * 2;
    int num6 = ((card % 1000000000000) / 100000000000) * 2;
    int num7 = ((card % 100000000000000) / 10000000000000) * 2;
    int num8 = ((card % 10000000000000000) / 1000000000000000) * 2;

    //Split the products up by digit and sum those digits separately
    num1 = ((num1 % 100) / 10) + (num1 % 10);
    num2 = ((num2 % 100) / 10) + (num2 % 10);
    num3 = ((num3 % 100) / 10) + (num3 % 10);
    num4 = ((num4 % 100) / 10) + (num4 % 10);
    num5 = ((num5 % 100) / 10) + (num5 % 10);
    num6 = ((num6 % 100) / 10) + (num6 % 10);
    num7 = ((num7 % 100) / 10) + (num7 % 10);
    num8 = ((num8 % 100) / 10) + (num8 % 10);

    //Sum all those sums together
    int sum1 = num1 + num2 + num3 + num4 + num5 + num6 + num7 + num8;

    //Now gather every second digit starting from last digit
    //num9 is last digit, num10 is 3rd to last digit, etc.
    int num9 = ((card % 10) / 1);
    int num10 = ((card % 1000) / 100);
    int num11 = ((card % 100000) / 10000);
    int num12 = ((card % 10000000) / 1000000);
    int num13 = ((card % 1000000000) / 100000000);
    int num14 = ((card % 100000000000) / 10000000000);
    int num15 = ((card % 10000000000000) / 1000000000000);
    int num16 = ((card % 1000000000000000) / 100000000000000);

    //Now add sum1 to all the above digits
    int totalsum = sum1 + num9 + num10 + num11 + num12 + num13 + num14 + num15 + num16;

    //Check if last digit is a 0 in totalsum
    int x;
    if (totalsum % 10 == 0)
    {
        return x = 1;
    }
    else
    {
        return x = 0;
    }

}