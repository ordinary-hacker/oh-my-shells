#!/usr/bin/env bash
set -e

REPO="https://github.com/ordinary-hacker/oh-my-shells.git"
INSTALL_DIR="/opt/oh-my-shells"
BIN_PATH="/usr/local/bin/oh-my-shells"

echo "[*] Installing oh-my-shells..."

if [ -d "$INSTALL_DIR" ]; then
    echo "[*] Updating existing repo at $INSTALL_DIR"
    git -C "$INSTALL_DIR" pull
else
    echo "[*] Cloning into $INSTALL_DIR"
    sudo git clone "$REPO" "$INSTALL_DIR"
fi

echo "[*] Building..."
sudo make -C "$INSTALL_DIR" clean
sudo make -C "$INSTALL_DIR"
sudo chmod +x "$INSTALL_DIR/oh-my-shells"

if [ -L "$BIN_PATH" ]; then
    sudo rm "$BIN_PATH"
fi
sudo ln -s "$INSTALL_DIR/oh-my-shells" "$BIN_PATH"

echo "[+] Installed!"
echo "    Binary: $BIN_PATH"
echo "    Repo:   $INSTALL_DIR"
echo
echo "Update anytime with:"
echo "    cd $INSTALL_DIR && sudo git pull && sudo make"
