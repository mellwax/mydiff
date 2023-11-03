#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>

// TODO: Kommentare schreiben (Doxygen Style)
// TODO: bei prints ".txt" entfernen


static const int num_of_args = 3;
static const char *program_name;
static bool case_insensitive = false;
static bool write_output = false;

static void parse_args(int argc, char* argv[], char **input1_path, char **input2_path, 
        char **output_path);
static void print_error_and_exit(const char *message);
static void print_usage_error_and_exit(const char *message);
static void try_open_file(FILE **file, const char *file_path, char *mode);
static void work(FILE *input1, FILE *input2, FILE *output);
static void print_diff(int lin, int chr, FILE *output);
static int is_eof_or_newline(char c1, char c2);
static int is_c_eof_or_newline(char c);
static char skip_to_next_char(FILE *file);
static void close(FILE *file);

int main(int argc, char* argv[]) {
    program_name = argv[0];

    char *input1_path = NULL;
    char *input2_path = NULL;
    char *output_path = NULL;

    parse_args(argc, argv, &input1_path, &input2_path, &output_path);

    FILE *input1 = NULL;
    FILE *input2 = NULL;
    FILE *output = NULL;
    
    try_open_file(&input1, input1_path, "r");
    try_open_file(&input2, input2_path, "r");
    try_open_file(&output, output_path, "w");
   
    work(input1, input2, output);
    
    close(input1);
    close(input2);
    close(output);
    
    return EXIT_SUCCESS;
}

static void parse_args(int argc, char* argv[], char **input1_path, char **input2_path,
                        char **output_path) {
    int option_ind = 0;

    while ((option_ind = getopt(argc, argv, "io:")) != -1) {
        switch (option_ind) {
            case 'i':
                case_insensitive = true;
                break;
            case 'o':
                *output_path = optarg;
                write_output = true;
                break;
            case '?':
                print_usage_error_and_exit("Unknown Argument");
                break;
            default:
                print_error_and_exit("Unknown Error");
                break;
        }
    }

    if (argc - (optind - 1) < num_of_args) {
        print_usage_error_and_exit("Insufficient Arguments");
    }

    *input1_path = argv[optind];
    *input2_path = argv[optind + 1];
}

static void print_error_and_exit(const char *message) {
    fprintf(stderr, "%s: %s\n", program_name, message);
    exit(EXIT_FAILURE);
}

static void print_usage_error_and_exit(const char *message) {
    fprintf(stdout, "%s: %s - USAGE: %s [-i] [-o output.txt] input1.txt input2.txt\n",
            program_name, message, program_name);
    exit(EXIT_FAILURE);
}

static void try_open_file(FILE **file, const char *file_path, char *mode) {
    if (file_path != NULL) {
        *file = fopen(file_path, mode);
    
        if (*file == NULL) {
            print_error_and_exit("Error occurred trying to open file");
        }
    }
}

static void close(FILE *file) {
    if (file != NULL) {
        fclose(file);
    }
}

static void work(FILE *input1, FILE *input2, FILE *output) {
    int line = 1;
    
    char c1 = fgetc(input1);
    char c2 = fgetc(input2);
    
    while (c1 != EOF || c2 != EOF) {
        int num_diff_char = 0;
        
        while (is_eof_or_newline(c1, c2) == 0) {
            if (case_insensitive) {
                if (tolower(c1) != tolower(c2)) {
                    num_diff_char++;
                }
            } else {
                if (c1 != c2) {
                    num_diff_char++;
                }
            }
            
            c1 = fgetc(input1);
            c2 = fgetc(input2);
        }

        if (num_diff_char > 0) print_diff(line, num_diff_char, output);
        
        if (is_c_eof_or_newline(c1) == 1) {
            c1 = fgetc(input1);
        } else {
            c1 = skip_to_next_char(input1);
        }

        if (is_c_eof_or_newline(c2) == 1) {
            c2 = fgetc(input2);
        } else {
            c2 = skip_to_next_char(input2);
        }
        
        line++;
    }
}

static void print_diff(int lin, int chr, FILE *output) {
    if (write_output) {
        fprintf(output, "Line: %d, characters: %d\n", lin, chr);
    } else {
        fprintf(stdout, "Line: %d, characters: %d\n", lin, chr);
    }
}

static int is_eof_or_newline(char c1, char c2) {
    if (c1 == '\n') return 1;
    if (c1 == EOF) return 1;
    if (c2 == '\n') return 1;
    if (c2 == EOF) return 1;
    return 0;
}

static int is_c_eof_or_newline(char c) {
    if (c == '\n') return 1;
    if (c == EOF) return 1;

    return 0;
}

static char skip_to_next_char(FILE *file) {
    char c = fgetc(file);

    while (is_c_eof_or_newline(c) == 0) {
        c = fgetc(file);
    }
    return fgetc(file);
}
