#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "notebook.h"
#include "funcs.h"

#define EXPIRE_TIME 36000

/*
 * Frees memory allocated for notes' pointers
 */
void freenotes(int count, Note **notes) {
    int i;
    for (i = 0; i < count; i++) {
        if (notes[i]) {
            if (notes[i]->text) free(notes[i]->text);
            free(notes[i]);
        }
    }
    free(notes);
}

/*
 * Read one line (aka note) from file
 */
char *read_note(FILE *file) {
    int i;
    int cursor;
    int note_len = 0;
    
    while (1) {
        cursor = fgetc(file);
        if (cursor == '\n' || cursor == EOF) {
            break;
        } else {
            note_len++;
        }
    }

    if (note_len == 0) {
        return NULL; // no notes left to read
    }

    char *note_text = malloc(++note_len);
    fseek(file, -(note_len), SEEK_CUR);

    for (i = 0; i < note_len - 1; i++) {
        cursor = fgetc(file);
        note_text[i] = cursor;
    }

    note_text[note_len - 1] = '\0';
    fgetc(file); // skip '\n' for further reading
    return note_text;
}

FILE *clear_notebook(char *filename, FILE *file) {
    fclose(file);
    FILE* clean_file = fopen(filename, "w");
    return clean_file;
}

void handle_temp_notebook(char* filename, FILE *file) {
    time_t current_time;
    time(&current_time);    

    fseek(file, 0L, SEEK_END);
    if (ftell(file) != 0) { // file is not new, check if expired
        rewind(file);
        time_t created_time;
        fread(&created_time, sizeof(time_t), 1, file);
    
        if (current_time - created_time > EXPIRE_TIME) { // 10 hours passed
            file = clear_notebook(filename, file);
            fwrite(&current_time, sizeof(time_t), 1, file);
        }
    } else {
        rewind(file);
        fwrite(&current_time, sizeof(time_t), 1, file);
    }
}

void write_notebook(Note note, int is_temp, char* filename, FILE *file) {
    if (is_temp) { // check if 10 hours expired
                   // if so, then clean notebook
                   // esle if file is new, then add time mark
        handle_temp_notebook(filename, file);
    }

    size_t note_len = strlen(note.text);
    fwrite(note.text, 1, note_len, file);
    fwrite("\n", 1, 1, file);
}

Note **read_notebook(int is_temp, int *out_count, char *filename, FILE *file) {
    rewind(file);
    Note *note = NULL;
    Note **notes = NULL;
    
    if (is_temp) {
        handle_temp_notebook(filename, file);
        fseek(file, sizeof(time_t), SEEK_SET); // skip time mark
    }

    int lines = 0;
    char *note_text = NULL;
    while((note_text = read_note(file)) != NULL) {
        lines++;
        note = malloc(sizeof(Note));
        note->text = note_text;
        notes = realloc(notes, lines * sizeof(Note*));
        notes[lines - 1] = note;
    }

    *out_count = lines;
    return notes;
}

void remind_notes(int selector, int count, Note **notes) {
    int i;
    switch (selector) {
        case ALL:
            for (i = 0; i < count; i++) {
                printf("%d: %s\n", i + 1, notes[i]->text);
            }
            break;
        case FIRST:
            printf("Note: %s\n", notes[0]->text);   
            break;
        case LAST:
            printf("Note: %s\n", notes[count - 1]->text);
            break;
        default:
            eject("Invalid reminder selector provided."); 
    }
}

