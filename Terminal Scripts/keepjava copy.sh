#!/bin/bash

# Change directory to absolute directory from parameter
cd "$PWD/$1"

# Remove all files not ending in .java, with user permission
find "$PWD" -type f -not -name '*.java' -exec rm -i {} \;