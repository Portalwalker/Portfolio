#!/bin/bash


source "$(realpath "${BASH_SOURCE%/*}")/globals"


pertinent="$(find "$top_code_dir" | grep '[a-zA-Z0-9\-_]\+\.[hc]$')"
for pattern in "$@"
do
    for path in $pertinent
    do
                 matches=$(grep -n "$pattern" "$path")
        if [ ! "$matches" = "" ]
        then
            echo ""
            echo "[+] ${path##*/} matches"
            echo ""
            echo -e "\e[36m""$matches""\e[0m"
        fi
    done
done

echo ""

exit
