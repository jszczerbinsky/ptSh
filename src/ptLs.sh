#!/bin/bash

source /usr/share/ptSh/config
test -f ~/.config/ptSh/config && source ~/.config/ptSh/config

arg=""
if [[ $1 == "-"* ]];then
    arg=$1
fi

LS=$(ls -al --group-directories-first | sed 's/\s+/ /g')

columnSize=0
while read -r line; do
    read -a words <<< $line
    for val in "${words[@]}"; do
        if (($columnSize < ${#val})); then
            columnSize=${#val}
        fi
    done 
done <<<$(ls -a | sed 's/\s+/ /g')

columnSize=$(($columnSize+$LS_MIN_FILE_OFFSET))

if ((${#DIR_PREFIX} > ${#FILE_PREFIX})); then
    columnSize=$(($columnSize+${#DIR_PREFIX}))
else
    columnSize=$(($columnSize+${#FILE_PREFIX}))
fi

columns=$(($(tput cols) / $columnSize))
i=0
actualChar=0
actualColumn=0

echo "$LS" | while read -r line; do
    if ((i > 2)); then
        read -a words <<< $line
        word=0
        wordCount=$(echo $line | wc -w)
        lastWord=$(($wordCount-1))
        if [[ ${line} == *"->"* ]]; then
            lastWord=$((lastWord-2))
        fi
        dir=0
        for val in "${words[@]}"; do
            if ((word == 0)); then
                if [ ${val:0:1} == "d" ]; then
                    dir=1
                    echo -ne $DIR_PREFIX_ESCAPE_CODES
                    echo -n "$DIR_PREFIX"
                    actualChar=$((actualChar+${#DIR_PREFIX}))
                else
                    echo -ne $FILE_PREFIX_ESCAPE_CODES
                    echo -n "$FILE_PREFIX"
                    actualChar=$((actualChar+${#FILE_PREFIX}))
                fi
                echo -ne "\e[0m"
            fi
            if ((word == lastWord)); then
                if (($dir==1)); then
                    echo -ne $DIR_NAME_ESCAPE_CODES
                else
                    echo -ne $FILE_NAME_ESCAPE_CODES
                fi
                echo -ne "${val}\e[0m"
                actualChar=$((actualChar+${#val}))

                if [[ $arg == *"l"* ]]; then
                    echo ""
                else
                    for (( i=0; i<$((columnSize-$((actualChar%columnSize)))); i++ )); do
                        echo -n " "
                    done
                    actualChar=$((actualChar+$((columnSize-$((actualChar%columnSize))))))
                    actualColumn=$((actualColumn+1))
                    if (($actualColumn >= $columns)); then
                        echo
                        actualColumn=0
                        actualChar=0
                    fi
                fi
            fi
            word=$((word+1))
        done
    fi
    i=$((i+1))
done
echo
