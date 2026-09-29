#!/bin/bash

source "$(realpath "${BASH_SOURCE%/*}")/globals"


# prereq scripts to run
colecho red    "......."
colecho yellow "......"
colecho green  "....."
colecho cyan   "...."
colecho blue   "..."
colecho purple ".."
colecho clear  "."
colecho clear  ""

${cwd}/linkup.sh clean >/dev/null 2>&1
${cwd}/linkup.sh

     result=$?
[   $result -eq 0 ] && colecho blue "[+] DEPENDENCY WEB CENTRALIZED" || colecho red "[-] DEPENDENCY WEB FAILED"
[ ! $result -eq 0 ] && exit 1

echo -e "$cyan"
ls -l "$dep_dir" | grep -v '\(\.a\|\.log\|debug\)$' | awk 'NR>1 {printf "    %-24s%-4s%s\n", $9, $10, substr($11, 38)}'
echo -e "$clear"

# gather static libraries and build main program
colecho   cyan ""
colecho   blue "[+] CREATING ${yellow}EXECUTABLE"
colecho   cyan ""
colecho   cyan "    BUILDING [${green}${exe_name}]"
colecho   cyan ""

${cwd}/unify.sh "$exe_target" ""
[ ! $? -eq 0 ] && exit 1

colecho clear ""
colecho blue "[+] statically compiled ${green}[${exe_file##*/}]${blue} is $(ls -l "${exe_file}" | awk '{print $5}') bytes"

colecho green ""
colecho green "[+] DONE"
colecho green ""

exit 0
