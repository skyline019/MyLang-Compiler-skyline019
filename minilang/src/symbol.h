#ifndef SYMBOL_H
#define SYMBOL_H

#include "ast.h"

// 明确的类型定义
#define TYPE_INT 0
#define TYPE_FLOAT 1
#define TYPE_STRING 2
#define TYPE_INT_ARRAY 3
#define TYPE_FLOAT_ARRAY 4
#define TYPE_STRING_ARRAY 5
#define TYPE_FUNCTION 6

// 符号表结构
typedef struct
{
    char *name;
    int int_value;
    float float_value;
    char *string_value;
    int type;
    int array_size;
    void *array_data;
    bool is_initialized;

    // 函数相关字段
    bool is_function;
    char **param_types;
    int param_count;
    ASTNode *function_def;
} Symbol;

// 符号表函数
Symbol *find_symbol(const char *name);
void set_symbol(const char *name, int int_value, float float_value, char *string_value, int type, bool is_function);
void set_function_params(const char *name, char **param_types, int param_count);
void set_function_def(const char *name, ASTNode *func_def);
int get_type_from_string(const char *type_str);
void free_current_symbol_table();
void ast_free_symbol_table();

#endif // SYMBOL_H