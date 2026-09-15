# Simple Line Editor (C)

A command-line line editor built for the "Build a Simple Line Editor in C" studio competition.

## Team

- Member 1 — Harshini Reddy — Paper Design
- Member 2 — Chetana Poorvi — Documentation and  testing
- Member 3 — PavanTej — Built in VScode

## Features implemented

Core (required 2–3, we implemented 4):
- Insert a line
- Delete a line
- Display the document
- Save / load a file

Bonus:
- Search (find which line number(s) contain a word or phrase)

## Data structure

The document is stored in memory as a **dynamic array of strings** (`char**` that
doubles in capacity when full). We chose this over a linked list because display
(the most frequent operation) needs fast indexed access, and for a small document
the O(n) shifting cost of insert/delete is negligible — see the comment block at
the top of `editor.c` for the full trade-off discussion.

## How to compile

```
gcc -Wall -Wextra -std=c99 -o editor editor.c
```

## How to run

```
./editor
```

Type `h` at the prompt for the list of commands, or see `HELP.md`.

## Files

- `editor.c` — the editor source code
- `HELP.md` — command reference with usage examples
- `README.md` — this file
- paper design photo/scan — [add filename once uploaded]
