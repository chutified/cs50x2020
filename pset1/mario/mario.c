# include <stdio.h>
# include <cs50.h>

void printBlock(string s, int n);
void pyramid(int n);
int getOneToEight(void);

int main(void)
{
    int h = getOneToEight();
    pyramid(h);
}

// geoOneToEight promts user for the int between 1 and 8, included.
int getOneToEight(void)
{
    while (true)
    {
        int height = get_int("Height: ");

        // validate input
        if (height >= 1 && height <= 8)
        {
            return height;
        }
    }
}

// pyramid prints the mario pyramid trap
void pyramid(int n)
{
    // range over rows
    for (int i = 1; i <= n; i++)
    {
        printBlock(" ", n - i); // spaces
        printBlock("#", i);     // left hashes
        printBlock(" ", 2);     // gap
        printBlock("#", i);     // right hashes
        printf("\n");
    }
}


// printBlock prints n number of #
void printBlock(string s, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%s", s);
    }
}