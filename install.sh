#!/bin/bash

SOURCE_FILE="main.c"
BINARY_NAME="chksm"
INSTALL_DIR="/usr/local/bin"

if [ "$(id -u)" -ne 0 ]; then
    echo "Root privileges are required to install the script."
    exit 1
fi

if [ ! -f "$SOURCE_FILE" ]; then
    echo "Source file '$SOURCE_FILE' not found."
    exit 1
fi

command -v clang >/dev/null 2>&1 || { echo "clang is not installed. Please install clang.'"; exit 1; }
dpkg -s libssl-dev >/dev/null 2>&1 || { echo "libssl-dev is not installed. Please install libssl-dev."; exit 1; }

if [ ! -f "$BINARY_NAME" ]; then
    echo "Executable file '$BINARY_NAME' not found. Compiling from '$SOURCE_FILE'..."
    clang "$SOURCE_FILE" -o "$BINARY_NAME" -lssl -lcrypto

    if [ $? -ne 0 ]; then
        echo "Error compiling the source file. Please check for missing dependencies."
        exit 1
    fi
    echo "Compilation successful."
fi

if [ ! -f "$BINARY_NAME" ]; then
    echo "Executable file '$BINARY_NAME' still not found after compilation."
    exit 1
fi

echo "Installing '$BINARY_NAME' to $INSTALL_DIR..."
cp "$BINARY_NAME" "$INSTALL_DIR"

if [ $? -eq 0 ]; then
    echo "File successfully installed to $INSTALL_DIR."
else
    echo "Error installing the file."
    exit 1
fi

chmod +x "$INSTALL_DIR/$BINARY_NAME"
echo "File made executable."

echo "Installation completed."
