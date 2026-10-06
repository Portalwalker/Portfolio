#!/bin/bash

source "$(realpath "${BASH_SOURCE%/*}")/globals"

filelist=$(find $(realpath "$top") | grep 'forChrist\(/code\|/compile\)\?/[a-zA-Z0-9_-]*' | grep -v '\.[ao]\|\.sha256\|/other/')

openlist=""
for pattern in "$@"
do
    openlist="${openlist} $(echo "$filelist" | grep "/${pattern}")"
done

"$texteditor" $openlist

exit 0
