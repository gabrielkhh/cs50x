#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    int *array;
    size_t used;
    size_t size;
} Array;

string checkCardType(long long cardNumber);
bool checkIsValidLength(long long cardNumber);
bool checkLuhnAlgorithm(long long cardNumber);
void initArray(Array *a, size_t initialSize);
void insertArray(Array *a, int element, int count);

int main(void)
{
    long long cardNumber;
    string cardType;
    //Run the prompt as long as the given number is less than 10 quadrillion (ensures that the given number is at most 16 digits long).
    do
    {
        //Ask the user for a credit card number to check while making sure the number given is valid.
        cardNumber = get_long("Card Number: ");
    }
    while (!(cardNumber < 10000000000000000));

    //Check if card number is of valid length otherwise return INVALID message to the user.
    if (checkIsValidLength(cardNumber))
    {
        //Check if the first 2 numbers is within the valid range of 34, 37, 51, 52, 53, 54, 55 and 4x.
        cardType = checkCardType(cardNumber);
        if (strcmp(cardType, "INVALID") != 0)
        {
            //Check number authenticity.
            bool isTrue = checkLuhnAlgorithm(cardNumber);
            if (isTrue)
            {
                printf("%s\n", cardType);
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            //Return Invalid message to user.
            printf("%s\n", cardType);
        }
    }
    else
    {
        //Return INVALID.
        printf("INVALID\n");
    }
}

string checkCardType(long long cardNumber)
{
    //Variable for storing the type of the card (VISA, MASTERCARD etc).
    string cardType = "INVALID";
    //Variable for storing the first 2 digits of the card number within this function.
    int value;
    //Using if statements to retrieve the first 2 digits of the card number based on the different possible permitted lengths of the card number.
    if (cardNumber > 999999999999 && cardNumber < 10000000000000)
    {
        //Card Number is 13 digits long for VISA
        value = cardNumber / 100000000000;
        //Check if the first digit starts with 4
        if (value > 39 && value < 50)
        {
            cardType = "VISA";
        }
        return cardType;
    }
    else if (cardNumber > 9999999999999 && cardNumber < 100000000000000)
    {
        //Card Number is 14 digits long and no valid payment service uses 14-digit numbers so return invalid.
        return cardType;
    }
    else if (cardNumber > 99999999999999 && cardNumber < 1000000000000000)
    {
        //Card Number is 15 digits long for AMEX.
        value = cardNumber / 10000000000000;
        //Check if the first 2 digits is either 34 or 37.
        if (value == 34 || value == 37)
        {
            cardType = "AMEX";
        }
        return cardType;
    }
    else if (cardNumber > 999999999999999 && cardNumber < 10000000000000000)
    {
        //Card Number is 16 digits long for MASTERCARD & VISA.
        value = cardNumber / 100000000000000;
        //Check if the first 2 digits is either 51 through 55 or 4x.
        if (value > 50 && value < 56)
        {
            cardType = "MASTERCARD";
        }
        else if (value > 39 && value < 50)
        {
            cardType = "VISA";
        }
        return cardType;
    }
    return cardType;
}

bool checkIsValidLength(long long cardNumber)
{
    //Check if the number provided is at least 13, 15 or 16 digits long since the min length is 13 digits long, 14 isn't a valid card number length and 16 is the max.
    if ((cardNumber > 999999999999 && cardNumber < 100000000000000) || (cardNumber > 99999999999999 && cardNumber < 10000000000000000))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool checkLuhnAlgorithm(long long cardNumber)
{
    //Intialise various variables to be used internally in this function
    int sum1 = 0;
    //Intialise var to store the length of card number
    size_t length;
    //Initialise array of digits to store card number in reversed order
    Array cardNumberReversed;
    initArray(&cardNumberReversed, 1);
    //Intialise counter
    int count = 0;
    //retrieve each digit from the card number
    //retrieves it from last digit to first.
    long long value = cardNumber;
    while (value > 0)
    {
        int digit = value % 10;
        insertArray(&cardNumberReversed, digit, count);
        count++;
        value /= 10;
    }

    //Find out the number of digits of the card number and process with the relevant logic.
    //Use count as an indicator to retrieve the correct value from the array.
    count -= 2;
    //Get and store the summation of digits starting from the second last digit of card number.
    for (int i = 1; i < (int)cardNumberReversed.used; i += 2)
    {
        int MultiplyResult = cardNumberReversed.array[i] * 2;
        //Breakdown the products' digits and add them individually.
        if (MultiplyResult > 9)
        {
            //Get the first digit and add it to sum1
            sum1 += (int)(MultiplyResult / 10);

            //Get the second digit and add it to sum1
            sum1 += MultiplyResult % 10;
        }
        else
        {
            //Because the result is a single digit, there is no need to use modulo or division to breakdown the digits.
            sum1 += MultiplyResult;
        }
    }

    //Get and store the summation of the remainder digits of the card number that were omitted from the previous for loop.
    for (int i = 0; i < (int)cardNumberReversed.used; i += 2)
    {
        int digit = cardNumberReversed.array[i];
        sum1 += digit;
    }

    //Find out if the last digit is zero from the results.
    int result = sum1 % 10;
    if (result == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void initArray(Array *a, size_t initialSize)
{
    a->array = (int *)malloc(initialSize * sizeof(int));
    a->used = 0;
    a->size = initialSize;
}

void insertArray(Array *a, int element, int count)
{
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size
    a->size = count + 1;
    a->array = (int *)realloc(a->array, a->size * sizeof(int));
    a->array[a->used++] = element;
}