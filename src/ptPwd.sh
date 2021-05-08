#!/bin/bash

source /usr/share/ptSh/config
test -f ~/.config/ptSh/config && source ~/.config/ptSh/config

PWD=$(pwd | sed 's/\// /g')
read -a words <<< $PWD

wordCount=$(echo $PWD | wc -w)
spaces=0
actualWord=0

if (($PWD_SHOW_DIR_PREFIX == 1)); then
    echo -ne "${DIR_PREFIX_ESCAPE_CODES}${DIR_PREFIX}\e[0m"
fi
echo -e "${DIR_NAME_ESCAPE_CODES}/\e[0m"

for val in "${words[@]}"; do
    for (( i=0; i<$spaces; i++ )) do
        echo -n " "
    done

    spaces=$((spaces+$PWD_NEXTLINE_MARGIN))
    echo -ne "${PWD_LINE_ESCAPE_CODES}└ \e[0m"

if (($PWD_SHOW_DIR_PREFIX == 1)); then
    echo -ne "${DIR_PREFIX_ESCAPE_CODES}${DIR_PREFIX}\e[0m"
fi
    echo -e "${DIR_NAME_ESCAPE_CODES}$val\e[0m"

    actualWord=$((actualWord+1))
done
