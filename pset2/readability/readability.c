#include <cs50.h>
#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>

int match(regex_t *pexp, char *sz);
int getColemanLiauIndex(int letterCount, int sentenceCount, int wordCount);

int main(void)
{
    //Init regex
    regex_t regex;
    int reti;
    //Counter values to be used later.
    int numberOfLetters = 0;
    int numberOfWords = 0;
    int numberOfSentences = 0;
    //Variable to store the output message
    string outputMsg = "An unknown problem occurred.";
    //Get the text input from the user.
    string text = get_string("Text : ");

    //Regular Expressions Conditions
    char *LetterExpression = "[a-zA-Z0-9]";
    char *WordExpression = "[a-zA-Z0-9'_-]+";
    char *SentenceExpression = "[!?\\.]";

    //Count the number of letters
    reti = regcomp(&regex, LetterExpression, REG_EXTENDED | REG_ICASE);
    if (reti != 0)
    {
        printf("regcomp failed\n");
    }
    numberOfLetters = match(&regex, text);
    regfree(&regex);

    //Count the number of words
    reti = regcomp(&regex, WordExpression, REG_EXTENDED | REG_ICASE);
    if (reti != 0)
    {
        printf("regcomp failed\n");
    }
    numberOfWords = match(&regex, text);
    regfree(&regex);

    //Count the number of sentences
    reti = regcomp(&regex, SentenceExpression, REG_EXTENDED | REG_ICASE);
    if (reti != 0)
    {
        printf("regcomp failed\n");
    }
    numberOfSentences = match(&regex, text);
    regfree(&regex);

    //Get the index value of the Coleman-Liau Index
    int colemanLiauIndex = getColemanLiauIndex(numberOfLetters, numberOfSentences, numberOfWords);

    if (colemanLiauIndex < 1)
    {
        outputMsg = "Before Grade 1";
        printf("%s\n", outputMsg);
    }
    else if (colemanLiauIndex >= 16)
    {
        outputMsg = "Grade 16+";
        printf("%s\n", outputMsg);
    }
    else
    {
        //concatenate the index with the word Grade.
        char index[8];
        sprintf(index, "Grade %i", colemanLiauIndex);
        puts(index);
        outputMsg = index;
    }
}

int match(regex_t *pexp, char *sz)
{
    // we just need the whole string match in this example
    regmatch_t whole_match;

    int count = 0;

    // we store the eflags in a variable, so that we can make
    // ^ match the first time, but not for subsequent regexecs
    int eflags = 0;
    int match = 0;
    size_t offset = 0;
    size_t length = strlen(sz);

    while (regexec(pexp, sz + offset, 1, &whole_match, eflags) == 0)
    {
        // do not let ^ match again.
        eflags = REG_NOTBOL;
        match = 1;
        /*printf("range %zd - %zd matches\n",
               offset + whole_match.rm_so,
               offset + whole_match.rm_eo);*/

        count++;
        // increase the starting offset
        offset += whole_match.rm_eo;

        // a match can be a zero-length match, we must not fail
        // to advance the pointer, or we'd have an infinite loop!
        if (whole_match.rm_so == whole_match.rm_eo)
        {
            offset += 1;
        }

        // break the loop if we've consumed all characters. Note
        // that we run once for terminating null, to let
        // a zero-length match occur at the end of the string.
        if (offset > length)
        {
            break;
        }
    }
    if (! match)
    {
        //printf("\"%s\" does not contain a match\n", sz);
    }
    return count;
}

int getColemanLiauIndex(int letterCount, int sentenceCount, int wordCount)
{
    float multiplier = (float)wordCount / 100;
    float avgLetters = (float)letterCount / multiplier;
    float avgSentences = (float)sentenceCount / multiplier;
    float result = round(0.0588 * avgLetters - 0.296 * avgSentences - 15.8);
    int index = result;
    return index;
}