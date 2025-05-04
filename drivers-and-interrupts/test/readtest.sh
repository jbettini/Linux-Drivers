#!/bin/bash

TXT_RED="\033[1;31m"
TXT_GREEN="\033[1;32m"
TXT_YELLOW="\033[1;33m"
TXT_BLUE="\033[1;34m"
FANCY_RESET="\033[0m"

print_color () {
    echo -e "$1$2$FANCY_RESET"
}

print_color "$TXT_GREEN" "Checking multiple read\n"

FIRST_READ="${FIRST_READ:-7}"
SECOND_READ="${SECOND_READ:-8}"
MISC="/dev/Keylogger_Misc"

exec 3< "$MISC"

first=$(head -c "$FIRST_READ" <&3)
second=$(head -c "$SECOND_READ" <&3)

echo -e "First read : $first"
echo -e "Second read : $second"

exec 3<&-

print_color "$TXT_GREEN" "\nChecking cat\n"

cat "$MISC"

