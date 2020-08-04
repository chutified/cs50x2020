from cs50 import get_string


def main():
    text = get_string("Text: ")
    grade = readibility(text)
    if grade > 15:
        print("Grade 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {grade}")


# readibility determines the grade level of the text.
def readibility(text):

    # define vars
    letters = 0
    words = 1
    sentences = 0

    # count
    for i in range(len(text)):
        ch = text[i]

        # add
        if ch.isalpha():
            letters += 1
        elif ch == " ":
            words += 1
        elif ch in [".", "!", "?"]:
            sentences += 1

    # apply formula
    lx = float(letters) / float(words) * 100.0
    sx = float(sentences) / float(words) * 100.0
    return round(0.0588 * lx - 0.296 * sx - 15.8)


main()
