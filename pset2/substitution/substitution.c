#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_SIZE 26

int main(int argc, string argv[])
{
    bool keyHasRepeatChar = false;
    //Init regex
    regex_t regex;
    int reti;
    //Regex pattern to check against anything other than word characters
    char *Expression = "[^A-Za-z]";
    //Check if an argument has been provided by the user
    if (argc == 2 && strlen(argv[1]) == MAX_SIZE)
    {
        //Check if its a valid key
        //printf("success test %s\n", argv[1]);
        string key = argv[1];

        //TODO check there are nothing other than A-Z or a-z characters in the key (regex)
        reti = regcomp(&regex, Expression, REG_EXTENDED | REG_ICASE);
        if (reti != 0)
        {
            printf("regcomp failed\n");
        }


        /* Execute regular expression */
        reti = regexec(&regex, key, 0, NULL, 0);
        if (!reti)
        {
            //Something other than A-Z or a-z is in the key. Reject key.
            printf("Key must not contain anything other than alphabetical characters.\n");
            return 1;
        }
        else if (reti == REG_NOMATCH)
        {
            //Key has no illegal characters.
            //char keyInArray[] = key;
            //Using an array to check for repeated characters in the key
            int keyRepeatStore[MAX_SIZE];
            int asciiIndex = 65;
            for (int i = 0; i < MAX_SIZE; i++)
            {
                //keyRepeatStore[i][0] = asciiIndex;
                keyRepeatStore[i] = 0;
                //asciiIndex++;
            }

            //Iterate through the key and check against the Array for repeated characters in the key.
            for (int i = 0; i < MAX_SIZE; i++)
            {
                //Regardless of case, convert character to upper case for easy reference.
                char UpperCaseLetter = toupper(key[i]);
                int Letter_In_ASCII = (int)UpperCaseLetter;
                //Because the 2D Array is populated in ASCII in alphabetical order, we can deduce that index[0] will be the reference for A(65), index[1] is B(66) so on and so forth.
                int indexForArray = Letter_In_ASCII - 65;
                if (keyRepeatStore[indexForArray] == 0)
                {
                    //Letter has not appeared before in the key, set value to 1 to mark it as appeared.
                    keyRepeatStore[indexForArray] = 1;
                }
                else
                {
                    //Letter has appeared before in the key, set keyHasRepeatChar value to true and throw an error to the user.
                    keyHasRepeatChar = true;
                    break;
                }
            }
        }
        regfree(&regex);
        if (keyHasRepeatChar)
        {
            //Throw an error to the user indicating that the key has repeat characters
            printf("Key must not contain any repeated characters.\n");
            return 1;
        }
        else
        {
            //TODO when key has no repeated characters
        }
    }
    else if (argc == 2)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
}