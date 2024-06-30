
#include <stdio.h>
#include <stdlib.h>
#include "widget_extractor.h"

void open_and_parse_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Unable to open file");
        return;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char *)malloc(length + 1);
    if (content) {
        fread(content, 1, length, file);
        content[length] = '\0';
    }
    fclose(file);

    extract_widgets(content);
    free(content);
}
