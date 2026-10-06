#!/bin/bash

source "$(realpath "${BASH_SOURCE%/*}")/globals"

filelist=$(find $(realpath "$top") | grep "${exe_prfx}\.c$\|\(/code\|/compile\)\?/[a-zA-Z0-9_-]*" | grep -v '\.[ao]\|\.sha256\|/other/\|/dep')

for pattern in "$@"
do
                 openfiles=$(echo "$filelist" | grep "/${pattern}")
    for file in $openfiles
    do
        if [ -f "$file" ]
        then
            openlist="${openlist} ${file}"
        fi
    done
done

     parentpid=$PPID
grandparentpid=$(ps -ef | grep "$parentpid" | awk '{print $3}')

if [ ! "$grandparentpid" = "" ]
then
    texteditor_pid=$(ps -ef | grep "$grandparentpid" | grep "$texteditor" | awk '{print $2}')
fi

if [ ! "$texteditor_pid" = "" ] && echo "$texteditor" | grep -q "kate"
then
    texteditor_pid_flags="--pid"
    "$texteditor" $texteditor_pid_flags "$texteditor_pid" $openlist
else
    "$texteditor" $openlist
fi

exit 0
