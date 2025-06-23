# Naming Conventions for Zathras Chess Engine

## File Naming

### Header Files
- Use **PascalCase** for header files: `Position.h`, `Move_generator.h`, `Bitboard.h`
- Use underscores to separate logical components: `Move_generator.h`, `Move_container.h`
- Header guards should match the filename in UPPERCASE with underscores: `POSITION_H_`, `MOVE_GENERATOR_H_`

### Source Files
- Match the header file name exactly but with `.cpp` extension
- Example: `Position.h` → `Position.cpp`

### Case Sensitivity
- **IMPORTANT**: The codebase must work on case-sensitive filesystems (Linux/macOS)
- All `#include` statements must match the exact case of the file
- Do not create files that differ only in case

## Code Naming

### Classes
- Use **PascalCase**: `Position`, `MoveGenerator`, `Bitboard`

### Functions and Methods
- Use **snake_case**: `generate_legal_moves()`, `make_move()`, `extract_square()`

### Variables
- Local variables: **snake_case**: `legal_moves`, `white_to_move`
- Member variables: **snake_case** with no prefix: `white`, `black`, `pawns`
- Constants: **UPPERCASE_WITH_UNDERSCORES**: `MAX_MOVES`, `STARTING_FEN`

### Namespaces
- Use **PascalCase**: `Positions`, `Moves`, `Interface`

### Type Definitions
- Basic types: **snake_case** with `_t` suffix: `square_t`, `piece_t`, `move_type_t`
- Bitboard type: `bb` (established convention)

## Include Guidelines

### Include Order
1. Corresponding header (for .cpp files)
2. System headers (`<iostream>`, `<vector>`, etc.)
3. Library headers
4. Project headers

### Include Style
- Use quotes for project headers: `#include "Position.h"`
- Use angle brackets for system headers: `#include <iostream>`
- Always use forward slashes, never backslashes: `#include "zathras_lib/src/Position.h"`

## Platform Compatibility

### Conditional Compilation
- Guard platform-specific code appropriately
- Example from Bitboard.h:
```cpp
#if defined(__GNUC__)
  // GCC-specific code
#elif defined(_WIN64) && (_MSC_VER >= 1500)
  #include <intrin.h>
#endif
```

### Avoid Platform-Specific Headers
- Do not use Windows-specific headers like `<windows.h>` or `<direct.h>`
- Use standard C++ headers whenever possible
- If platform-specific code is needed, provide alternatives for all supported platforms