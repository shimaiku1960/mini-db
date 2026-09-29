#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGS 8
#define MAX_ENTRIES 100
#define DB_FILE "data.db"

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

void save(void) {
    FILE *fp = fopen(DB_FILE, "w");
    if (fp == NULL) {
        printf("保存できませんでした\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s %s\n", table[i].key, table[i].value);
    }

    fclose(fp);
}

void load(void) {
    FILE *fp = fopen(DB_FILE, "r");
    if (fp == NULL) {
        return;
    }

    char key[256];
    char value[256];
    while (count < MAX_ENTRIES && fscanf(fp, "%255s %255s", key, value) == 2) {
        table[count].key = strdup(key);
        table[count].value = strdup(value);
        count++;
    }

    fclose(fp);
}

int main(void) {

    char line[256];

    load();

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
        } else if (strcmp(args[0], "del") == 0) {
            if (nargs != 2) {
                printf("使い方: del <key>\n");
                continue;
            }

            int i = find(args[1]);
            if (i >= 0) {
                free(table[i].key);
                free(table[i].value);
                table[i] = table[count - 1];
                count--;
                printf("OK\n");
            } else {
                printf("(見つかりません)\n");
            }
        } else {
            printf("知らないコマンドです: %s\n", args[0]);
        }
    }

    save();

    printf("さようなら\n");

    return 0;
}
