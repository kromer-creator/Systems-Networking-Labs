#!/bin/bash

# Print one line to terminal window
echo "Hello world"

# Print one line into results file
echo "Hi there" >> handin.txt

# Take in four command line arguments
echo "Positional Parameters"

# Script
echo '$0 = ' $0   
# File name
echo '$1 = ' $1
# Number 1
echo '$2 = ' $2
# Number 2
echo '$3 = ' $3

# Variables for each command line argument
numA=$2
numB=$3
fileName=$1

# Check to see which number is bigger
if [[ $numA -gt $numB ]]; then
    echo "$numA is greater than $numB"
else
    echo "$numA is less than $numB"
fi

# Check to see if file exists
if [[ -f "$fileName" ]]; then
    echo "$fileName exists"
    
else 
    echo "$fileName does not exist"
fi

# Check which files correspond to folders
for file in *.txt; do
    echo "File is ${file}"
done

echo "File: $1, int 1: $2, int 2: $3" >> $1