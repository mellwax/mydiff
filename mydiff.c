#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

static bool writeOutputFile = false;
static bool caseInsensitive = false;

static void parseOptions(int argc, char *argv[]);
static int getShorterStringLength(const char *str1, const char* str2);
int compare(const char *str1, const char *str2);
static void printUsageErrorAndFail(char *message, char *programName);
void printErrorMessageAndFail(char *message, char *programname);

int main(int argc, char *argv[]) {

    parseOptions(argc, argv);
    printf("reached end\n");

    return EXIT_SUCCESS;
}

static void parseOptions(int argc, char *argv[]) {
    int optionIndex = 0;

    while ((optionIndex = getopt(argc, argv, "io:")) != -1) {
        switch (optionIndex) {
            case 'i':
                caseInsensitive = true;
                break;
            case 'o':
                writeOutputFile = true;
                break;
            case '?':
                printUsageErrorAndFail("Illegal Option", argv[0]);
                break;
            default:
                printErrorMessageAndFail("Unknown Error", argv[0]);
                break;
        }
    }
}

static int getShorterStringLength(const char *str1, const char *str2) {
    int strLength1 = strlen(str1);
    int strLength2 = strlen(str2);


    if (strLength1 < strLength2) {
        return strLength1;
    } else if (strLength2 < strLength1) {
        return strLength2;
    }
    
    return strLength1;
}

int compare(const char *str1, const char *str2) {

    int result;
    int n = getShorterStringLength(str1, str2);

    if (caseInsensitive) {
        result = strncasecmp(str1, str2, n);
    } else {
        result = strncmp(str1, str2, n);
    }
    
    return result;
}

static void printUsageErrorAndFail(char *message, char *programName) {
    fprintf(stderr, "%s: USAGE: %s [-i] [-o outfile] input1 input2\n" ,message, programName);
    exit(EXIT_FAILURE);
}

void printErrorMessageAndFail(char *message, char *programName) {
    fprintf(stderr, "%s: %s\n", programName, message);
    exit(EXIT_FAILURE);
}

