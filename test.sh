#!/bin/bash
set -e

# Remove old binary
rm -f oh-my-shells

echo -e "\033[1;34m[+] Building oh-my-shells...\033[0m"
make

echo -e "\n\033[1;32m[+] Testing 'generate' command with all flag combinations\033[0m\n"

SHELL_ID="bash_i"
LHOST="10.10.10.10"
LPORT="5555"

# Test matrix for generate command
COMMANDS=(
    "./oh-my-shells generate $SHELL_ID -H $LHOST -P $LPORT"
    "./oh-my-shells generate $SHELL_ID -H $LHOST -P $LPORT -p"
    "./oh-my-shells generate $SHELL_ID -H $LHOST -P $LPORT -l"
    "./oh-my-shells generate $SHELL_ID -H $LHOST -P $LPORT -p -l"
    "./oh-my-shells generate $SHELL_ID -H $LHOST -P $LPORT -e url"
    "./oh-my-shells generate $SHELL_ID -H $LHOST -P $LPORT -e base64"
    "./oh-my-shells generate $SHELL_ID -H $LHOST -P $LPORT --no-color"
    "./oh-my-shells generate $SHELL_ID -H $LHOST -P $LPORT -p --no-color"
    "./oh-my-shells generate $SHELL_ID -H $LHOST -P $LPORT -l --no-color"
    "./oh-my-shells generate $SHELL_ID -H $LHOST -P $LPORT -e url --no-color"
)

for cmd in "${COMMANDS[@]}"; do
    echo -e "\n\033[1;33m$ $cmd\033[0m"
    eval $cmd
    echo -e "\033[1;30m-----------------------------\033[0m"
done

echo -e "\n\033[1;32m[+] Testing 'list', 'search', and 'show' commands\033[0m\n"

# List command with filters
echo -e "\033[1;33m$ ./oh-my-shells list --os unix --type reverse --proto tcp --lang bash\033[0m"
./oh-my-shells list --os unix --type reverse --proto tcp --lang bash

# Search command
echo -e "\n\033[1;33m$ ./oh-my-shells search bash\033[0m"
./oh-my-shells search bash

# Show command
echo -e "\n\033[1;33m$ ./oh-my-shells show $SHELL_ID\033[0m"
./oh-my-shells show $SHELL_ID

echo -e "\n\033[1;32m[+] All tests completed.\033[0m\n"
