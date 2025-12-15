#include <stdio.h>
#include <string.h>
#include <ctype.h>

void reverse(char *begin, char *end) {
    char temp;
    while (begin < end) {
        temp = *begin;
        *begin = *end;
        *end = temp;
        begin++;
        end--;
    }
}

void reverseWords(char *str) {
    if (*str == '\0') return;

    char *start = str;
    char *temp = str;

    while (isspace(*start)) {
        start++;
    }

    int len = strlen(start);
    reverse(start, start + len - 1);

    char *result = str;
    char *word_begin = start;
    char *word_end;

    while (*start) {
        while (isspace(*start)) {
            start++;
        }

        if (*start == '\0') {
            break;
        }

        word_begin = start;
        while (*start && !isspace(*start)) {
            start++;
        }
        word_end = start - 1;

        reverse(word_begin, word_end);

        char *p = word_begin;
        while (p <= word_end) {
            *result++ = *p++;
        }

        if (*start && !isspace(*start)) {
            *result++ = ' ';
        }
    }

    if (result > str && isspace(*(result - 1))) {
        result--;
    }

    *result = '\0';
}

int main() {
    char line[50001];

    while (fgets(line, sizeof(line), stdin) != NULL) {
        line[strcspn(line, "\n")] = '\0';
        reverseWords(line);
        printf("%s\n", line);
    }

    return 0;
}
