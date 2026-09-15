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