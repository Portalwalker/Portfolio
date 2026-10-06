#!/bin/bash

source "$(realpath "${BASH_SOURCE%/*}")/globals"

${cwd}/linkup.sh clean

archive_files=$(find "${top}/code" | grep '\.a$')
   hash_files=$(find "${top}/code" | grep "\.${hash_sufx}$")
 object_files=$(find "${top}/code" | grep '\.o$')
 sha256_files=$(find "${top}/code" | grep '\.sha256$')

RM  $archive_files
RM  $hash_files
RM  $object_files
RM  $sha256_files
RM "$staticA"
RM "$exe_file"

colecho clear ""
colecho  blue "[+] ${white}Cleaning ${blue}up!"
colecho clear ""

exit 0
