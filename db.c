#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGS 8
#define MAX_ENTRIES 100

struct entry {
    char *key;
    char *value;
};

struct entry table[MAX_ENTRIES];
int count = 0;

int find(const char *key) {
    for (int i = 0; i < count; i++) {
        if (strcmp(table[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

int main(void) {

    char line[256];

    printf("mini-db へようこそ（exit で終了）\n");

    while (1) {
        printf("db> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        char *args[MAX_ARGS];
        int nargs = 0;

        char *token = strtok(line, " ");
        while (token != NULL && nargs < MAX_ARGS) {
            args[nargs] = token;
            nargs++;
            token = strtok(NULL, " ");
        }

        if (nargs == 0) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            break;
        } else if (strcmp(args[0], "set") == 0) {
            if (nargs != 3) {
                printf("使い方: set <key> <value>\n");
                continue;
            }

            int i = find(args[1]);
            if (i >= 0) {
                free(table[i].value);
                table[i].value = strdup(args[2]);
            } else {
                if (count >= MAX_ENTRIES) {
                    printf("棚がいっぱいです\n");
                    continue;
                }
                table[count].key = strdup(args[1]);
                table[count].value = strdup(args[2]);
                count++;
            }
            printf("OK\n");
        } else if (strcmp(args[0], "get") == 0) {
            if (nargs != 2) {
                printf("使い方: get <key>\n");
                continue;
            }

            int i = find(args[1]);
            if (i >= 0) {
                printf("%s\n", table[i].value);
            } else {
                printf("(見つかりません)\n");
            }
        } else {
            printf("知らないコマンドです: %s\n", args[0]);
        }
    }

    printf("さようなら\n");

    return 0;
}
