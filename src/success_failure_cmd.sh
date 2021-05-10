#!/bin/bash

source /usr/local/share/ptSh/config
test -f ~/.config/ptSh/config && source ~/.config/ptSh/config

ERR=$( ($(echo $1) $(echo ${@:2})) 2>&1 )

while read -r line; do
    if [[ -z $line ]]; then
       echo -ne "${SUCCESS_PREFIX_ESCAPE_CODES}${SUCCESS_PREFIX}\e[0m"
       echo -e "${SUCCESS_MESSAGE_ESCAPE_CODES}${SUCCESS_MESSAGE}\e[0m"
    else
        echo -ne "${ERROR_PREFIX_ESCAPE_CODES}${ERROR_PREFIX}\e[0m"
        echo -ne "${ERROR_MESSAGE_ESCAPE_CODES}"
        echo -n $(echo $line | sed "s/$1: //g" | sed 's/^[^:]*://g')
        echo -e "\e[0m"
    fi
done <<<$ERR
