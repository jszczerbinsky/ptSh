#!/usr/bin/env bash

echo -e "\e[35m"

# TODO /usr/local does not exist on non-FHS distros like NixOS and Guix
# put this somewhere else
cat /usr/local/share/ptSh/logo.txt

echo "Let your shell commands look prettier..."
echo -e "\e[0m"

echo 
while read -r line; do
    echo -n "$line "
done <<<$(cat /usr/local/share/ptSh/version.txt)

echo
echo

echo -e "If You enjoy ptSh, give it a \e[5m\e[93mstar\e[0m on github: \e[92mhttps://github.com/jszczerbinsky/ptSh\e[0m"
echo -e "\e[34m"
cat /usr/local/share/ptSh/LICENSE
echo -e "\e[0m"
