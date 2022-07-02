#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

string checkCard(long cardNb);
bool hasValidChecksum(long cardNum);
int getNbLength(long number);
int getFirstDigits(long number, int digits);

int main(void)
{
    // Get input
    long cardNb = get_long("Credit card number (eg. 4242424242424242): ");
    // Check card
    string result = checkCard(cardNb);
    // Print result
    printf("%s", result);
}

/**
 * Check wether the card is a valid Visa, Amex or Mastercard credit card
 */
string checkCard(long cardNb)
{
    int length = getNbLength(cardNb);
    int firstDigit = getFirstDigits(cardNb, 1);
    int first2Digits = getFirstDigits(cardNb, 2);

    // Check if has valid checksum, then check the card's type
    if (hasValidChecksum(cardNb))
    {
        // Visa: 13 or 16 digits and starts with 4
        if ((length == 13 || length == 16) && firstDigit == 4)
        {
            return "VISA\n";
        }
        // Amex: 15 digits and starts with 34 or 37
        else if (length == 15 && (first2Digits == 34 || first2Digits == 37))
        {
            return "AMEX\n";
        }
        // Mastercard: 16-digits and starts with 51, 52, 53, 54, or 55
        else if (length == 16 && (first2Digits >= 51 && first2Digits <= 55))
        {
            return "MASTERCARD\n";
        }
    }
    return "INVALID\n";
}

/**
 * Check wether the card's number is compatible with the Luhn’s Algorithm
 */
bool hasValidChecksum(long cardNum)
{
    // 1. Multiply "1 over 2 digits" by 2, starting with the number’s second-to-last digit. Then add those products’ digits together.
    long tempCardNum1 = cardNum / 10;
    int sum1 = 0;
    while (tempCardNum1 > 0)
    {
        int lastDigit = tempCardNum1 % 10; // modulo 10 = get last digit
        int doubled = lastDigit * 2; // multiply by 2
        // extract the products' digits and add them
        if (doubled < 10)
        {
            sum1 = sum1 + doubled;
        }
        else
        {
            sum1 = sum1 + (doubled % 10) + (floor(doubled / 10));
        }
        tempCardNum1 = tempCardNum1 / 100; // remove 2 last digits before looping
    }
    // 2. "Add the sum to the sum of the digits that weren’t multiplied by 2."
    long tempCardNum2 = cardNum;
    int sum2 = 0;
    while (tempCardNum2 > 0)
    {
        int lastDigit = tempCardNum2 % 10;
        sum2 = sum2 + lastDigit;
        tempCardNum2 = tempCardNum2 / 100;
    }
    // 3. If the total’s last digit is 0, the number is valid!
    if ((sum1 + sum2) % 10 == 0)
    {
        return true;
    }
    return false;
}

int getNbLength(long number)
{
    return (number == 0) ? 1 : log10(number) + 1;
}

int getFirstDigits(long number, int digits)
{
    int length = getNbLength(number);
    return number / pow(10, length - digits);
}