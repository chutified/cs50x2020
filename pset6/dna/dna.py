from sys import argv
import csv


def main():

    # check lengh of the arguments
    if len(argv) != 3:
        print("Usage: python dna.py [records.csv] [dna.txt]")
        return 1

    # csv file
    strs, people = handleCSV(argv[1])
    # dna file
    dna = handleTXT(argv[2])

    # count STRs appeareces
    dSTRs = countSTRs(strs, dna)
    print(whoseDNA(people, dSTRs))


# whoseDNA returns the name of the person to tho the DNA matches.
# If nobody matches, it returns "No matches".
def whoseDNA(people, dSTRs):
    for p in people:
        if people[p] == dSTRs:
            return p
    return "No match"


# countSTRs returns the dictionary which holds how many
# times did each certain STR appeared in the given string.
def countSTRs(strs, dna):
    dSTRs = []

    i = 0
    # range over STRs
    for s in strs:
        m = 0
        # range over dna's characters
        for index in range(len(dna)):
            count = strsFromThere(s, dna[index:])
            if count > m:
                m = count

        dSTRs.append(str(m))
        i += 1

    return dSTRs


# strsFromThere counts how many s are from the beggining of the text.
def strsFromThere(s, text):
    count = 0
    while True:
        if text[:len(s)] == s:
            count += 1
            text = text[len(s):]
        else:
            # sequence stoped
            break
    return count


# handleCSV handles the csv file and returns the
# list of STRs and the people and their records.
def handleCSV(fileName):

    # open csv file
    csvFile = open(fileName, "r")
    reader = csv.reader(csvFile)

    # STRs sequences
    strs = []
    # dict[name]: [list of records]
    people = {}

    # range over the file's rows
    for row in reader:

        # first row
        if len(strs) == 0:
            for i in range(len(row)):
                if i == 0:
                    continue
                strs.append(row[i])
            continue

        # add the record
        people[row[0]] = row[1:]

    csvFile.close()
    return strs, people


# handleTXT reads from the given file and returns it.
def handleTXT(fileName):
    f = open(fileName, "r")
    return f.read()


main()
