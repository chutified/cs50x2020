# include <stdio.h>
# include <string.h>
# include <cs50.h>
# include <ctype.h>

# include <math.h>

bool unique(string s, char ch);
bool validateKey(string key);
string cipher(string key, string text);
int findPos(string s, char ch);

int main(int argc, string argv[])
{
    // handle the key
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    if ((strlen(argv[1]) != 26) || (validateKey(argv[1]) == false))
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }
    string key = argv[1];

    // log the plaintext
    string text = get_string("plaintext:");

    string encrypted = cipher(key, text);
    printf("ciphertext:%s\n", encrypted);

    return 0;
}

// cipher returns encrypted text with the key.
string cipher(string key, string text)
{
    char result[strlen(text)];

    // encrypting into the results
    for (int i = 0, l = strlen(text); i < l; i++)
    {
        if (isalpha(text[i]))
        {
            bool isLow = islower(text[i]);

            char ch = toupper(text[i]);
            int pos = findPos("ABCDEFGHIJKLMNOPQRSTUVWXYZ", ch);

            char a = key[pos];
            // switch the casing back
            if (isLow)
            {
                a = tolower(a);
            }
            else
            {
                a = toupper(a);
            }

            result[i] = a;
        }
        else
        {
            result[i] = text[i];
        }
    }

    string r = result;
    return r;
}

// findPos returns the index of the first located char in the string.
int findPos(string s, char ch)
{
    for (int i = 0, l = strlen(s); i < l; i++)
    {
        if (ch == s[i])
        {
            // char founded
            return i;
        }
    }
    return -1;
}

// validateKey checks if the key is valid.
bool validateKey(string key)
{
    int len = strlen(key);

    // check lengh
    if (len != 26)
    {
        return false;
    }

    // check for duplicates
    for (int i = 0; i < len; i++)
    {
        if (unique(key, key[i]) == false)
        {
            return false;
        }
        if (isalpha(key[i]) == 0)
        {
            return false;
        }
    }

    return true;
}

// unique checks if the character is uniwue in the text.
bool unique(string s, char ch)
{
    int l = strlen(s);

    bool has = false;
    // range over the string
    for (int i = 0; i < l; i++)
    {
        if (ch == s[i])
        {
            // switch
            if (has)
            {
                return false;
            }
            has = true;
        }
    }

    return true;
}