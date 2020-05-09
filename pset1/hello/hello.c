#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //Get the name of the user.
    string name = get_string("What is your name?\n");
    //Print a hello message addressing their name in the console.
    printf("Hello, %s!\n", name);
}