from cs50 import SQL
from sys import argv


def main():

    # validate argument lengh
    if len(argv) != 2:
        print("Invalid argument. Usage: python roster.py [HOUSE]")
        exit(1)

    # connect to db
    db = SQL("sqlite:///students.db")

    rows = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", argv[1])
    # iterate over db rows
    for row in rows:

        # check middle name
        if row["middle"] == None:
            print(f"{row['first']} {row['last']}, born {row['birth']}")
        else:
            print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")

    exit(0)


main()