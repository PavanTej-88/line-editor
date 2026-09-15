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