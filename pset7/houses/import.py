import csv
from cs50 import SQL
from sys import argv


def main():

    # validate argument lengh
    if len(argv) != 2:
        print("Invalid argument. Usage: python import.py [file.csv]")
        exit(1)

    # connect to db
    db = SQL("sqlite:///students.db")

    # open csv fil
    csvFile = open(argv[1])
    reader = csv.DictReader(csvFile)
    # iterate over rows
    for row in reader:

        # insert data
        f, m, l, h, b = handleRow(row)
        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)", f, m, l, h, b)

    exit(0)


# handleRows returns parsed values from the csv file row
def handleRow(row):

    # get first, middle, last names
    name = row["name"].split(" ")
    f = name[0]
    if len(name) == 2:
        m = None
        l = name[1]
    else:
        m = name[1]
        l = name[2]

    return f, m, l, row["house"], row["birth"]


main()