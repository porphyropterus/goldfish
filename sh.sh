#!/bin/bash

# Check if a directory is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

DIRECTORY="$1"

# Check if the directory exists
if [ ! -d "$DIRECTORY" ]; then
    echo "Error: Directory $DIRECTORY does not exist."
    exit 1
fi

# Create a tarball of all files in the directory
OUTPUT_FILE="$DIRECTORY/combined_files.tar"
tar -cvf "$OUTPUT_FILE" -C "$DIRECTORY" .

# Show the original size of the tarball
ORIGINAL_SIZE=$(stat --printf="%s" "$OUTPUT_FILE")
echo "Original tarball size: $ORIGINAL_SIZE bytes"

# Compress the tarball with gzip -9
gzip -9 "$OUTPUT_FILE"

# Show the compressed tarball size
COMPRESSED_FILE="$OUTPUT_FILE.gz"
COMPRESSED_SIZE=$(stat --printf="%s" "$COMPRESSED_FILE")
echo "Compressed tarball size: $COMPRESSED_SIZE bytes"

# Delete all original files except the compressed tarball
find "$DIRECTORY" -type f ! -name "$(basename \"$COMPRESSED_FILE\")" -exec rm {} +

echo "All original files deleted except $COMPRESSED_FILE"
