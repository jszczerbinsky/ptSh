#!/bin/bash

source /usr/local/share/ptSh/config
test -f ~/.config/ptSh/config && source ~/.config/ptSh/config

columnSize=0

function getColumnSize(){
    while read -r line; do
        read -ra words <<< $line
        for val in "${words[@]}"; do
            if ((columnSize < ${#val})); then
                columnSize=${#val}
            fi
        done 
    done <<<$(ls $arg 2>/dev/null)
}

function setArgs(){
    if [[ $1 != "-"* ]] || [[ $1 == "-l" ]]; then
        arg=""
        return 0
    fi
    if [[ $1 == *"a"* ]]; then
        arg="${arg}a"
    fi
    if [[ $1 == *"A"* ]]; then
        arg="${arg}A"
    fi
}

function align(){
    for (( i=0; i<$((columnSize-(actualChar%columnSize))); i++ )); do
        echo -n " "
    done
}

arg="-"
setArgs $1

a="-"
if [[ ! -z $arg ]];
    then a=$arg
fi

LS=$(ls ${a}l --group-directories-first --time-style=+"" 2>/dev/null)

getColumnSize

columnSize=$((columnSize+LS_MIN_FILE_OFFSET))

if ((${#DIR_PREFIX} > ${#FILE_PREFIX})); then
    columnSize=$((columnSize+${#DIR_PREFIX}))
else
    columnSize=$((columnSize+${#FILE_PREFIX}))
fi

columns=$(($(tput cols) / columnSize))
i=0
actualChar=0
actualColumn=0

while read -r line; do
    read -a words <<< $line
    if [[ $(echo $line | wc -w) == 2 ]]; then continue; fi

    if [[ ${words[0]} == "d"* ]]; then
        filename="${DIR_PREFIX_ESCAPE_CODES}${DIR_PREFIX}\e[0m${DIR_NAME_ESCAPE_CODES}"
    elif [[ ${words[0]} == "l"* ]];then
        filename="${DIR_PREFIX_ESCAPE_CODES}link\e[0m${DIR_NAME_ESCAPE_CODES}"
    else
        filename="${FILE_PREFIX_ESCAPE_CODES}${FILE_PREFIX}\e[0m${FILE_NAME_ESCAPE_CODES}"
    fi

    filename="$filename${words[5]}\e[0m"
    
    if [[ $1 == *"l"* ]]; then
        echo -ne $filename
        actualChar=${#filename}
        align        
        echo "${words[0]} ${words[2]} ${words[3]}"
    else
        echo -ne $filename
        actualChar=$((actualChar+${#filename}))
        align
        actualChar=$((actualChar+(columnSize-(actualChar%columnSize))))
        actualColumn=$((actualColumn+1))
        if (($actualColumn >= $columns)); then
            echo
            actualColumn=0
            actualChar=0
        fi
    fi
done <<<$(echo "$LS")
echo
echo
