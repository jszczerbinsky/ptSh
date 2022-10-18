#!/usr/bin/env bash

echo -e "\x1B[35m"

PREFIX=$(dirname -- "${BASH_SOURCE[0]}")/..

cat $PREFIX/share/ptSh/logo.txt

echo "Let your shell commands look prettier..."
echo -e "\x1B[0m"

echo 
while read -r line; do
    echo -n "$line "
done <<<$(cat $PREFIX/share/ptSh/version.txt)

echo
echo

echo -e "If You enjoy ptSh, give it a \x1B[5m\x1B[93mstar\x1B[0m on github: \x1B[92mhttps://github.com/jszczerbinsky/ptSh\x1B[0m"
echo -e "\x1B[34m"
cat $PREFIX/share/ptSh/LICENSE
echo -e "\x1B[0m"
