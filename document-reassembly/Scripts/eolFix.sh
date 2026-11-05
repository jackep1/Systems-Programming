#!/bin/bash

# Check if the number of arguments is correct
if [ "$#" -ne 3 ]; then
    echo "Illegal number of parameters"
    exit 1
fi

# Check if the input directory exists
if [ ! -d "$1" ]; then
    echo "Input directory does not exist"
    exit 1
fi

# Check if the scrap folder exists, if not create it
if [ ! -d "$2" ]; then
    mkdir "$2"
fi

# Check if the output directory exists, if not create it
if [ ! -d "$3" ]; then
    mkdir "$3"
fi

# Loop through all files in the input directory
for file in "$1"/*; do
    # Check if the file is a regular file
    if [ -f "$file" ]; then
        # Create a temporary file in the scrap folder
        tempFile="$2/$(basename "$file")"
        touch "$tempFile"
        original="$2/$(basename "$file")-o-ascii.txt"
        touch "$original"
        newascii="$2/$(basename "$file")-n-ascii.txt"
        touch "$newascii"

        # Loop through all lines in the file
        while IFS= read -r line; do
            # Check if the line uses Windows line endings, if not, add the line as-is
            if [[ "$line" != *$'\r' ]]; then
                echo "$line" >> "$tempFile"
                echo "$line" | od -An -vtu1 >> "$original"
                echo "$line" | od -An -vtu1 >> "$newascii"
                continue
            fi
            # Add the original line's ascii values to the original ascii file
            echo "$line" | od -An -vtu1 >> "$original"
            # Replace Windows line endings with Unix line endings
            echo "$line" | tr -d '\r' >> "$tempFile"
            # Add the altered line to the new ascii file
            echo "$line" | tr -d '\r' | od -An -vtu1 >> "$newascii"
        done < "$file"

        # Copy the temporary file to the output directory
        cp "$tempFile" "$3/$(basename "$file")"

        # Remove the temporary file
        rm "$tempFile"
    fi
done

# Remove the scrap folder
rm -r "$2"