# Contributing to Zathras

Welcome! This guide will help you get started with contributing to the Zathras chess engine.

## Quick Start

1. **Clone the repository**
   ```bash
   git clone https://github.com/nczempin/zathras.git
   cd zathras
   ```

2. **Set up your development environment**
   ```bash
   ./scripts/setup_dev_env.sh
   ```
   This installs all necessary development tools.

3. **Build and test**
   ```bash
   make clean && make
   ./zathras  # Run the engine
   ```

## Development Workflow

1. **Create a feature branch**
   ```bash
   git checkout dev
   git pull origin dev
   git checkout -b feature/ZTH-<issue>-<description>
   ```

2. **Make your changes**
   - Follow the coding standards (see below)
   - Add tests if applicable
   - Run linting: `make lint`

3. **Test your changes**
   ```bash
   make clean && make
   # Run perft tests
   echo -e "uci\nposition startpos\nperft 4\nquit" | ./zathras
   ```

4. **Commit and push**
   ```bash
   git add .
   git commit -m "Your descriptive commit message"
   git push -u origin your-branch-name
   ```

5. **Create a pull request**
   - Target the `dev` branch (not `main`)
   - Reference the issue number
   - Describe your changes

## Coding Standards

- **C++17** standard
- Follow **C++ Core Guidelines** (enforced by clang-tidy)
- Run `make lint` before committing
- Use meaningful variable and function names
- Comment complex algorithms

## Tools

- **Compiler**: G++ with C++17 support
- **Build**: Make
- **Linting**: cppcheck and clang-tidy
- **Debugging**: GDB
- **Memory checking**: Valgrind

## Project Structure

```
zathras/
├── src/                  # UCI interface
├── zathras_lib/src/      # Core chess engine
├── scripts/              # Development scripts
├── docs/                 # Documentation
└── .github/workflows/    # CI/CD configuration
```

## Need Help?

- Read [CLAUDE.md](CLAUDE.md) for detailed project information
- Check [docs/LINTING.md](docs/LINTING.md) for linting guidelines
- Open an issue on GitHub for questions or bug reports

Happy coding! ♟️