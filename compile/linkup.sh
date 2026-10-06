#!/bin/bash

source "$(realpath "${BASH_SOURCE%/*}")/globals"


# COLLECT DIRECTORIES WITH SOURCE CODE
  code_files=$(find "${top}/code" | grep '\.[ch]$')
code_headers=$(echo $code_files | tr -s ' ' '\n' | grep '\.h$')
   code_dirs=""

for file in $code_files
do
     code_dirs="${file%/*} ${code_dirs}"
done
code_dirs=$(echo "$code_dirs" | tr -s ' ' '\n' | sort | uniq)


# LINK HEADERS & DIRECTORIES  OR  CLEAN UP
if [ ! "$1" = "clean" ]
then

    # CREATE SELF-POINTING LINKS (NECESSARY DURING COMPILATION)
    mkdir -p "$dep_dir"
    [ ! $? -eq 0 ] && exit 1
    ln -sfn  "$dep_dir" "${top}/${dep_name}"
    [ ! $? -eq 0 ] && exit 1
    ln -sfn  "$dep_dir" "${dep_dir}/${dep_name}"
    [ ! $? -eq 0 ] && exit 1


    # CONNECT EVERY DIRECTORY WITH SOURCE CODE
    for dir in $code_dirs
    do
                              dep_link="${dir}/${dep_name}"
         ln -sfn "$dep_dir" "$dep_link"
         [ ! $? -eq 0 ] && exit 1
    done


    # CREATE LINKS FOR ALL HEADERS
    for hdr in $code_headers
    do
                          hdr_link="${dep_dir}/${hdr##*/}"
         ln -sfn "$hdr" "$hdr_link"
         [ ! $? -eq 0 ] && exit 1
    done

else # CLEAN UP

    RM "${top}/${dep_name}"
    RM  ${dep_dir}/*

    for dir in $code_dirs
    do
        RM "${dir}/${dep_name}"
    done

fi

exit 0
