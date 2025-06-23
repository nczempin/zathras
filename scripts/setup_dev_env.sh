#!/bin/bash
# Setup script for development tools

set -e

echo "=== Zathras Development Environment Setup ==="
echo ""

# Parse arguments
INSTALL_MODE="install"  # Default: install missing tools
while [[ $# -gt 0 ]]; do
    case $1 in
        --check-only)
            INSTALL_MODE="check"
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [options]"
            echo ""
            echo "By default, installs all missing development tools."
            echo ""
            echo "Options:"
            echo "  --check-only  Only check what's installed (don't install)"
            echo "  -h, --help    Show this help message"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use -h for help"
            exit 1
            ;;
    esac
done

# Function to check and optionally install a package
check_or_install() {
    local cmd=$1
    local package=$2
    local description=$3
    
    if command -v "$cmd" &> /dev/null; then
        echo "✓ $description is installed: $(eval "$cmd --version 2>&1 | head -1")"
        return 0
    else
        echo "✗ $description not found"
        if [ "$INSTALL_MODE" = "install" ]; then
            echo "  Installing $package..."
            sudo apt-get install -y "$package"
            return $?
        else
            echo "  To install: sudo apt-get install $package"
            return 1
        fi
    fi
}

# Check if running on Ubuntu/Debian
if ! command -v apt-get &> /dev/null; then
    echo "⚠️  Non-Debian system detected."
    echo "Please manually install:"
    echo "  - build-essential (GCC/G++ compiler)"
    echo "  - clang-tidy (LLVM linting tool)"
    echo "  - cppcheck (static analysis)"
    echo "  - clang-format (code formatter)"
    echo "  - gdb (debugger)"
    echo "  - valgrind (memory checker)"
    echo "  - cmake (build system)"
    exit 1
fi

echo "Detected Ubuntu/Debian system"
echo ""

# Update package list if installing
if [ "$INSTALL_MODE" = "install" ]; then
    echo "This script will install missing development tools."
    echo "You may be prompted for your sudo password."
    echo ""
    echo "Updating package list..."
    sudo apt-get update
    echo ""
fi

# Track if all tools are present
ALL_PRESENT=true

echo "Core Development Tools:"
echo "----------------------"
check_or_install "g++" "build-essential" "G++ compiler" || ALL_PRESENT=false
check_or_install "make" "build-essential" "Make build tool" || ALL_PRESENT=false
check_or_install "gdb" "gdb" "GDB debugger" || ALL_PRESENT=false
check_or_install "cmake" "cmake" "CMake build system" || ALL_PRESENT=false

echo ""
echo "Code Quality Tools:"
echo "------------------"
check_or_install "cppcheck" "cppcheck" "Cppcheck static analyzer" || ALL_PRESENT=false
check_or_install "clang-tidy" "clang-tidy" "Clang-tidy linter" || ALL_PRESENT=false
check_or_install "clang-format" "clang-format" "Clang-format formatter" || ALL_PRESENT=false
check_or_install "valgrind" "valgrind" "Valgrind memory checker" || ALL_PRESENT=false

echo ""
echo "Version Control:"
echo "---------------"
check_or_install "git" "git" "Git version control" || ALL_PRESENT=false

echo ""
echo "============================================"
if [ "$ALL_PRESENT" = true ]; then
    echo "✅ All development tools are installed!"
else
    if [ "$INSTALL_MODE" = "check" ]; then
        echo "⚠️  Some tools are missing."
        echo ""
        echo "To install all missing tools, run:"
        echo "  $0"
    else
        echo "⚠️  Some tools could not be installed."
        echo "Please check the output above for errors."
    fi
fi

echo ""
echo "Available development commands:"
echo "  make              - Build the project"
echo "  make clean        - Clean build artifacts"
echo "  make lint         - Run all linters"
echo "  make lint-cppcheck - Run cppcheck only"
echo "  make lint-clang-tidy - Run clang-tidy only"
echo ""
echo "For more information, see:"
echo "  README.md         - Project overview"
echo "  CLAUDE.md         - Development guidelines"
echo "  docs/LINTING.md   - Linting documentation"