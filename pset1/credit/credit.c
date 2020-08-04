# include <stdio.h>
# include <cs50.h>

long getNumber(void);
int digitSum(int n);
int sum1(long n);
int sum2(long number);
bool validate(long number);
int getFirstTwo(long num);
int getLen(long num);
string getCard(long n);

int main(void)
{
    long num = getNumber();
    if (validate(num))
    {
        printf("%s\n", getCard(num));
    }
    else
    {
        printf("INVALID\n");
    }
}

// getCard returns the credit cards company.
string getCard(long n)
{
    int ft = getFirstTwo(n);
    int l = getLen(n);

    // search for the company
    if (l == 15 && (ft == 34 || ft == 37))
    {
        return "AMEX";
    }
    if (l == 16 && (ft >= 51 && ft <= 55))
    {
        return "MASTERCARD";
    }
    if (l == 13 || l == 16)
    {
        if (ft / 10 == 4)
        {
            return "VISA";
        }
    }
    return "INVALID";
}

// getLen returns the lengh of the number
int getLen(long num)
{
    int len = 0;
    while (num > 0)
    {
        num /= 10;
        len++;
    }
    return len;
}

// getFirstTwo returns first two digits of the number.
int getFirstTwo(long num)
{
    while (num > 99)
    {
        num /= 10;
    }

    // get digits
    int second = num % 10;
    num /= 10;
    int first = num % 10;

    // return
    int result = first * 10 + second;
    return result;
}

// getNumber prompts the user for the positive credit number.
long getNumber(void)
{
    while (true)
    {
        long number = get_long("Number: ");

        // check the number
        if (number > 0)
        {
            return number;
        }
    }
}

// valitdate validates if the number is a lived credit card number.
bool validate(long number)
{
    int sum = sum1(number) + sum2(number);
    if (sum % 10 == 0)
    {
        return true;
    }
    return false;
}

// sum1 multiplies every other digit by 2, starting with the number’s second
// to-last digit, and then add those products’ digits together.
int sum1(long n)
{
    // prepare result to return
    int sum = 0;

    while (n > 0)
    {
        // remove the first digit
        int first = n % 10;
        n -= first;
        n /= 10;

        // handle the second number
        int mod = n % 10;
        n -= mod;
        n /= 10;

        // add the number to the result
        sum += digitSum(mod * 2);
    }

    return sum;
}


// sum2 calculates the sum of the odd digits.
int sum2(long n)
{
    // prepare result to return
    int sum = 0;

    while (n > 0)
    {
        // handle the first number
        int mod = n % 10;
        n -= mod;
        n /= 10;

        // add the number to the result
        sum += mod;

        // remove the second digit
        int first = n % 10;
        n -= first;
        n /= 10;
    }

    return sum;
}

// digitSum calculates the sum of the number's digit.
int digitSum(int n)
{
    // prepare th result
    int sum = 0;

    while (n > 0)
    {
        // remove the first digit
        int first = n % 10;
        n -= first;
        n /= 10;

        // add to the result
        sum += first;
    }

    return sum;
}