#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "notebook.h"
#include "parser.h"
#include "funcs.h"


/*
 * book.notebook -- standart notebook filename
 * book.important.notebook -- notebook filename for important notes
 * Creates path name from book name, assigns it to path_to_file
 * Opens .notebook file which path is generated and returns it
 */
FILE *open_notebook(char *book, char **path_to_file, int important) {
    char *filename;
    int name_len = 4; // for "temp"

    char *temp_filename = "temp";
    char *dot_imp = ".important";
    char *dot_nb = ".notebook";

    // where all notebook files are placed
    char *folder = "/.makenote/"; 
    char *dir = malloc(strlen(getenv("HOME")) + strlen(folder) + 1);
    strcpy(dir, getenv("HOME"));
    strcat(dir, folder);

    // +1 for '\0'
    if (book) {
        int len = strlen(book);
        filename = malloc(len + 1);
        strncpy(filename, book, len + 1);
        name_len = len;
    } else {
        filename = malloc(strlen(temp_filename) + 1);
        strncpy(filename, temp_filename, 5);
    }

    if (important) {
        // ".important"
        int imp_len = strlen(dot_imp);
        name_len += imp_len;
        filename = realloc(filename, name_len + 1);
        strcat(filename, dot_imp);
    }
    
    // ".notebook"
    int nb_len = strlen(dot_nb);
    name_len += nb_len;
    filename = realloc(filename, name_len + 1);
    strcat(filename, dot_nb);        
    
    // "~/.makenote/"
    int dir_len = strlen(dir);
    char *path = malloc(dir_len + name_len + 1);
    strncpy(path, dir, dir_len + 1);
    strcat(path, filename);
    
    *path_to_file = path;
    FILE *file = fopen(path, "a+");
    return file;
}

int main(int argc, char *argv[]) {
    if (argc == 1) eject("No arguments given.");

    int important = 0, remind = 0, clear = 0;
    char *note = NULL;
    char *book = NULL;

    // parse command line arguments
    int check = parse_args(argv + 1, argc - 1, &important, &remind, &note, &book, &clear);
    
    // inform about error
    char *message;
    if (check == INVALID_OPTION) {
        message = "Invalid option provided.";
    } else if (check == NOTE_PROVIDED_TWICE) {
        message = "Can't write two notes at once, did you forget single quotes around note text?";
    } else if (check == INVALID_VALUE) {
        message = "Invalid value for option provided.";
    } else if (check == NO_VALUE_PROVIDED) {
        message = "No value for option is provided.";
    } else if (check == INVALID_MIX) {
        message = "Not provided necessary value for option or mixed unsuitable options.";
    }
    if (check != 0) {
        eject(message);
    }

    int is_temp = (book) ? 0 : 1;
    char *path_to_file = NULL;
    
    if (remind == IMPORTANT) { // need to open important notebook
                               // if must remind important notes
        important = 1;
        remind = ALL;
    }
    
    is_temp = (important) ? 0 : is_temp; // if writing to, or reading from
                                         // temp.important.notebook, consider it
                                         // as non-temp notebook
    FILE* file = open_notebook(book, &path_to_file, important);

    if (note) { // write note to file
        int note_len = strlen(note);
        char *text = malloc(note_len + 1);
        strncpy(text, note, note_len + 1);

        Note note_struct = { .text = text };
        write_notebook(note_struct, is_temp, path_to_file, file);
    
    } else if (remind) { // remind notes
        int count = 0;
        Note **notes = read_notebook(is_temp, &count, path_to_file, file);

        if (!important && remind == ALL) { // display important notes too
            is_temp = 0; // treat as non-temp
            int im_count = 0;
            important = 1;

            fclose(file);
            file = open_notebook(book, &path_to_file, important);
            Note **important_notes = read_notebook(is_temp, &im_count, path_to_file, file);

            if (important_notes) {
                printf("\nImportant notes:\n");
                remind_notes(remind, im_count, important_notes);
                freenotes(im_count, important_notes); // no more needed
            }
        }

        if (notes) {
            printf("\n%s", (remind == ALL) ? "Notes:\n" : "");
            remind_notes(remind, count, notes);
            freenotes(count, notes); // no more needed
        }
      
        printf("\n");

    } else if (clear) { // clear notes
        clear_notebook(1, path_to_file, file);
        
        // clear important notes too
        important = 1;
        file = open_notebook(book, &path_to_file, important);
        clear_notebook(1, path_to_file, file);
        return 0; // skip closing deleted file
    }
    
    fclose(file);
    
    return 0;
}

