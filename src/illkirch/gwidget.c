
#include <regex.h>
#include <stdlib.h>
#include <stdio.h>
#include "widget_extractor.h"
#include "widget.h"

// Function to split arguments
int split_args(const char *args, char ***argv) {
    // Example implementation: split by commas
    int argc = 0;
    char *arg_copy = strdup(args);
    char *token = strtok(arg_copy, ",");
    while (token != NULL) {
        (*argv) = realloc(*argv, sizeof(char *) * (++argc));
        (*argv)[argc - 1] = strdup(token);
        token = strtok(NULL, ",");
    }
    free(arg_copy);
    return argc;
}

// Function to free split arguments
void free_args(int argc, char **argv) {
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
    free(argv);
}

void extract_widgets(const char *content) {
    const char *patterns[] = {
        "CreateWindowWidget\\(([^)]+)\\);",
        "CreateButtonWidget\\(([^)]+)\\);",
        "CreateTextBoxWidget\\(([^)]+)\\);",
        "CreateCheckboxWidget\\(([^)]+)\\);"
    };

    regex_t regex;
    regmatch_t matches[2];
    for (int i = 0; i < 4; i++) {
        if (regcomp(&regex, patterns[i], REG_EXTENDED)) {
            perror("Could not compile regex");
            return;
        }

        const char *p = content;
        while (!regexec(&regex, p, 2, matches, 0)) {
            char match[256];
            snprintf(match, matches[1].rm_eo - matches[1].rm_so + 1, "%s", p + matches[1].rm_so);
            reconstruct_widget(i, match);
            p += matches[0].rm_eo;
        }

        regfree(&regex);
    }
}

void reconstruct_widget(int widget_type, const char *args) {
    char **argv = NULL;
    int argc = split_args(args, &argv);

    switch (widget_type) {
        case 0:  // Window
            CreateWindowWidget(argv[0], argv[1], argv[2], argv[3], argv[4]);
            break;
        case 1:  // Button
            CreateButtonWidget(argv[0], argv[1], argv[2], argv[3], argv[4]);
            break;
        case 2:  // TextBox
            CreateTextBoxWidget(argv[0], argv[1], argv[2], argv[3], argv[4]);
            break;
        case 3:  // Checkbox
            CreateCheckboxWidget(argv[0], argv[1], argv[2], argv[3]);
            break;
    }

    free_args(argc, argv);
}
