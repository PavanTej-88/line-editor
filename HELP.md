# Line Editor — Help

Run the editor, then type commands one at a time at the `>` prompt.

| Command | What it does | Example |
|---|---|---|
| `i <line#> <text>` | Insert `text` at line number `<line#>`, shifting existing lines down. | `i 1 hello world` |
| `d <line#>` | Delete the line at `<line#>`, shifting the lines below it up. | `d 2` |
| `p` | Print/display the whole document with line numbers. | `p` |
| `f <word>` | Find a word or phrase, print every line number where it appears. | `f hello` |
| `s <filename>` | Save the current document to a text file. | `s notes.txt` |
| `l <filename>` | Load a document from a text file (replaces what's in memory). | `l notes.txt` |
| `h` | Show this list of commands. | `h` |
| `q` | Quit the editor. | `q` |

## Example session

```
Simple Line Editor (C). Type 'h' for help, 'q' to quit.
> i 1 hello world
> i 2 second line
> p
  1: hello world
  2: second line
> f hello
Match on line 1: hello world
> d 1
> p
  1: second line
> s notes.txt
Saved 1 line(s) to 'notes.txt'.
> q
Goodbye.
```

## Notes

- Line numbers start at 1.
- Inserting at a line number beyond the end of the document appends the line at the end instead of failing.
- Deleting an invalid line number (too high, too low, or on an empty document) prints an error, leaves the document unchanged — the editor does not crash.
