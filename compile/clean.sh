#!/bin/bash

source "$(realpath "${BASH_SOURCE%/*}")/globals"

echo ""
${cwd}/linkup.sh clean

archive_files=$(find "${top}/code" | grep '\.a$')
   hash_files=$(find "${top}/code" | grep "\.sha256$")
 object_files=$(find "${top}/code" | grep '\.o$')
 sha256_files=$(find "${top}/code" | grep '\.sha256$')

RM  $archive_files
RM  $hash_files
RM  $object_files
RM  $sha256_files
RM "$staticA"
RM "$exe_file"


echo -e ""
echo -e "${blue}[+] ${clear}Cleaned ${blue}up!${clear}"
echo -e ""

exit 0
