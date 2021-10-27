#include <stdio.h>
#include <stdlib.h>

char line_buffer[256] = {0};
char * pointers[32] = {0};
unsigned char sizes[32] = {0};
int block_head = 0;

void create(void) {
    if (block_head >= 32) {
        puts("Out of blocks.");
    } else {
        fputs("Size: ", stdout);
        if (fgets(line_buffer, sizeof line_buffer, stdin) == NULL) {
            puts("Read error!");
            exit(EXIT_FAILURE);
        }
        fflush(stdin);
        int size = atoi(line_buffer);
        if (size < 256) {
            pointers[block_head] = malloc(size);
            sizes[block_head] = size;
            fputs("Data: ", stdout);
            if (fgets(pointers[block_head++], size, stdin) == NULL) {
                puts("Read error!");
                exit(EXIT_FAILURE);
            }
            fflush(stdin);
            puts("Done.");
        } else
            puts("Too big.");
    }
}

void read(void) {
    fputs("Index: ", stdout);
    if (fgets(line_buffer, sizeof line_buffer, stdin) == NULL) {
        puts("Read error!");
        exit(EXIT_FAILURE);
    }
    fflush(stdin);
    int index = atoi(line_buffer);
    if (pointers[index] == NULL)
        puts("Does not exist.");
    else {
        fwrite(pointers[index], sizes[index], 1, stdout);
        putchar('\n');
    }
}

void update(void) {
    fputs("Index: ", stdout);
    if (fgets(line_buffer, sizeof line_buffer, stdin) == NULL) {
        puts("Read error!");
        exit(EXIT_FAILURE);
    }
    fflush(stdin);
    int index = atoi(line_buffer);
    if (pointers[index] == NULL)
        puts("Does not exist.");
    else {
        fputs("Data: ", stdout);
        if (fgets(pointers[index], sizes[index], stdin) == NULL) {
            puts("Read error!");
            exit(EXIT_FAILURE);
        }
        fflush(stdin);

        puts("Done.");
    }
}

void delete(void) {
    fputs("Index: ", stdout);
    if (fgets(line_buffer, sizeof line_buffer, stdin) == NULL) {
        puts("Read error!");
        exit(EXIT_FAILURE);
    }
    fflush(stdin);
    int index = atoi(line_buffer);
    if (pointers[index] == NULL)
        puts("Does not exist.");
    else {
        free(pointers[index]);
        puts("Done.");
    }
}

void leave(void) {
    exit(0);
}

int get_choice(void) {
    fwrite("> ", 2, 1, stdout);
    if (fgets(line_buffer, sizeof line_buffer, stdin) == NULL) {
        puts("Read error!");
        exit(EXIT_FAILURE);
    }
    fflush(stdin);
    return atoi(line_buffer) - 1;
}

void (*actions[])(void) = {create, read, update, delete, leave};

char menu[] = "---- CRUD Notes ----\n1: Create\n2: Read\n3: Update\n4: Delete\n5: Leave\n--------------------";

int main(void) {
    while (1) {
        puts(menu);
        actions[get_choice()]();
    }
}
