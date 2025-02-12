#!/bin/bash

# Name of the installed binary
BINARY_NAME="chksm"
INSTALL_DIR="/usr/bin"

# Check for root privileges
if [ "$(id -u)" -ne 0 ]; then
    echo "Root privileges are required to uninstall the script."
    exit 1
fi

# Check if the file exists in /usr/bin
if [ ! -f "$INSTALL_DIR/$BINARY_NAME" ]; then
    echo "Executable file '$BINARY_NAME' not found in $INSTALL_DIR. Nothing to remove."
    exit 1
fi

# Remove the binary
echo "Removing '$BINARY_NAME' from $INSTALL_DIR..."
rm "$INSTALL_DIR/$BINARY_NAME"

# Verify removal
if [ $? -eq 0 ]; then
    echo "Uninstallation completed successfully."
else
    echo "Error occurred while removing the file."
    exit 1
fi
