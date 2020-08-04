# include <stdio.h>
# include <cs50.h>

int main(void)
{
    // ask for input
    string name = get_string("What's your name? ");
    // log the message
    printf("Hi, %s!\n", name);
}