#!/bin/bash

source "$(realpath "${BASH_SOURCE%/*}")/globals"

filelist=$(find $(realpath "$top") | grep '.*/bettermake\(/code\|/compile\)\?/[a-zA-Z0-9_-]*')

openlist=""
for pattern in "$@"
do
    openlist="${openlist} $(echo "$filelist" | grep "${pattern}")"
done

"$texteditor" $openlist

exit
