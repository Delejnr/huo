#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "base_util.h"
#include "core_functions.h"
#include "process_defs.h"
#include "execute.h"

struct Map * make_args_map(struct Tree * ast, struct Tree_map * defined, struct Map * let_map, int idx){
    struct Map * arguments = malloc(sizeof(struct Map));
    arguments->size = 0;
    for(int i = 0; i < ast->size; i++){
        struct Keyval * store = malloc(sizeof(struct Keyval));
        struct Value val = execute(ast->children[i], defined, let_map);
        arguments->members[i] = store;
        arguments->members[i]->key = copy_value_heap(&defined->trees[idx]->children[i+1]->content);
        arguments->members[i]->val = copy_value_heap(&val);
        arguments->size++;
    }
    return arguments;
}

struct Tree * populate_args(struct Map * arguments, struct Tree * ast){
    if(ast->type == 'k' && !ast->size){
        for(int i = 0; i < arguments->size; i++){
            if(string_matches(&arguments->members[i]->key->data.str, &ast->content.data.str)){
                ast->content = copy_value_stack(arguments->members[i]->val);
                if(arguments->members[i]->val->type == 's'){
                    ast->type = 's';
                }
                else if(arguments->members[i]->val->type == 'k') {
                    ast->type = 'k';
                }
                else if(arguments->members[i]->val->type == 'a') {
                    ast->type = 'a';
                } else {
                    ast->type = 'n';
                }
                return ast;
            }
        }
    }
    if(ast->type == 'a'){
        populate_array(arguments, ast->content.data.array);
    }
    else {
        for(int i = 0; i < ast->size; i++){
            populate_args(arguments, ast->children[i]);
        }
    }
    return ast;
}

void populate_array(struct Map * arguments, struct Value_array * array){
    for(int i = 0; i < array->size; i++){
        if(array->values[i]->type == 'k'){
            for(int l = 0; l < arguments->size; l++){
                if(string_matches(&array->values[i]->data.str, &arguments->members[i]->key->data.str)){
                        array->values[i] = copy_value_heap(arguments->members[i]->val);
                        if (array->values[i]->type != 'k') {
                            break;
                        }
                }
            }
        } else if(array->values[i]->type == 'a'){
            populate_array(arguments, array->values[i]->data.array);
        }
    }
}

struct Tree * get_defined_body(struct Tree * function){
    // just pulls the function body out of a (def...)
    int check = 1;
    int index = 1;
    while(check){
        if(!function->children[index]->size){
            index++;
        } else {
            check = 0;
            function = function->children[index];
        }
    }
    return function;
}

int is_defined_func(struct Tree_map * defined, struct String key){
    assert(string_is_sane(&key));
    for(int i = 0; i < defined->size; i++){
        if(string_matches(defined->names[i], &key)){
            return i;
        }
    }
    return -1;
}
