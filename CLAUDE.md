# Claude AI Development Guidelines

## Branching Strategy

### Preferred Branch Structure
- **main**: Primary production branch (replaces legacy "master")
- **dev**: Development integration branch where feature PRs are merged
- **feature branches**: Individual feature development (PR to dev)
- **release branches**: Optional semver-based release preparation (dev → release → main)

### Branch Flow
1. Feature development: `feature/branch-name` → `dev`
2. Integration testing: `dev` branch
3. Release preparation: `dev` → `release/vX.Y.Z` → `main` (optional)
4. Direct release: `dev` → `main`

### Implementation Notes
- Replace legacy "master" branches with "main"
- All feature PRs should target "dev", not "main"
- Only release/hotfix PRs should target "main" directly
- Use semantic versioning for release branches when applicable

## AI Development Standards
- Always follow the established branching strategy
- Check target branches before rebasing PRs
- Update repository default branches to align with strategy
- Maintain clean, linear history where possible