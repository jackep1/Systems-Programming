#!/bin/bash

# Gather args
base_name=$1
grid_file_path=$2
node_file_path=$3
start_idx=$4
end_idx=$5
scrap_dir_path=$6
output_file_path=$7

case "$output_file_path" in
    */*)
        output_dir="${output_file_path%/*}/"
        output_name="${output_file_path##*/}"
        ;;
    *)
        output_dir=""
        output_name="$output_file_path"
        ;;
esac

case "$output_name" in
    *.*)
        output_stem="${output_name%.*}"
        output_extension=".${output_name##*.}"
        ;;
    *)
        output_stem="$output_name"
        output_extension=""
        ;;
esac

# Build all executables
chmod +x ./Scripts/build.sh
./Scripts/build.sh "$base_name"

# Version names
v1="${base_name}1"
v2="${base_name}2"
v3="${base_name}3"

# If scrap directories for versions 1-3 does not exist, create it
if [ ! -d "${scrap_dir_path}1" ]; then
    mkdir "${scrap_dir_path}1"
fi
if [ ! -d "${scrap_dir_path}2" ]; then
    mkdir "${scrap_dir_path}2"
fi
if [ ! -d "${scrap_dir_path}3" ]; then
    mkdir "${scrap_dir_path}3"
fi

# Run all executables
echo "Running $v1"
./"$v1" "$grid_file_path" "$node_file_path" "$start_idx" "$end_idx" "${scrap_dir_path}1" "${output_dir}${output_stem}1${output_extension}"
echo "Running $v2"
./"$v2" "$grid_file_path" "$node_file_path" "$start_idx" "$end_idx" "${scrap_dir_path}2" "${output_dir}${output_stem}2${output_extension}"
echo "Running $v3"
./"$v3" "$grid_file_path" "$node_file_path" "$start_idx" "$end_idx" "${scrap_dir_path}3" "${output_dir}${output_stem}3${output_extension}"
