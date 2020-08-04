# include <stdio.h>
# include <cs50.h>
# include <string.h>
# include <ctype.h>
# include <math.h>

int letters(string s);
int words(string s);
int sentences(string s);
int coleman_liau(string s);

int main(void)
{
    // get the grade
    string text = get_string("Text: ");
    int grade = coleman_liau(text);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

// coleman_liau calculates the index of the readibility
int coleman_liau(string s)
{
    // get data
    int ls = letters(s);
    int ws = words(s);
    int ss = sentences(s);
    int len = strlen(s);

    // calculates letters per 100 words
    float lx = (float) ls / (float) ws * 100;
    // callculates sentences per 100 words;
    float sx = (float) ss / (float) ws * 100;

    // apply the formula
    double index = 0.0588 * lx - 0.296 * sx - 15.8;
    int result = (int) round(index);

    return result;
}

// letters calculates the number of letters in the text.
int letters(string s)
{
    int count = 0;

    // range over the string
    for (int i = 0, l = strlen(s); i < l; i++)
    {
        if (isalpha(s[i]))
        {
            count++;
        }
    }

    return count;
}

// words counts the number of the words in the text.
int words(string s)
{
    int count = 0;

    // range over the string
    for (int i = 0, l = strlen(s); i < l; i++)
    {
        if (s[i] == ' ')
        {
            count++;
        }
    }

    return count + 1;
}

// sentences counts the number of the sentences in the text.
int sentences(string s)
{
    int count = 0;

    // range over the string
    for (int i = 0, l = strlen(s); i < l; i++)
    {
        if (s[i] == '.' || s[i] == '!' || s[i] == '?')
        {
            count++;
        }
    }

    return count;
}