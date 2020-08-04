from cs50 import get_int


def main():
    i = get_my_int()
    pyramids(i)


# get wanted number
def get_my_int():
    while True:
        i = get_int("Height: ")
        if i >= 1 and i <= 8:
            # success
            return i


# print pyramids
def pyramids(n):
    for i in range(n):
        spaces = " " * (n-1-i)
        blocks = "#" * (i+1)
        print(f"{spaces}{blocks}  {blocks}")


main()
