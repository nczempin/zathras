#!/bin/bash
set -e

# Install dependencies if not already installed
packages=(g++ make cppcheck)

echo "Updating package lists..."
sudo apt-get update -y

echo "Installing required packages: ${packages[*]}"
sudo apt-get install -y "${packages[@]}"

echo "Verifying installations..."
for pkg in "${packages[@]}"; do
    if ! command -v ${pkg%%+*} >/dev/null 2>&1; then
        echo "Error: $pkg not found after installation" >&2
        exit 1
    fi
    echo "$pkg is installed"
done

echo "Setup complete."
