#!/bin/bash

source "$(realpath "${BASH_SOURCE%/*}")/globals"


    # flags
       align_options="-mstackrealign"
    compiler_options="-std=gnu99 -static"
      depend_options="-nostdlib -nodefaultlibs -nostartfiles -fno-builtin -ffreestanding -Wl,-e,_start"
        form_options="-pie -fPIE"
       stack_options="-fomit-frame-pointer -fno-stack-protector -mno-red-zone"
        warn_options="-Wno-return-mismatch"
      memory_options="-fno-tree-dse -fcf-protection=none"

    if [ -f "${dep_dir}/debug" ]
    then
        optimize_options="-g -D DEBUG"
    else
        optimize_options="-Os -ffunction-sections -fdata-sections -Wl,--gc-sections"
    fi

    internal_options="${align_options} ${compiler_options} ${stack_options} ${memory_options} ${depend_options} ${form_options} ${warn_options} ${optimize_options}"


     # globals
     cfile="$1"
     tPath="${cfile%/*}"
     tName="${cfile##*/}"
     tName="${tName%.*}"
     oTemp="${tPath}/otemp"
    target="${cfile%.*}"
 wkspcsave="$PWD"


    # create, enter, and clean build space
    mkdir -p "$oTemp"
    [ ! $? -eq 0 ] && exit 1

    cd "$oTemp"
    RM *.o >/dev/null 2>&1


    if [ ! "$cfile" = "${exe_target}.c" ]
    then
            # compile this library source code into an object file
            gcc ${internal_options} -o "${oTemp}/${tName}.o" -c "${cfile}"
            if [ ! $? -eq 0 ]
            then
                colecho clear ""
                colecho red "gcc \"${internal_options}\" -o \"${oTemp}/${tName}.o\" -c \"${cfile}\""
                exit 1
            else
                colecho cyan "gcc \"${internal_options}\" -o \"${oTemp}/${tName}.o\" -c \"${cfile}\""
            fi

            # make an archive of this object file
            ar rcs "${tPath}/${tName}.a" "${oTemp}/${tName}.o"

    else

            # gather all .a files into fellowship.a
            afiles=$(find "$top_code_dir"| grep '\.a$')
            for alib in $afiles; do ar -x $alib; done

            # create ultimate static library 'fellowship.a' from all unpacked .o (object) files
            ar rcs "$staticA" $(find "$oTemp" | grep '\.o')

            # compile the main binary
            gcc ${internal_options} -o "${exe_file}" "${cfile}" "$staticA"

            if [ ! $? -eq 0 ]
            then
                colecho red "gcc \"${internal_options}\" -o \"${exe_file}\" \"${cfile}\" \"${staticA}\""
                colecho clear ""
                exit 1
            else
                colecho green "gcc \"${internal_options}\" -o \"${exe_file}\" \"${cfile}\" \"${staticA}\""
            fi

            # make it executable
            chmod +x "$exe_file"

    fi

    RM ${oTemp}/* >/dev/null 2>&1
    RM "$oTemp"   >/dev/null 2>&1

    # restore original working space
    cd "$wkspcsave"

exit 0
