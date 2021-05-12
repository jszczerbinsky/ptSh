#!/bin/bash

source /usr/local/share/ptSh/config
test -f ~/.config/ptSh/config && source ~/.config/ptSh/config

if [[ -z $1 ]] || [[ $1 == "--help" ]]; then
    cat <<< "$(touch --help)"
    exit
fi

args=""
names=()

read -ra words <<<"$@"

for val in "${words[@]}"; do
    if [[ $val == "-"* ]]; then
        args="$args $val"
    else
        names=(${names[@]} $val)
    fi
done

for name in "${names[@]}"; do
    echo -ne "${FILE_PREFIX_ESCAPE_CODES}${FILE_PREFIX}\e[0m"
    echo -e "${FILE_NAME_ESCAPE_CODES}$name\e[0m"
    echo -n "└"

    if [[ -z $args ]] && readlink -e $name >/dev/null; then
        ERR="File exists"
    else
        ERR=$(touch $args $name 2>&1)
    fi

    while read -r line; do
        if [[ -z $line ]]; then
            echo -ne "${SUCCESS_PREFIX_ESCAPE_CODES}${SUCCESS_PREFIX}\e[0m"
            echo -e "${SUCCESS_MESSAGE_ESCAPE_CODES}${SUCCESS_MESSAGE}\e[0m"
        else
            echo -ne "${ERROR_PREFIX_ESCAPE_CODES}${ERROR_PREFIX}\e[0m"
            echo -ne "${ERROR_MESSAGE_ESCAPE_CODES}"
            echo -n "$(echo "$line" | sed "s/$1: //g" | sed 's/^[^:]*://g')"
            echo -e "\e[0m"
        fi
    done <<< "$ERR"
done
