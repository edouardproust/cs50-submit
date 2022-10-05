# Loop: Prompt the user until the input is valid
while True:
    # Prompt user and check that input is an integer
    try:
        height = int(input("Height: "))
    except ValueError:
        continue
    # Check that the integer in between 1 and 8 included
    if height > 0 and height < 9:
        # Build pyramid
        for h in range(height):
            b = h + 1  # blocks number
            s = height - b  # spaces number
            # Display each line
            print(" "*s + "#"*b + "  " + "#"*b)
        break
