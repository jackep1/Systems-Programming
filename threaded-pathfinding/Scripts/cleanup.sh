#!/bin/bash

# Get scrap directory path from args
scrap_dir_path=$1
# Get base name from args
base_name=$2
# Get output file path from args
output_file_path=$3

# Remove all scrap directories for versions 1-3 if they exist
if [ -d "${scrap_dir_path}1" ]; then
    rm -r "${scrap_dir_path}1"
fi
if [ -d "${scrap_dir_path}2" ]; then
    rm -r "${scrap_dir_path}2"
fi
if [ -d "${scrap_dir_path}3" ]; then
    rm -r "${scrap_dir_path}3"
fi

# Remove all executables for versions 1-3 if they exist
if [ -f "${base_name}1" ]; then
    rm "${base_name}1"
fi
if [ -f "${base_name}2" ]; then
    rm "${base_name}2"
fi
if [ -f "${base_name}3" ]; then
    rm "${base_name}3"
fi

# Remove all output files for versions 1-3 if they exist
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

if [ -f "${output_dir}${output_stem}1${output_extension}" ]; then
    rm "${output_dir}${output_stem}1${output_extension}"
fi
if [ -f "${output_dir}${output_stem}2${output_extension}" ]; then
    rm "${output_dir}${output_stem}2${output_extension}"
fi
if [ -f "${output_dir}${output_stem}3${output_extension}" ]; then
    rm "${output_dir}${output_stem}3${output_extension}"
fi