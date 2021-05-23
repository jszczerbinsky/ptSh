#!/usr/bin/env bash

source ~/.local/share/ptSh/config
test -f ~/.config/ptSh/config && source ~/.config/ptSh/config

CMD="cp"

if [[ -z $1 ]] || [[ $1 == "--help" ]]; then
  cat <<< "$(command cp --help)"
  exit 0
fi

args=""
typeset -a files && files=()

read -ra words <<<"${@}"

for val in "${words[@]}"; do
  if [[ $val == "-"* ]]; then
    args="$args $val"
  else
    files+=( "$val" )
  fi
done

# trim leading space
args="${args#"${args%%[![:space:]]*}"}"

# if args contain interactive
if [[ $args =~ i ]]; then
  # if file copying to exists
  if [[ $(readlink -e "${files[1]}") ]]; then
    read -r -p "$(echo -e "${WARNING_PREFIX_ESCAPE_CODES}${WARNING_PREFIX}\x1B[0mcp: overwrite '${files[1]}'? ")" resp
    [[ $resp == [yY] ]] || exit 1
  # if args = '-i'
    if [[ "${#args}" -eq 2 ]]; then
      args=""
    else
      # if args contain only a pattern like '-iv'
      if [[ $args =~ ^-[a-zA-Z]+$ ]]; then
        # strip i leaving '-v'
        args="${args//i}"
      # if arg contain space like '-iv -a'
      elif [[ $args =~ [[:space:]] ]]; then
        typeset -a aargs && aargs=( "$args" )
        typeset -a newargs && newargs=()
        # convert to array for ease
        for a in "${aargs[@]}"; do
          if [[ $a =~ i ]]; then
            # if arg = '-iv'
            if [[ "${#a}" -gt 2 ]]; then
              newargs+=( "${a//i}" )
            # if arg = '-i'
            else
              newargs+=( "${a//\-i}" )
            fi
          # if arg = '-a'
          else
            newargs+=( "${a}" )
          fi
        done
        args="$(printf " %s" "${newargs[@]}")"
      fi
    fi
  fi
fi

while read -r line; do
  if [[ "$line" == *" -> "* ]]; then
      echo -e "${SUCCESS_PREFIX_ESCAPE_CODES}${SUCCESS_PREFIX}\x1B[0m$line"
  elif [[ -n $line ]];then
      err=$(echo "$line" | sed "s/$1: //g" | sed 's/^[^:]*://g')
      echo -ne "${ERROR_PREFIX_ESCAPE_CODES}${ERROR_PREFIX}\x1B[0m"
      echo -e "${ERROR_MESSAGE_ESCAPE_CODES}$err\x1B[0m"
  else
      echo -ne "${SUCCESS_PREFIX_ESCAPE_CODES}${SUCCESS_PREFIX}\x1B[0m"
      echo -e "${SUCCESS_MESSAGE_ESCAPE_CODES}${SUCCESS_MESSAGE}\x1B[0m"
  fi
done < <(eval "$CMD $args ${files[*]}")
