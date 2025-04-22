#!/bin/bash

# Directory containing the button images
BUTTONS_DIR="./assets/buttons"

# Check if ImageMagick's convert command is installed
if ! command -v convert &> /dev/null; then
    echo "Error: ImageMagick is not installed. Install it using 'sudo apt-get install imagemagick'."
    exit 1
fi

# Iterate over all files in the buttons directory
for file in "$BUTTONS_DIR"/*; do
    # Check if the file is a PNG
    if [[ "$file" == *.png ]]; then
        echo "Converting $file..."
        # Convert the file to ensure it's a valid PNG
        convert "$file" "$file"
    fi
done

echo "Conversion completed for all PNG files in $BUTTONS_DIR."