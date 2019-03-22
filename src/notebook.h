#ifndef NOTEBOOK_H
#define NOTEBOOK_H

enum RemindValues {
    ALL=1, FIRST, LAST, IMPORTANT
};

typedef struct Note {
    char *text;
} Note;

FILE *clear_notebook(int clear, char *filename, FILE *file);
void freenotes(int count, Note **notes);
void remind_notes(int selector, int count, Note **notes);
Note **read_notebook(int is_temp, int *out_count, char *filename, FILE *file);
void write_notebook(Note note, int is_temp, char *filename, FILE *file);

#endif
