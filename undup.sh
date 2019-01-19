# remove duplicate xpp macro definitions from file
# usage: ./undup.sh filename
debug=: # off (noop)
debug=echo # on
file=${1:-global.h}
undup () { # recursively remove first duplicate if any
  dup=$(cat "$1" end.h \
    |cpp 2>&1  >/dev/null\
    |sed -n '/note: this is the location of the previous definition/{s/^[^:]*://;s/:.*//;{p;q}}')
  $debug $dup
  if [ "$dup" != "" ]; then $debug true: duplicate on line $dup
    sed "${dup}d" "$1" >"$1".tmp && mv "$1".tmp "$1"
    undup "$1"
  else $debug false: no duplicates in $1
 fi
}
undup "$file"
