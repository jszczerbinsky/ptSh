#!/bin/bash

source /usr/share/ptSh/config
test -f ~/.config/ptSh/config && source ~/.config/ptSh/config

ERR=$( (mkdir $@) 2>&1 )

while read -r line; do
    if [[ -z $line ]]; then
       echo -ne "${MKDIR_SUCCESS_PREFIX_ESCAPE_CODES}${MKDIR_SUCCESS_PREFIX}\e[0m"
       echo -e "${MKDIR_SUCCESS_MESSAGE_ESCAPE_CODES}${MKDIR_SUCCESS_MESSAGE}\e[0m"
    else
        echo -ne "${MKDIR_ERROR_PREFIX_ESCAPE_CODES}${MKDIR_ERROR_PREFIX}\e[0m"
        echo -ne "${MKDIR_ERROR_MESSAGE_ESCAPE_CODES}"
        echo -n $(echo $line | sed 's/mkdir: //g' | sed 's/^[^:]*://g')
        echo -e "\e[0m"
    fi
done <<<$ERR
