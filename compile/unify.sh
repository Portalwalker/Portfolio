#!/bin/bash

source "$(realpath "${BASH_SOURCE%/*}")/globals"

depth=$(($4))
maxdepth=40

if [ $depth -gt $maxdepth ]
then
    colecho red "[-] Error...max recursion depth reached..."
    exit 1
fi

theLastLastTarget="$3"
theLastTarget="$2"
target="$1"
tName="${target##*/}"
tH=$([ -f "${target}.h" ] && echo "${target}.h")
tC="${target}.c"
tPath="${target%/*}"


            # gather all influencing code into a list
            target_hdeps=$(cat $tH $tC | grep '#include\s\+"dep/[a-zA-Z0-9\-_]\+\.h"' | grep -o '[a-zA-Z0-9\-_]\+\.h' | grep -v "$tName\.h" | sort | uniq)
            target_deps=$(for hdep in $target_hdeps; do echo "${tPath}/${dep_name}/${hdep}"; done)

if [ ! "$tC" = "${exe_target}.c" ]
then
            # get previous code signature
           oldhash=$([ -f "${tPath}/${tName}.sha256" ] && cat "${tPath}/${tName}.sha256")
           # update previous code signature
          holdhash=$(sha256sum $tH $tC $target_deps | awk '{print $1}')
            # get current code signature
           curhash=$(echo "$holdhash")
fi

if [ ! "$oldhash" = "$curhash" ] || [ "$tC" = "${exe_target}.c" ] # if new signature || main file ... build the thing
then
    if [ ! "$target" = "$theLastLastTarget" ] # stops infinite recursion of two cyclically-linked-headers
    then
        for header in $target_hdeps
        do
            hPath="$(realpath "${tPath}/${dep_name}/${header}")"
            cPath="${hPath%.*}.c"
            if [ -f "$cPath" ]
            then
                tobuild="${tobuild} $cPath"
            fi
        done

        for cfile in $tobuild
        do
            ${cwd}/unify.sh "${cfile%.*}" "$target" "$theLastTarget" $((depth + 1))
            [ ! $? -eq 0 ] && colecho red "[-] Tracing from ${target##*/}..." && exit 1
        done
    else
        exit 0 # will cause previous call & previous target to fall into =>'${cwd}/builder.sh $tC' statement below...thus ending infinite recursion
    fi

    ${cwd}/builder.sh "$tC"

    if [ $? -eq 0 ]
    then
        [ ! "$tC" = "${exe_target}.c" ] && echo "$holdhash" > "${tPath}/${tName}.sha256"
    else
        colecho clear ""
        exit 1
    fi
fi

exit 0
