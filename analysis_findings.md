# Move Generation Discrepancy Analysis - Issue #86

## Problem Summary
Zathras generates 10675 moves vs Stockfish's correct 9467 for Kiwipete perft depth 3.
**Discrepancy: +1208 moves (12.8% over-generation)**

## Root Cause Analysis

### 1. Promotion Move Display Issue
**Critical Finding**: Promotion moves not displayed correctly in divide output.

**Stockfish (correct)**:
```
b2a1b: 36  # bishop promotion
b2a1n: 36  # knight promotion  
b2a1q: 33  # queen promotion
b2a1r: 33  # rook promotion
```

**Zathras (incorrect)**:
```
b2a1: 36  # type unclear
b2a1: 36  # duplicate
b2a1: 36  # duplicate  
b2a1: 36  # duplicate
```

**Hypothesis**: Divide command formatting doesn't include promotion piece type, making analysis difficult.

### 2. Systematic Over-generation Pattern
Every root move generates more moves than Stockfish:

| Move  | Stockfish | Zathras | Difference |
|-------|-----------|---------|------------|
| b4c5  | 1352     | 1555    | +203       |
| c4c5  | 1409     | 1624    | +215       |
| d2d4  | 1643     | 1695    | +52        |
| f1f2  | 1623     | 1874    | +251       |
| f3d4  | 1687     | 1915    | +228       |
| g1h1  | 1753     | 2012    | +259       |

### 3. Potential Root Causes
1. **Illegal move generation**: Generating moves that leave king in check
2. **Duplicate move generation**: Same move generated multiple times
3. **Incorrect promotion handling**: Over-generating promotion variants
4. **Move validation issues**: Pseudolegal vs legal move filtering problems

## Next Steps
1. Fix divide command to show promotion piece types
2. Compare exact move lists (not just counts) between engines
3. Identify specific illegal moves being generated
4. Fix move generation validation
5. Validate fix with comprehensive perft tests

## Files to Investigate
- `src/Divide_command.cpp` - Fix promotion display
- `zathras_lib/src/Move_generator.cpp` - Move generation logic
- `zathras_lib/src/Move.cpp` - Move representation and formatting

Generated: 2025-06-24 00:47