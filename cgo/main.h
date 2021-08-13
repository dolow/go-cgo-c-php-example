#ifndef CGO_MAIN_H
#define CGO_MAIN_H

#include <stdlib.h>

typedef struct MyChildStruct {
    char* key;
    char* value;
} MyChildStruct;

typedef struct MyStruct {
    char* string;
    int integer;
    MyChildStruct* child;
} MyStruct;

#endif
