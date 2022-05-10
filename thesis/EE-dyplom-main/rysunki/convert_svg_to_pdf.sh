#!/usr/bin/env bash

input_file=$1
input_file=$(readlink -f $input_file)

output_file=$2

[ ! -f $input_file ] && echo "File $1 doesn't exist" && exit 1
[ -z $output_file ] && echo "Output filename must be defined!" && exit 1
[ -z $(which inkscape) ] && echo 'No inkscape instance detected' && exit 1

if [[ ! $output_file =~ "*.pdf" ]]; then
	echo "Adding PDF extension to: $output_file..."
	output_file="$output_file.pdf"
fi
echo "Converting $1 into $output_file"

inkscape -z --export-pdf=$output_file $input_file
