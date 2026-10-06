#!/bin/bash

source "$(realpath "${BASH_SOURCE%/*}")/globals"

filelist=$(find $(realpath "$top") | grep '/[a-zA-Z0-9_-]*' | grep -v '\.[ao]\|\.git\|\.sha256')

openlist=""
for pattern in "$@"
do
    openlist="${openlist} $(echo "$filelist" | grep "$pattern")"

done

"$texteditor" $openlist

exit 0
