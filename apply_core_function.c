#include "constants.h"
#include "structures.h"
#include "core_functions.h"
#include "base_util.h"

struct Value apply_core_function(struct Tree * ast, struct Value a, struct Value b){

    if(ast->type == 'k'){
        if(string_matches(&ast->content.data.str, &concat_const)){
            a = concat(a, b);
        }
        else if(string_matches(&ast->content.data.str, &index_const)){
            a = array_index(a, b);
        }
        else if(string_matches(&ast->content.data.str, &push_const)){
            a = array_push(a, b);
        }
        else if(string_matches(&split_const, &ast->content.data.str)){
            a = split_string(a, b);
        }
    }
    else if(ast->type == '*'){
        a = mul(a, b);
    }
    else if(ast->type == '+'){
        a = add(a, b);
    }
    else if(ast->type == '-'){
        a = sub(a, b);
    }
    else if(ast->type == '/'){
        a = divide(a, b);
    }
    else if(ast->type == '!'){
        a = not(a, b);
    }
    else if(ast->type == '='){
        a = equals(a, b);
    }
    else if(ast->type == '>'){
        a = greater_than(a, b);
    }
    else if(ast->type == '<'){
        a = greater_than(b, a);
    }
    return a;
}
