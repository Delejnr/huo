#include <assert.h>
#include "../structures.h"
#include "../execute.h"
#include "../base_util.h"

struct Value read_file(struct String file_name){
    assert(string_is_sane(&file_name));
    struct Value result;
    FILE *fp;
    fp = fopen(file_name.body, "r");
    if(fp == NULL){
        ERROR("Cannot find file: \"%s\"", file_name.body);
        /*result.type = 'u';
        return result; */
    }
    struct String file = {
        .length = 0,
        .body = NULL
    };

    char c;
    while ((c = fgetc(fp)) != EOF){
        if(c != '\n'){
            RESIZE(file.body, file.length + 1);
            file.body[file.length] = c;
            file.length++;
        }
    }
    fclose(fp);
    
    // Null character at end
    RESIZE(file.body, file.length + 1);
    file.body[file.length] = 0;
    assert(string_is_sane(&file));
    result.type = 's';
    result.data.str = file;
    return result;
}
