#!/usr/bin/env bash

source /usr/local/share/ptSh/config
test -f ~/.config/ptSh/config && source ~/.config/ptSh/config

function setval { printf -v "$1" "%s" "$(cat)"; declare -p "$1"; }

eval "$(pwd $@ 2> >(setval err) > >(setval std) )"

if [[ ! -z $err ]]; then
    echo -ne "${ERROR_PREFIX_ESCAPE_CODES}${ERROR_PREFIX}\e[0m"
    echo -e "${ERROR_MESSAGE_ESCAPE_CODES}$err\e[0m"
    exit
fi

if [[ $@ == *"--help"* ]]; then
    cat <<< $std
    exit
fi

IFS='/'

spaces=0

function displayDir(){

    if $2; then
        echo -ne "${PWD_LINE_ESCAPE_CODES}└ \e[0m"
    fi
    if ((PWD_SHOW_DIR_PREFIX == 1)); then
        echo -ne "${DIR_PREFIX_ESCAPE_CODES}${DIR_PREFIX}\e[0m"
    fi
    echo -e "${DIR_NAME_ESCAPE_CODES}$1\e[0m"
}

displayDir "/" false

for word in ${std:1}; do
    for (( i=0; i<spaces; i++ )) do
        echo -n " "
    done

    spaces=$((spaces+PWD_NEXTLINE_MARGIN))

    displayDir $word true
done
