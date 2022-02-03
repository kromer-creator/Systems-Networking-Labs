#!/bin/bash

#switching to absolute path
cd ${1}

#iterate through all files in the directory
for file in `ls * 2> /dev/null`; do

    #rename the file to "username . file extension"
    mv ${file} "$USER.${file##*.}"

done


exit