#!/bin/bash

source "$(realpath "${BASH_SOURCE%/*}")/globals"


# prereq scripts to run
colecho red    "......."
colecho gold   "......"
colecho green  "....."
colecho cyan   "...."
colecho blue   "..."
colecho purple ".."
colecho clear  "."
colecho clear  ""

${cwd}/linkup.sh clean >/dev/null 2>&1
${cwd}/linkup.sh

     result=$?
[   $result -eq 0 ] && colecho blue "[+] DEPENDENCY WEB CENTRALIZED" || echo "/"'!'"\\DEPENDENCY WEB FAILED"
[ ! $result -eq 0 ] && exit 1

echo -e "$cyan"
ls -l "$dep_dir" | grep -v '\(\.a\|debug\)$' | awk 'NR>1 {printf "    %-24s%-4s%s\n", $9, $10, substr($11, 38)}'
echo -e "$clear"

# gather static libraries and build main program
colecho   cyan ""
colecho   blue "[+] CREATING ${gold}EXECUTABLE"
colecho   cyan ""
colecho   cyan "    BUILDING [${green}${exe_name}]"
colecho   cyan ""

${cwd}/unify.sh "$exe_target" ""
[ ! $? -eq 0 ] && exit 1

colecho clear ""
colecho blue "[+] statically compiled ${green}[${exe_file##*/}]${blue} is $(ls -l "${exe_file}" | awk '{print $5}') bytes"

colecho green ""
colecho blue  "[+] ${green}This project is dedicated to \033[1;37mJesus Christ"
colecho clear ""
colecho clear "                                 \033[1;37mGod\033[0m \033[32mamong \033[31mmen\033[1;37m, \033[1;37mSavior\033[0m \033[33mto \033[34mheroes\033[1;37m, \033[1;37mKing\033[0m \033[1;33mover\033[0m ${blue}all\033[1;37m.\033[0m"
colecho clear ""

exit 0
