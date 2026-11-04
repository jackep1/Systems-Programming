#!/bin/bash

image_directory=$1
output_directory=$2

bash Scripts/build.sh
chmod +x Scripts/build.sh
chmod +x split
chmod +x rotate
chmod +x dimensions
chmod +x crop

for img in $image_directory/*.tga
do
    if [ -f "$img" ]; then
        img_name=$(basename "$img" .tga)

        ./split "$img";
        mv "$image_directory/$img_name [r].tga" $output_directory
        mv "$image_directory/$img_name [g].tga" $output_directory
        mv "$image_directory/$img_name [b].tga" $output_directory

        ./rotate -l "$img";
        mv "$image_directory/$img_name [L].tga" $output_directory

        ./rotate -r "$img";
        mv "$image_directory/$img_name [R].tga" $output_directory
    else
        echo "No .tga files found in $image_directory"
    fi
done