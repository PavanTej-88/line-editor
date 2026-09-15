/*
 * Simple Line Editor
 * ------------------
 * A command-line, line-oriented text editor written in C.
 * Holds the document in memory as a dynamic array of C strings
 * (an array of char* that doubles in capacity when it fills up).
 *
 * Why a dynamic array of strings instead of a linked list?
 *  - Display (printing all lines in order) is the most common
 *    operation here, and an array gives O(1) indexed access for that.
 *  - Insert/delete require shifting elements (O(n)), but for a small
 *    document (the kind this editor is meant for) that cost is
 *    negligible, and the code is simpler and more cache-friendly
 *    than a linked list.
 *  - A linked list would make insert/delete O(1) once you have a
 *    pointer to the node, but finding that node by line number is
 *    still O(n), and display + save/load become more fiddly to write
 *    correctly under time pressure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024
#define INITIAL_CAPACITY 8

typedef struct {
    char **lines;   /* array of heap-allocated line strings   */
    int count;      /* number of lines currently stored       */
    int capacity;   /* allocated size of the lines array      */
} Document;

/* ---- small helper: our own strdup so this compiles cleanly
 *      under -std=c99 with no "implicit declaration" warnings ---- */
static char *my_strdup(const char *s) {
    char *copy = malloc(strlen(s) + 1);
    if (copy != NULL) {
        strcpy(copy, s);
    }
    return copy;
}

void doc_init(Document *doc) {
    doc->capacity = INITIAL_CAPACITY;
    doc->count = 0;
    doc->lines = malloc(sizeof(char *) * doc->capacity);
}

void doc_free(Document *doc) {
    for (int i = 0; i < doc->count; i++) {
        free(doc->lines[i]);
    }
    free(doc->lines);
    doc->lines = NULL;
    doc->count = 0;
    doc->capacity = 0;
}

static void doc_grow_if_needed(Document *doc) {
    if (doc->count == doc->capacity) {
        doc->capacity *= 2;
        doc->lines = realloc(doc->lines, sizeof(char *) * doc->capacity);
    }
}

/* Insert `text` so it becomes line number `pos` (1-indexed).
 * Passing pos == count + 1 appends at the end.
 * Out-of-range positions are clamped instead of rejected, so the
 * editor never crashes on a bad line number here. */
void doc_insert(Document *doc, int pos, const char *text) {
    if (pos < 1) pos = 1;
    if (pos > doc->count + 1) pos = doc->count + 1;

    doc_grow_if_needed(doc);

    for (int i = doc->count; i >= pos; i--) {
        doc->lines[i] = doc->lines[i - 1];
    }
    doc->lines[pos - 1] = my_strdup(text);
    doc->count++;
}

/* Delete line number `pos` (1-indexed). Returns 1 on success,
 * 0 if pos was invalid (document empty or out of range). */
int doc_delete(Document *doc, int pos) {
    if (doc->count == 0) {
        printf("Error: document is empty, nothing to delete.\n");
        return 0;
    }
    if (pos < 1 || pos > doc->count) {
        printf("Error: line %d does not exist. Valid range is 1-%d.\n", pos, doc->count);
        return 0;
    }

    free(doc->lines[pos - 1]);
    for (int i = pos - 1; i < doc->count - 1; i++) {
        doc->lines[i] = doc->lines[i + 1];
    }
    doc->count--;
    return 1;
}

void doc_display(const Document *doc) {
    if (doc->count == 0) {
        printf("(document is empty)\n");
        return;
    }
    for (int i = 0; i < doc->count; i++) {
        printf("%3d: %s\n", i + 1, doc->lines[i]);
    }
}

void doc_save(const Document *doc, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: could not open '%s' for writing.\n", filename);
        return;
    }
    for (int i = 0; i < doc->count; i++) {
        fprintf(fp, "%s\n", doc->lines[i]);
    }
    fclose(fp);
    printf("Saved %d line(s) to '%s'.\n", doc->count, filename);
}

void doc_load(Document *doc, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open '%s'.\n", filename);
        return;
    }

    /* clear whatever is currently in memory before loading */
    for (int i = 0; i < doc->count; i++) {
        free(doc->lines[i]);
    }
    doc->count = 0;

    char buf[MAX_LINE_LEN];
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        buf[strcspn(buf, "\n")] = '\0';   /* strip trailing newline */
        doc_insert(doc, doc->count + 1, buf);
    }
    fclose(fp);
    printf("Loaded %d line(s) from '%s'.\n", doc->count, filename);
}

/* Bonus feature: search for a word/phrase, report matching line numbers. */
void doc_search(const Document *doc, const char *needle) {
    int found = 0;
    for (int i = 0; i < doc->count; i++) {
        if (strstr(doc->lines[i], needle) != NULL) {
            printf("Match on line %d: %s\n", i + 1, doc->lines[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("No matches found for \"%s\".\n", needle);
    }
}

void print_help(void) {
    printf("Commands:\n");
    printf("  i <line#> <text>   insert text as the given line number\n");
    printf("  d <line#>          delete the given line number\n");
    printf("  p                  print / display the whole document\n");
    printf("  f <word>           find a word or phrase, report line numbers\n");
    printf("  s <filename>       save the document to a text file\n");
    printf("  l <filename>       load a document from a text file\n");
    printf("  h                  show this help\n");
    printf("  q                  quit the editor\n");
}

int main(void) {
    Document doc;
    doc_init(&doc);

    char line[MAX_LINE_LEN];

    printf("Simple Line Editor (C). Type 'h' for help, 'q' to quit.\n");

    while (1) {
        printf("> ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;   /* EOF, e.g. Ctrl+D */
        }
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) {
            continue;   /* ignore blank input */
        }

        char cmd = line[0];
        char *rest = line + 1;
        while (*rest == ' ') rest++;   /* skip spaces after the command letter */

        if (cmd == 'q') {
            break;
        } else if (cmd == 'h') {
            print_help();
        } else if (cmd == 'p') {
            doc_display(&doc);
        } else if (cmd == 'i') {
            int pos;
            char text[MAX_LINE_LEN];
            if (sscanf(rest, "%d %[^\n]", &pos, text) == 2) {
                doc_insert(&doc, pos, text);
            } else {
                printf("Usage: i <line#> <text>\n");
            }
        } else if (cmd == 'd') {
            int pos;
            if (sscanf(rest, "%d", &pos) == 1) {
                doc_delete(&doc, pos);
            } else {
                printf("Usage: d <line#>\n");
            }
        } else if (cmd == 'f') {
            if (strlen(rest) > 0) {
                doc_search(&doc, rest);
            } else {
                printf("Usage: f <word or phrase>\n");
            }
        } else if (cmd == 's') {
            if (strlen(rest) > 0) {
                doc_save(&doc, rest);
            } else {
                printf("Usage: s <filename>\n");
            }
        } else if (cmd == 'l') {
            if (strlen(rest) > 0) {
                doc_load(&doc, rest);
            } else {
                printf("Usage: l <filename>\n");
            }
        } else {
            printf("Unknown command '%c'. Type 'h' for help.\n", cmd);
        }
    }

    doc_free(&doc);
    printf("Goodbye.\n");
    return 0;
}
