#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //initialize height
    int height;
    //Get the height from the user while ensuring the input is between 1 and 8 inclusive.
    do
    {
        height = get_int("Height: ");
    }
    while (!(height <= 8 && height > 0));

    //Initialize counter values hard coded strings to be used.
    int counter = height - 1;
    int count = 1;
    char space = ' ';
    //print the resulting pyramid based on the given height
    for (int i = 0; i < height; i++)
    {
        //Print the corresponding spaces so the blocks (#) for the left side pyramid are aligned for the current row.
        while (counter > 0)
        {
            printf("%c", space);
            counter--;
        };

        //Print the number of blocks (#) for the left side of the pyramid in the current row.
        for (int j = 0; j < count; j++)
        {
            printf("#");
        }

        //Print the double spacing before the next pyramid
        printf("  ");

        //Print the right side of the pyramid
        for (int j = 0; j < count; j++)
        {
            printf("#");
        }
        //Break to the next line after the current row is printed.
        printf("\n");
        //Update counter values.
        count++;
        counter = height - (i + 2);
    }

}