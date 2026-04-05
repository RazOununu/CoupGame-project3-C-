# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

---

## Important Behavior Rules

- Do not modify code automatically.
- Only analyze, review, and suggest improvements.
- Always explain why a change is recommended.
- Focus on readability, design, and correctness.

---

## Project Goals

This project is part of a professional portfolio and should meet high industry standards.

All suggestions should aim to improve:

- Code readability and naming clarity
- Object-oriented design quality
- Separation of concerns
- Maintainability and scalability
- Clean and consistent coding style
- Efficiency (time and space when relevant)

---

## Coding Standards

### Naming

- Use clear, descriptive, self-explanatory names
- Avoid abbreviations unless they are standard
- Prefer full words:
  - Avoid: `p`, `pl`, `cnt`
  - Prefer: `player`, `targetPlayer`, `coinCount`
- Function names should describe behavior:
  - Prefer `performCoup()` over `coup()`
  - Prefer `canArrestPlayer()` over `checkArrest()`

### Functions

- Each function should have one clear responsibility
- Avoid long functions (more than 30–40 lines)
- Extract logic into helper methods when needed

### Classes

- Each class should have a single responsibility
- Avoid placing too much logic inside `Game`
- Prefer encapsulation over direct field access

### Const Correctness

- Use `const` wherever possible
- Use `const std::string&` for parameters when appropriate
- Mark methods as `const` if they do not modify state

### Error Handling

- All invalid actions must throw `std::runtime_error`
- Error messages should be clear and meaningful

---

## Performance and Efficiency

- Avoid unnecessary copies (use references)
- Prefer passing parameters by `const&`
- Avoid redundant iterations over containers
- Use appropriate STL containers

---

## Architecture

All code lives in the `coup` namespace.

### Class hierarchy

- `Player` (abstract base, `players/Player.hpp`)
  - Defines all 6 core actions: `gather`, `tax`, `bribe`, `arrest`, `sanction`, `coup`
  - Declares pure virtual method `role() const`

- Derived roles:
  - `Governor`
  - `Spy`
  - `Baron`
  - `General`
  - `Judge`
  - `Merchant`

Each role:
- Overrides behavior when needed
- Adds unique abilities according to game rules

---

## Game Management

- `Game` owns `vector<Player*>`
- Responsible for:
  - Turn management
  - Game state
  - History tracking
  - Winner detection

- `HistoryAction`:
  ```cpp
  {action, playerBy, playerOn, coins}