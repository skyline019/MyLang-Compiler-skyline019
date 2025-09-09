#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// 明确的类型定义
#define TYPE_INT 0
#define TYPE_FLOAT 1
#define TYPE_STRING 2
#define TYPE_INT_ARRAY 3
#define TYPE_FLOAT_ARRAY 4
#define TYPE_STRING_ARRAY 5
#define TYPE_FUNCTION 6

// 简单的符号表结构
typedef struct
{
    char *name;
    int int_value;
    float float_value;
    char *string_value;
    int type; // 使用明确的类型定义
    int array_size;
    void *array_data;
    bool is_initialized;

    // 函数相关字段
    bool is_function;
    char **param_types;
    int param_count;
    ASTNode
        *function_def; // 改为存储整个函数定义节点
} Symbol;

// 全局符号表
static Symbol *symbol_table = NULL;
static int symbol_count = 0;
static int symbol_capacity = 0;

static char *print_buffer = NULL;
static int print_buffer_size = 0;
static int print_buffer_capacity = 0;

static void append_to_print_buffer(const char *str)
{
    if (!str)
        return;

    int len = strlen(str);
    if (print_buffer_size + len + 1 > print_buffer_capacity)
    {
        int new_capacity = print_buffer_capacity == 0 ? 256 : print_buffer_capacity * 2;
        while (print_buffer_size + len + 1 > new_capacity)
        {
            new_capacity *= 2;
        }
        print_buffer = realloc(print_buffer, new_capacity);
        if (!print_buffer)
        {
            perror("Memory allocation failed for print buffer");
            exit(1);
        }
        print_buffer_capacity = new_capacity;
    }

    strcpy(print_buffer + print_buffer_size, str);
    print_buffer_size += len;
}

// 查找符号
static Symbol *find_symbol(const char *name)
{
    if (name == NULL)
    {
        return NULL;
    }

    if (symbol_table == NULL || symbol_count == 0)
    {
        return NULL;
    }

    for (int i = 0; i < symbol_count; i++)
    {
        if (symbol_table[i].name != NULL && strcmp(symbol_table[i].name, name) == 0)
        {
            return &symbol_table[i];
        }
    }

    return NULL;
}

// 添加或更新符号
static void set_symbol(const char *name, int int_value, float float_value, char *string_value, int type, bool is_function)
{
    if (name == NULL)
    {
        fprintf(stderr, "Error: NULL variable name\n");
        exit(1);
    }

    for (int i = 0; i < symbol_count; i++)
    {
        if (symbol_table[i].name != NULL && strcmp(symbol_table[i].name, name) == 0)
        {
            symbol_table
                [i]
                    .int_value = int_value;
            symbol_table
                [i]
                    .float_value = float_value;
            if (symbol_table[i].string_value != NULL)
            {
                free(symbol_table[i].string_value);
            }
            symbol_table
                [i]
                    .string_value = string_value ? strdup(string_value) : NULL;
            symbol_table
                [i]
                    .type = type;
            symbol_table
                [i]
                    .is_initialized = true;
            symbol_table
                [i]
                    .is_function = is_function;
            return;
        }
    }

    if (symbol_count >= symbol_capacity)
    {
        symbol_capacity = symbol_capacity == 0 ? 8 : symbol_capacity * 2;
        symbol_table = realloc(symbol_table, symbol_capacity * sizeof(Symbol));
        if (!symbol_table)
        {
            perror("Memory allocation failed");
            exit(1);
        }
    }

    symbol_table
        [symbol_count]
            .name = strdup(name);
    symbol_table
        [symbol_count]
            .int_value = int_value;
    symbol_table
        [symbol_count]
            .float_value = float_value;
    symbol_table
        [symbol_count]
            .string_value = string_value ? strdup(string_value) : NULL;
    symbol_table
        [symbol_count]
            .type = type;
    symbol_table
        [symbol_count]
            .array_size = 0;
    symbol_table
        [symbol_count]
            .array_data = NULL;
    symbol_table
        [symbol_count]
            .is_initialized = false;
    symbol_table
        [symbol_count]
            .is_function = is_function;
    symbol_table
        [symbol_count]
            .param_types = NULL;
    symbol_table
        [symbol_count]
            .param_count = 0;
    symbol_table
        [symbol_count]
            .function_def = NULL; // 初始化为NULL
    symbol_count++;
}

// 设置函数参数
static void set_function_params(const char *name, char **param_types, int param_count)
{
    if (name == NULL || param_types == NULL)
        return;

    Symbol
        *sym = find_symbol(name);
    if (sym && sym->is_function)
    {
        sym
            ->param_types = param_types;
        sym
            ->param_count = param_count;
    }
}

// 设置函数定义
static void set_function_def(const char *name, ASTNode *func_def)
{
    if (name == NULL || func_def == NULL || func_def->type != AST_FUNCTION_DEF)
        return;

    Symbol
        *sym = find_symbol(name);
    if (sym && sym->is_function)
    {
        sym
            ->function_def = func_def;
    }
}

// 辅助函数：将类型字符串转换为类型常量
static int get_type_from_string(const char *type_str)
{
    if (type_str == NULL)
        return TYPE_INT;
    if (strcmp(type_str, "int") == 0)
        return TYPE_INT;
    if (strcmp(type_str, "float") == 0)
        return TYPE_FLOAT;
    if (strcmp(type_str, "string") == 0)
        return TYPE_STRING;
    return TYPE_INT; // 默认
}

// 只释放当前符号表（用于函数调用）
static void free_current_symbol_table()
{
    for (int i = 0; i < symbol_count; i++)
    {
        if (symbol_table[i].name != NULL)
        {
            free(symbol_table[i].name);
        }
        if (symbol_table[i].string_value != NULL)
        {
            free(symbol_table[i].string_value);
        }
        if (symbol_table[i].array_data != NULL)
        {
            if (symbol_table[i].type == TYPE_STRING_ARRAY)
            {
                char **str_array = (char **)symbol_table[i].array_data;
                for (int j = 0; j < symbol_table[i].array_size; j++)
                {
                    if (str_array[j] != NULL)
                    {
                        free(str_array[j]);
                    }
                }
            }
            free(symbol_table[i].array_data);
        }
        // 注意：不要释放param_types，因为它们是函数定义的一部分
    }
    if (symbol_table != NULL)
    {
        free(symbol_table);
    }
    symbol_table = NULL;
    symbol_count = 0;
    symbol_capacity = 0;
}

// 释放符号表
void ast_free_symbol_table()
{
    // 释放print缓冲区
    if (print_buffer)
    {
        free(print_buffer);
        print_buffer = NULL;
    }
    print_buffer_size = 0;
    print_buffer_capacity = 0;

    for (int i = 0; i < symbol_count; i++)
    {
        if (symbol_table[i].name != NULL)
        {
            free(symbol_table[i].name);
        }
        if (symbol_table[i].string_value != NULL)
        {
            free(symbol_table[i].string_value);
        }
        if (symbol_table[i].array_data != NULL)
        {
            if (symbol_table[i].type == TYPE_STRING_ARRAY)
            {
                char **str_array = (char **)symbol_table[i].array_data;
                for (int j = 0; j < symbol_table[i].array_size; j++)
                {
                    if (str_array[j] != NULL)
                    {
                        free(str_array[j]);
                    }
                }
            }
            free(symbol_table[i].array_data);
        }
        if (symbol_table[i].param_types != NULL)
        {
            for (int j = 0; j < symbol_table[i].param_count; j++)
            {
                if (symbol_table[i].param_types[j] != NULL)
                {
                    free(symbol_table[i].param_types[j]);
                }
            }
            free(symbol_table[i].param_types);
        }
    }
    if (symbol_table != NULL)
    {
        free(symbol_table);
    }
    symbol_table = NULL;
    symbol_count = 0;
    symbol_capacity = 0;
}

ASTNode *ast_new_integer(int value, int line_no)
{
    ASTNode
        *node = malloc(sizeof(ASTNode));
    node
        ->type = AST_INTEGER;
    node
        ->line_no = line_no;
    node
        ->int_value = value;
    return node;
}

ASTNode *ast_new_float(float value, int line_no)
{
    ASTNode
        *node = malloc(sizeof(ASTNode));
    node
        ->type = AST_FLOAT;
    node
        ->line_no = line_no;
    node
        ->float_value = value;
    return node;
}

ASTNode *ast_new_string(char *value, int line_no)
{
    ASTNode
        *node = malloc(sizeof(ASTNode));
    node
        ->type = AST_STRING;
    node
        ->line_no = line_no;
    node
        ->string_value = strdup(value);
    return node;
}

ASTNode *ast_new_variable(char *name, int line_no)
{
    ASTNode
        *node = malloc(sizeof(ASTNode));
    node
        ->type = AST_VARIABLE;
    node
        ->line_no = line_no;
    node
        ->string_value = strdup(name);
    return node;
}

ASTNode *ast_new_function_def(char *func_name, char *return_type, ASTNode *param_block,
                              ASTNode
                                  *body,
                              int line_no)
{
    ASTNode
        *node = malloc(sizeof(ASTNode));
    if (node == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for function definition\n");
        exit(1);
    }

    node
        ->type = AST_FUNCTION_DEF;
    node
        ->line_no = line_no;
    node
        ->func_def.func_name = strdup(func_name);
    node
        ->func_def.return_type = strdup(return_type);

    // 从block节点中提取参数
    if (param_block && param_block->type == AST_BLOCK)
    {
        node
            ->func_def.params = param_block->block.statements;
        node
            ->func_def.param_count = param_block->block.count;

        // 释放block节点但不释放statements数组
        free(param_block);
    }
    else
    {
        node
            ->func_def.params = NULL;
        node
            ->func_def.param_count = 0;
    }

    node
        ->func_def.body = body;

    printf("Creating function %s with %d parameters\n", func_name, node->func_def.param_count);

    // 设置函数定义到符号表
    set_function_def(func_name, node);

    return node;
}

ASTNode *ast_new_formatted_print(char *format_string, ASTNode **args,
                                 int arg_count, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_FORMATTED_PRINT;
    node->line_no = line_no;
    node->formatted_print.format_string = strdup(format_string);
    node->formatted_print.args = args;
    node->formatted_print.arg_count = arg_count;
    return node;
}

ASTNode *ast_new_array_declaration(char *var_name, ASTNode *size, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ARRAY_DECLARATION;
    node->line_no = line_no;
    node->array_decl.var_name = strdup(var_name);
    node->array_decl.size = size;
    return node;
}

ASTNode *ast_new_array_access(char *var_name, ASTNode *index, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ARRAY_ACCESS;
    node->line_no = line_no;
    node->array_access.var_name = strdup(var_name);
    node->array_access.index = index;
    return node;
}

ASTNode *ast_new_array_assignment(ASTNode *array_access, ASTNode *value, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ARRAY_ASSIGNMENT;
    node->line_no = line_no;
    node->array_assignment.array_access = array_access;
    node->array_assignment.value = value;
    return node;
}

ASTNode *ast_new_binary_op(char *op, ASTNode *left, ASTNode *right, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY_OP;
    node->line_no = line_no;
    node->binary.op = strdup(op);
    node->binary.left = left;
    node->binary.right = right;
    return node;
}

ASTNode *ast_new_assignment(ASTNode *var, ASTNode *expr, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ASSIGNMENT;
    node->line_no = line_no;
    node->binary.left = var;
    node->binary.right = expr;
    return node;
}

ASTNode *ast_new_if(ASTNode *cond, ASTNode *then_body, ASTNode *else_body, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_IF;
    node->line_no = line_no;
    node->if_stmt.cond = cond;
    node->if_stmt.then_body = then_body;
    node->if_stmt.else_body = else_body;
    return node;
}

ASTNode *ast_new_while(ASTNode *cond, ASTNode *body, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_WHILE;
    node->line_no = line_no;
    node->while_loop.cond = cond;
    node->while_loop.body = body;
    return node;
}

ASTNode *ast_new_for(ASTNode *init, ASTNode *cond, ASTNode *update, ASTNode *body, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_FOR;
    node->line_no = line_no;
    node->for_loop.init = init;
    node->for_loop.cond = cond;
    node->for_loop.update = update;
    node->for_loop.body = body;
    return node;
}

ASTNode *ast_new_block(ASTNode **statements, int count, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BLOCK;
    node->line_no = line_no;
    node->block.statements = statements;
    node->block.count = count;
    return node;
}

ASTNode *ast_new_declaration(char *var_name, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_DECLARATION;
    node->line_no = line_no;
    node->decl.var_name = strdup(var_name);
    node->decl.init_value = NULL;
    return node;
}

ASTNode *ast_new_param_declaration(char *var_name, char *var_type, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_PARAM_DECLARATION; // 使用新类型
    node->line_no = line_no;
    node->decl.var_name = strdup(var_name);
    node->decl.var_type = strdup(var_type);
    node->decl.init_value = NULL;
    return node;
}

ASTNode *ast_new_declaration_init(char *var_name, ASTNode *init_value, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_DECLARATION_INIT;
    node->line_no = line_no;
    node->decl.var_name = strdup(var_name);
    node->decl.init_value = init_value;
    return node;
}

ASTNode *ast_new_function_call(char *func_name, ASTNode **args, int arg_count, int line_no)
{
    if (func_name == NULL)
    {
        fprintf(stderr, "Error: NULL function name\n");
        exit(1);
    }

    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }

    memset(node, 0, sizeof(ASTNode));

    node->type = AST_FUNCTION_CALL;
    node->line_no = line_no;
    node->func_call.func_name = strdup(func_name);
    if (node->func_call.func_name == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(node);
        exit(1);
    }

    if (arg_count > 0)
    {
        node->func_call.args = malloc(sizeof(ASTNode *) * arg_count);
        if (node->func_call.args == NULL)
        {
            fprintf(stderr, "Error: Memory allocation failed\n");
            free(node->func_call.func_name);
            free(node);
            exit(1);
        }

        for (int i = 0; i < arg_count; i++)
        {
            node->func_call.args[i] = args[i];
        }
    }
    else
    {
        node->func_call.args = NULL;
    }

    node->func_call.arg_count = arg_count;

    return node;
}

ASTNode *ast_new_return(ASTNode *expr, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_RETURN;
    node->line_no = line_no;
    node->binary.left = expr;
    return node;
}

ASTNode *ast_new_empty(int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_EMPTY;
    node->line_no = line_no;
    return node;
}

void ast_print_indent(int indent)
{
    for (int i = 0; i < indent; i++)
    {
        printf("  ");
    }
}

void ast_print(ASTNode *node, int indent)
{
    if (!node)
        return;

    ast_print_indent(indent);
    printf("Line %d: ", node->line_no);

    switch (node->type)
    {
    case AST_INTEGER:
        printf("INTEGER(%d)\n", node->int_value);
        break;
    case AST_FLOAT:
        printf("FLOAT(%f)\n", node->float_value);
        break;
    case AST_STRING:
        printf("STRING(\"%s\")\n", node->string_value);
        break;
    case AST_VARIABLE:
        printf("VARIABLE(%s)\n", node->string_value);
        break;
    case AST_FUNCTION_DEF:
        printf("FUNCTION_DEF(%s, returns: %s)\n",
               node->func_def.func_name, node->func_def.return_type);
        for (int i = 0; i < node->func_def.param_count; i++)
        {
            ast_print(node->func_def.params[i], indent + 1);
        }
        ast_print(node->func_def.body, indent + 1);
        break;
    case AST_FORMATTED_PRINT:
        printf("FORMATTED_PRINT(\"%s\")\n", node->formatted_print.format_string);
        for (int i = 0; i < node->formatted_print.arg_count; i++)
        {
            ast_print(node->formatted_print.args[i], indent + 1);
        }
        break;
    case AST_PARAM_DECLARATION:
        printf("PARAM_DECLARATION(%s: %s)\n", node->decl.var_name, node->decl.var_type);
        break;
    case AST_ARRAY_DECLARATION:
        printf("ARRAY_DECLARATION(%s", node->array_decl.var_name);
        if (node->array_decl.size)
        {
            printf("[");
            ast_print(node->array_decl.size, 0);
            printf("]");
        }
        printf(")\n");
        break;
    case AST_ARRAY_ACCESS:
        printf("ARRAY_ACCESS(%s[", node->array_access.var_name);
        ast_print(node->array_access.index, 0);
        printf("])\n");
        break;
    case AST_ARRAY_ASSIGNMENT:
        printf("ARRAY_ASSIGNMENT\n");
        ast_print(node->array_assignment.array_access, indent + 1);
        ast_print(node->array_assignment.value, indent + 1);
        break;
    case AST_BINARY_OP:
        printf("BINARY_OP(%s)\n", node->binary.op);
        ast_print(node->binary.left, indent + 1);
        ast_print(node->binary.right, indent + 1);
        break;
    case AST_ASSIGNMENT:
        printf("ASSIGNMENT\n");
        ast_print(node->binary.left, indent + 1);
        ast_print(node->binary.right, indent + 1);
        break;
    case AST_IF:
        printf("IF\n");
        ast_print(node->if_stmt.cond, indent + 1);
        ast_print(node->if_stmt.then_body, indent + 1);
        if (node->if_stmt.else_body)
        {
            ast_print(node->if_stmt.else_body, indent + 1);
        }
        break;
    case AST_WHILE:
        printf("WHILE\n");
        ast_print(node->while_loop.cond, indent + 1);
        ast_print(node->while_loop.body, indent + 1);
        break;
    case AST_FOR:
        printf("FOR\n");
        ast_print(node->for_loop.init, indent + 1);
        ast_print(node->for_loop.cond, indent + 1);
        ast_print(node->for_loop.update, indent + 1);
        ast_print(node->for_loop.body, indent + 1);
        break;
    case AST_BLOCK:
        printf("BLOCK (%d statements)\n", node->block.count);
        for (int i = 0; i < node->block.count; i++)
        {
            ast_print(node->block.statements[i], indent + 1);
        }
        break;
    case AST_DECLARATION:
        printf("DECLARATION(%s)\n", node->decl.var_name);
        break;
    case AST_DECLARATION_INIT:
        printf("DECLARATION_INIT(%s)\n", node->decl.var_name);
        ast_print(node->decl.init_value, indent + 1);
        break;
    case AST_FUNCTION_CALL:
        printf("FUNCTION_CALL(%s)\n", node->func_call.func_name);
        if (node->func_call.args != NULL)
        {
            for (int i = 0; i < node->func_call.arg_count; i++)
            {
                if (node->func_call.args[i] != NULL)
                {
                    ast_print(node->func_call.args[i], indent + 1);
                }
            }
        }
        break;
    case AST_RETURN:
        printf("RETURN\n");
        ast_print(node->binary.left, indent + 1);
        break;
    case AST_EMPTY:
        printf("EMPTY\n");
        break;
    default:
        printf("UNKNOWN\n");
    }
}

void ast_free(ASTNode *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case AST_STRING:
        free(node->string_value);
        break;
    case AST_VARIABLE:
        free(node->string_value);
        break;
    case AST_FUNCTION_DEF:
        free(node->func_def.func_name);
        free(node->func_def.return_type);
        for (int i = 0; i < node->func_def.param_count; i++)
        {
            ast_free(node->func_def.params[i]);
        }
        free(node->func_def.params);
        ast_free(node->func_def.body);
        break;
    case AST_FORMATTED_PRINT:
        free(node->formatted_print.format_string);
        for (int i = 0; i < node->formatted_print.arg_count; i++)
        {
            ast_free(node->formatted_print.args[i]);
        }
        free(node->formatted_print.args);
        break;
    case AST_PARAM_DECLARATION:
        free(node->decl.var_name);
        free(node->decl.var_type);
        break;
    case AST_ARRAY_DECLARATION:
        free(node->array_decl.var_name);
        ast_free(node->array_decl.size);
        break;
    case AST_ARRAY_ACCESS:
        free(node->array_access.var_name);
        ast_free(node->array_access.index);
        break;
    case AST_ARRAY_ASSIGNMENT:
        ast_free(node->array_assignment.array_access);
        ast_free(node->array_assignment.value);
        break;
    case AST_BINARY_OP:
        free(node->binary.op);
        ast_free(node->binary.left);
        ast_free(node->binary.right);
        break;
    case AST_ASSIGNMENT:
        ast_free(node->binary.left);
        ast_free(node->binary.right);
        break;
    case AST_IF:
        ast_free(node->if_stmt.cond);
        ast_free(node->if_stmt.then_body);
        ast_free(node->if_stmt.else_body);
        break;
    case AST_WHILE:
        ast_free(node->while_loop.cond);
        ast_free(node->while_loop.body);
        break;
    case AST_FOR:
        ast_free(node->for_loop.init);
        ast_free(node->for_loop.cond);
        ast_free(node->for_loop.update);
        ast_free(node->for_loop.body);
        break;
    case AST_BLOCK:
        for (int i = 0; i < node->block.count; i++)
        {
            ast_free(node->block.statements[i]);
        }
        free(node->block.statements);
        break;
    case AST_DECLARATION:
        free(node->decl.var_name);
        break;
    case AST_DECLARATION_INIT:
        free(node->decl.var_name);
        ast_free(node->decl.init_value);
        break;
    case AST_FUNCTION_CALL:
    {
        if (node->func_call.func_name != NULL)
        {
            free(node->func_call.func_name);
        }
        if (node->func_call.args != NULL)
        {
            for (int i = 0; i < node->func_call.arg_count; i++)
            {
                if (node->func_call.args[i] != NULL)
                {
                    ast_free(node->func_call.args[i]);
                }
            }
            free(node->func_call.args);
        }
        break;
    }
    case AST_RETURN:
        ast_free(node->binary.left);
        break;
    default:
        break;
    }

    free(node);
}

// 解释AST节点，返回值通过指针参数传出
void ast_interpret_node(ASTNode *node, int *int_result, float *float_result, bool *is_int)
{
    if (!node)
    {
        *is_int = true;
        *int_result = 0;
        *float_result = 0.0f;
        return;
    }

    printf("Interpreting node type: %d at line %d\n", node->type, node->line_no);

    int left_int, right_int;
    float left_float, right_float;
    bool left_is_int, right_is_int;

    switch (node->type)
    {
    case AST_BLOCK:
    {
        for (int i = 0; i < node->block.count; i++)
        {
            ast_interpret_node(node->block.statements[i], int_result, float_result, is_int);
        }
        break;
    }
    case AST_DECLARATION:
    {
        set_symbol(node->decl.var_name, 0, 0.0f, NULL, TYPE_INT, false);
        printf("Declared variable %s\n", node->decl.var_name);
        *is_int = true;
        *int_result = 0;
        *float_result = 0.0f;
        break;
    }
    case AST_DECLARATION_INIT:
    {
        int temp_int;
        float temp_float;
        bool temp_is_int;

        ast_interpret_node(node->decl.init_value, &temp_int, &temp_float, &temp_is_int);

        int type;
        char *string_value = NULL;
        if (node->decl.init_value->type == AST_STRING)
        {
            type = TYPE_STRING;
            string_value = node->decl.init_value->string_value;
        }
        else if (temp_is_int)
        {
            type = TYPE_INT;
        }
        else
        {
            type = TYPE_FLOAT;
        }

        set_symbol(node->decl.var_name, temp_int, temp_float, string_value, type, false);

        // 将返回值传递给调用者
        *int_result = temp_int;
        *float_result = temp_float;
        *is_int = temp_is_int;

        if (type == TYPE_INT)
        {
            printf("Variable %s = %d\n", node->decl.var_name, temp_int);
        }
        else if (type == TYPE_FLOAT)
        {
            printf("Variable %s = %f\n", node->decl.var_name, temp_float);
        }
        else
        {
            printf("Variable %s = \"%s\"\n", node->decl.var_name, string_value);
        }
        break;
    }
    case AST_ASSIGNMENT:
    {
        int temp_int;
        float temp_float;
        bool temp_is_int;

        ast_interpret_node(node->binary.right, &temp_int, &temp_float, &temp_is_int);
        char *var_name = node->binary.left->string_value;

        int type;
        char *string_value = NULL;
        if (node->binary.right->type == AST_STRING)
        {
            type = TYPE_STRING;
            string_value = node->binary.right->string_value;
        }
        else if (temp_is_int)
        {
            type = TYPE_INT;
        }
        else
        {
            type = TYPE_FLOAT;
        }

        set_symbol(var_name, temp_int, temp_float, string_value, type, false);

        if (type == TYPE_INT)
        {
            printf("Assigned %s = %d\n", var_name, temp_int);
        }
        else if (type == TYPE_FLOAT)
        {
            printf("Assigned %s = %f\n", var_name, temp_float);
        }
        else
        {
            printf("Assigned %s = \"%s\"\n", var_name, string_value);
        }

        *int_result = temp_int;
        *float_result = temp_float;
        *is_int = temp_is_int;
        break;
    }
    case AST_INTEGER:
    {
        *is_int = true;
        *int_result = node->int_value;
        *float_result = (float)node->int_value;
        break;
    }
    case AST_FLOAT:
    {
        *is_int = false;
        *int_result = (int)node->float_value;
        *float_result = node->float_value;
        break;
    }
    case AST_STRING:
    {
        *is_int = false;
        *int_result = 0;
        *float_result = 0.0f;
        break;
    }
    case AST_VARIABLE:
    {
        Symbol *sym = find_symbol(node->string_value);
        if (sym == NULL)
        {
            fprintf(stderr, "Error: Variable '%s' not found\n", node->string_value);
            exit(1);
        }
        *is_int = (sym->type == TYPE_INT);
        *int_result = sym->int_value;
        *float_result = sym->float_value;
        break;
    }
    case AST_FUNCTION_DEF:
    {
        // 将函数定义注册到符号表
        set_symbol(node->func_def.func_name, 0, 0.0f, NULL,
                   get_type_from_string(node->func_def.return_type), true);

        // 设置函数定义
        set_function_def(node->func_def.func_name, node);

        // 设置参数类型
        if (node->func_def.param_count > 0)
        {
            char **param_types = malloc(node->func_def.param_count * sizeof(char *));
            if (param_types == NULL)
            {
                fprintf(stderr, "Error: Memory allocation failed for parameter types\n");
                exit(1);
            }

            for (int i = 0; i < node->func_def.param_count; i++)
            {
                if (node->func_def.params[i] &&
                    node->func_def.params[i]->type == AST_PARAM_DECLARATION &&
                    node->func_def.params[i]->decl.var_type)
                {
                    param_types[i] = strdup(node->func_def.params[i]->decl.var_type);
                }
                else
                {
                    param_types[i] = strdup("int"); // 默认类型
                }
                if (param_types[i] == NULL)
                {
                    fprintf(stderr, "Error: Memory allocation failed for parameter type\n");
                    exit(1);
                }
            }
            set_function_params(node->func_def.func_name, param_types, node->func_def.param_count);
        }
        else
        {
            set_function_params(node->func_def.func_name, NULL, 0);
        }

        printf("Defined function %s with %d parameters\n", node->func_def.func_name, node->func_def.param_count);

        *is_int = true;
        *int_result = 0;
        *float_result = 0.0f;
        break;
    }
    case AST_FORMATTED_PRINT:
    {
        char output[1024] = {0};
        char *format = node->formatted_print.format_string;
        if (!format)
        {
            fprintf(stderr, "Error: NULL format string\n");
            exit(1);
        }

        int format_len = strlen(format);
        int arg_index = 0;

        // 立即处理所有参数
        int *int_args = NULL;
        float *float_args = NULL;
        bool *int_flags = NULL;
        char **string_args = NULL;

        if (node->formatted_print.arg_count > 0)
        {
            int_args = malloc(node->formatted_print.arg_count * sizeof(int));
            float_args = malloc(node->formatted_print.arg_count * sizeof(float));
            int_flags = malloc(node->formatted_print.arg_count * sizeof(bool));
            string_args = malloc(node->formatted_print.arg_count * sizeof(char *));

            for (int i = 0; i < node->formatted_print.arg_count; i++)
            {
                if (node->formatted_print.args[i]->type == AST_STRING)
                {
                    int_flags[i] = false;
                    int_args[i] = 0;
                    float_args[i] = 0.0f;
                    // 确保字符串值被正确复制
                    if (node->formatted_print.args[i]->string_value)
                    {
                        string_args[i] = strdup(node->formatted_print.args[i]->string_value);
                    }
                    else
                    {
                        string_args[i] = strdup("");
                    }
                }
                else
                {
                    ast_interpret_node(node->formatted_print.args[i],
                                       &int_args[i], &float_args[i], &int_flags[i]);
                    string_args[i] = NULL;
                }
            }
        }

        // 处理格式字符串
        for (int i = 0; i < format_len; i++)
        {
            if (format[i] == '%' && i + 1 < format_len)
            {
                i++; // 跳过%
                if (arg_index < node->formatted_print.arg_count)
                {
                    switch (format[i])
                    {
                    case 'd':
                    case 'i':
                    case 'n':
                    { // 整数格式
                        char num_str[32];
                        if (int_flags[arg_index])
                        {
                            snprintf(num_str, sizeof(num_str), "%d", int_args[arg_index]);
                        }
                        else
                        {
                            snprintf(num_str, sizeof(num_str), "%f", float_args[arg_index]);
                        }
                        strcat(output, num_str);
                        break;
                    }
                    case 'f':
                    { // 浮点数格式
                        char num_str[32];
                        snprintf(num_str, sizeof(num_str), "%f", float_args[arg_index]);
                        strcat(output, num_str);
                        break;
                    }
                    case 's':
                    { // 字符串格式
                        if (string_args[arg_index])
                        {
                            // 确保只处理ASCII字符
                            for (int j = 0; string_args[arg_index][j]; j++)
                            {
                                if ((unsigned char)string_args[arg_index][j] < 128)
                                {
                                    strncat(output, &string_args[arg_index][j], 1);
                                }
                            }
                        }
                        else
                        {
                            char temp_str[32];
                            if (int_flags[arg_index])
                            {
                                snprintf(temp_str, sizeof(temp_str), "%d", int_args[arg_index]);
                            }
                            else
                            {
                                snprintf(temp_str, sizeof(temp_str), "%f", float_args[arg_index]);
                            }
                            strcat(output, temp_str);
                        }
                        break;
                    }
                    case '%': // 转义%
                        strcat(output, "%");
                        break;
                    default: // 未知格式符，原样输出
                        strncat(output, &format[i], 1);
                        break;
                    }
                    arg_index++;
                }
                else
                {
                    // 参数不足，原样输出格式说明符
                    char temp[3] = {'%', format[i], '\0'};
                    strcat(output, temp);
                }
            }
            else
            {
                // 确保只处理ASCII字符
                if ((unsigned char)format[i] < 128)
                {
                    strncat(output, &format[i], 1);
                }
            }
        }
        // 释放临时数组
        if (int_args)
            free(int_args);
        if (float_args)
            free(float_args);
        if (int_flags)
            free(int_flags);
        if (string_args)
        {
            for (int i = 0; i < node->formatted_print.arg_count; i++)
            {
                if (string_args[i])
                    free(string_args[i]);
            }
            free(string_args);
        }

        // 立即输出结果
        printf("%s\n", output);
        append_to_print_buffer(output);
        append_to_print_buffer("\n");

        *is_int = true;
        *int_result = 0;
        *float_result = 0.0f;
        break;
    }
    case AST_ARRAY_DECLARATION:
    {
        int size = 0;
        if (node->array_decl.size)
        {
            int temp_int;
            float temp_float;
            bool temp_is_int;
            ast_interpret_node(node->array_decl.size, &temp_int, &temp_float, &temp_is_int);
            size = temp_int;
            if (size <= 0)
            {
                fprintf(stderr, "Error: Array size must be positive\n");
                exit(1);
            }
        }

        // 假设所有数组都是int类型（在实际实现中应该在语法分析时记录类型）
        int type = TYPE_INT_ARRAY;
        void *array_data = NULL;

        if (size > 0)
        {
            switch (type)
            {
            case TYPE_INT_ARRAY:
                array_data = calloc(size, sizeof(int));
                break;
            case TYPE_FLOAT_ARRAY:
                array_data = calloc(size, sizeof(float));
                break;
            case TYPE_STRING_ARRAY:
                array_data = calloc(size, sizeof(char *));
                break;
            default:
                fprintf(stderr, "Error: Unknown array type\n");
                exit(1);
            }

            if (!array_data)
            {
                fprintf(stderr, "Error: Memory allocation failed for array %s\n", node->array_decl.var_name);
                exit(1);
            }
        }

        set_symbol(node->array_decl.var_name, 0, 0.0f, NULL, type, false);
        symbol_table[symbol_count - 1].array_size = size;
        symbol_table[symbol_count - 1].array_data = array_data;
        symbol_table[symbol_count - 1].is_initialized = true;

        printf("Declared array %s with size %d (type: %d)\n", node->array_decl.var_name, size, type);
        *is_int = true;
        *int_result = 0;
        *float_result = 0.0f;
        break;
    }
    case AST_ARRAY_ACCESS:
    {
        Symbol *sym = find_symbol(node->array_access.var_name);
        if (sym == NULL)
        {
            fprintf(stderr, "Error: Array '%s' not declared\n", node->array_access.var_name);
            exit(1);
        }

        if (!sym->is_initialized)
        {
            fprintf(stderr, "Error: Array '%s' not initialized\n", node->array_access.var_name);
            exit(1);
        }

        if (node->array_access.index == NULL)
        {
            fprintf(stderr, "Error: Array index expression is NULL\n");
            exit(1);
        }

        int index;
        float temp_float;
        bool temp_is_int;
        ast_interpret_node(node->array_access.index, &index, &temp_float, &temp_is_int);

        if (index < 0 || index >= sym->array_size)
        {
            fprintf(stderr, "Error: Array index out of bounds (index: %d, size: %d)\n", index, sym->array_size);
            exit(1);
        }

        if (sym->array_data == NULL)
        {
            fprintf(stderr, "Error: Array '%s' data is NULL\n", node->array_access.var_name);
            exit(1);
        }

        switch (sym->type)
        {
        case TYPE_INT_ARRAY:
            *is_int = true;
            *int_result = ((int *)sym->array_data)[index];
            *float_result = (float)*int_result;
            printf("Array access: %s[%d] = %d\n", node->array_access.var_name, index, *int_result);
            break;
        case TYPE_FLOAT_ARRAY:
            *is_int = false;
            *float_result = ((float *)sym->array_data)[index];
            *int_result = (int)*float_result;
            printf("Array access: %s[%d] = %f\n", node->array_access.var_name, index, *float_result);
            break;
        case TYPE_STRING_ARRAY:
            *is_int = false;
            *int_result = 0;
            *float_result = 0.0f;
            if (((char **)sym->array_data)[index] != NULL)
            {
                printf("Array access: %s[%d] = \"%s\"\n", node->array_access.var_name, index,
                       ((char **)sym->array_data)[index]);
            }
            else
            {
                printf("Array access: %s[%d] = NULL\n", node->array_access.var_name, index);
            }
            break;
        default:
            fprintf(stderr, "Error: '%s' is not an array\n", node->array_access.var_name);
            exit(1);
        }
        break;
    }
    case AST_ARRAY_ASSIGNMENT:
    {
        if (!node->array_assignment.array_access || !node->array_assignment.value)
        {
            fprintf(stderr, "Error: Invalid array assignment\n");
            exit(1);
        }

        char *var_name = node->array_assignment.array_access->array_access.var_name;
        Symbol *sym = find_symbol(var_name);
        if (sym == NULL)
        {
            fprintf(stderr, "Error: Array '%s' not declared\n", var_name);
            exit(1);
        }

        if (!sym->is_initialized)
        {
            fprintf(stderr, "Error: Array '%s' not initialized\n", var_name);
            exit(1);
        }

        if (node->array_assignment.array_access->array_access.index == NULL)
        {
            fprintf(stderr, "Error: Array index expression is NULL\n");
            exit(1);
        }

        int index;
        float temp_float;
        bool temp_is_int;
        ast_interpret_node(node->array_assignment.array_access->array_access.index, &index, &temp_float, &temp_is_int);

        if (index < 0 || index >= sym->array_size)
        {
            fprintf(stderr, "Error: Array index out of bounds (index: %d, size: %d)\n", index, sym->array_size);
            exit(1);
        }

        if (sym->array_data == NULL)
        {
            fprintf(stderr, "Error: Array '%s' data is NULL\n", var_name);
            exit(1);
        }

        int value_int;
        float value_float;
        bool value_is_int;
        ast_interpret_node(node->array_assignment.value, &value_int, &value_float, &value_is_int);

        switch (sym->type)
        {
        case TYPE_INT_ARRAY:
            if (value_is_int)
            {
                ((int *)sym->array_data)[index] = value_int;
            }
            else
            {
                ((int *)sym->array_data)[index] = (int)value_float;
            }
            printf("Assigned %s[%d] = %d\n", var_name, index, ((int *)sym->array_data)[index]);
            break;
        case TYPE_FLOAT_ARRAY:
            if (value_is_int)
            {
                ((float *)sym->array_data)[index] = (float)value_int;
            }
            else
            {
                ((float *)sym->array_data)[index] = value_float;
            }
            printf("Assigned %s[%d] = %f\n", var_name, index, ((float *)sym->array_data)[index]);
            break;
        case TYPE_STRING_ARRAY:
        {
            char **str_array = (char **)sym->array_data;
            if (str_array[index] != NULL)
            {
                free(str_array[index]);
            }

            if (node->array_assignment.value->type == AST_STRING)
            {
                str_array[index] = strdup(node->array_assignment.value->string_value);
            }
            else
            {
                char buffer[256];
                if (value_is_int)
                {
                    snprintf(buffer, sizeof(buffer), "%d", value_int);
                }
                else
                {
                    snprintf(buffer, sizeof(buffer), "%f", value_float);
                }
                str_array[index] = strdup(buffer);
            }

            if (str_array[index] == NULL)
            {
                fprintf(stderr, "Error: Memory allocation failed for string assignment\n");
                exit(1);
            }
            printf("Assigned %s[%d] = \"%s\"\n", var_name, index, str_array[index]);
            break;
        }
        default:
            fprintf(stderr, "Error: '%s' is not an array\n", var_name);
            exit(1);
        }

        *is_int = value_is_int;
        *int_result = value_int;
        *float_result = value_float;
        break;
    }
    case AST_BINARY_OP:
    {
        ast_interpret_node(node->binary.left, &left_int, &left_float, &left_is_int);
        ast_interpret_node(node->binary.right, &right_int, &right_float, &right_is_int);

        *is_int = left_is_int && right_is_int;

        if (strcmp(node->binary.op, "+") == 0)
        {
            if (*is_int)
            {
                *int_result = left_int + right_int;
                *float_result = (float)*int_result;
                printf("Binary operation: %d + %d = %d\n", left_int, right_int, *int_result);
            }
            else
            {
                *float_result = left_float + right_float;
                *int_result = (int)*float_result;
                printf("Binary operation: %f + %f = %f\n", left_float, right_float, *float_result);
            }
        }
        else if (strcmp(node->binary.op, "-") == 0)
        {
            if (*is_int)
            {
                *int_result = left_int - right_int;
                *float_result = (float)*int_result;
                printf("Binary operation: %d - %d = %d\n", left_int, right_int, *int_result);
            }
            else
            {
                *float_result = left_float - right_float;
                *int_result = (int)*float_result;
                printf("Binary operation: %f - %f = %f\n", left_float, right_float, *float_result);
            }
        }
        else if (strcmp(node->binary.op, "*") == 0)
        {
            if (*is_int)
            {
                *int_result = left_int * right_int;
                *float_result = (float)*int_result;
                printf("Binary operation: %d * %d = %d\n", left_int, right_int, *int_result);
            }
            else
            {
                *float_result = left_float * right_float;
                *int_result = (int)*float_result;
                printf("Binary operation: %f * %f = %f\n", left_float, right_float, *float_result);
            }
        }
        else if (strcmp(node->binary.op, "/") == 0)
        {
            if (right_int == 0 && right_float == 0.0f)
            {
                fprintf(stderr, "Error: Division by zero\n");
                exit(1);
            }
            *is_int = false;
            *float_result = left_float / right_float;
            *int_result = (int)*float_result;
            printf("Binary operation: %f / %f = %f\n", left_float, right_float, *float_result);
        }
        else if (strcmp(node->binary.op, "<") == 0)
        {
            *is_int = true;
            if (left_is_int && right_is_int)
            {
                *int_result = left_int < right_int ? 1 : 0;
            }
            else
            {
                *int_result = left_float < right_float ? 1 : 0;
            }
            *float_result = (float)*int_result;
            printf("Binary operation: < comparison result = %d\n", *int_result);
        }
        else if (strcmp(node->binary.op, "<=") == 0)
        {
            *is_int = true;
            if (left_is_int && right_is_int)
            {
                *int_result = left_int <= right_int ? 1 : 0;
            }
            else
            {
                *int_result = left_float <= right_float ? 1 : 0;
            }
            *float_result = (float)*int_result;
            printf("Binary operation: <= comparison result = %d\n", *int_result);
        }
        else if (strcmp(node->binary.op, ">") == 0)
        {
            *is_int = true;
            if (left_is_int && right_is_int)
            {
                *int_result = left_int > right_int ? 1 : 0;
            }
            else
            {
                *int_result = left_float > right_float ? 1 : 0;
            }
            *float_result = (float)*int_result;
            printf("Binary operation: > comparison result = %d\n", *int_result);
        }
        else if (strcmp(node->binary.op, ">=") == 0)
        {
            *is_int = true;
            if (left_is_int && right_is_int)
            {
                *int_result = left_int >= right_int ? 1 : 0;
            }
            else
            {
                *int_result = left_float >= right_float ? 1 : 0;
            }
            *float_result = (float)*int_result;
            printf("Binary operation: >= comparison result = %d\n", *int_result);
        }
        else if (strcmp(node->binary.op, "==") == 0)
        {
            *is_int = true;
            if (left_is_int && right_is_int)
            {
                *int_result = left_int == right_int ? 1 : 0;
            }
            else
            {
                *int_result = left_float == right_float ? 1 : 0;
            }
            *float_result = (float)*int_result;
            printf("Binary operation: == comparison result = %d\n", *int_result);
        }
        else if (strcmp(node->binary.op, "!=") == 0)
        {
            *is_int = true;
            if (left_is_int && right_is_int)
            {
                *int_result = left_int != right_int ? 1 : 0;
            }
            else
            {
                *int_result = left_float != right_float ? 1 : 0;
            }
            *float_result = (float)*int_result;
            printf("Binary operation: != comparison result = %d\n", *int_result);
        }
        else
        {
            fprintf(stderr, "Error: Unknown operator %s\n", node->binary.op);
            exit(1);
        }
        break;
    }
    case AST_IF:
    {
        ast_interpret_node(node->if_stmt.cond, &left_int, &left_float, &left_is_int);
        bool condition = (left_is_int && left_int != 0) || (!left_is_int && left_float != 0.0f);

        if (condition)
        {
            ast_interpret_node(node->if_stmt.then_body, int_result, float_result, is_int);
        }
        else if (node->if_stmt.else_body)
        {
            ast_interpret_node(node->if_stmt.else_body, int_result, float_result, is_int);
        }
        else
        {
            *is_int = true;
            *int_result = 0;
            *float_result = 0.0f;
        }
        break;
    }
    case AST_WHILE:
    {
        *is_int = true;
        *int_result = 0;
        *float_result = 0.0f;

        while (true)
        {
            ast_interpret_node(node->while_loop.cond, &left_int, &left_float, &left_is_int);
            bool condition = (left_is_int && left_int != 0) || (!left_is_int && left_float != 0.0f);

            if (!condition)
                break;

            ast_interpret_node(node->while_loop.body, int_result, float_result, is_int);
        }
        break;
    }
    case AST_FOR:
    {
        ast_interpret_node(node->for_loop.init, int_result, float_result, is_int);

        while (true)
        {
            ast_interpret_node(node->for_loop.cond, &left_int, &left_float, &left_is_int);
            bool condition = (left_is_int && left_int != 0) || (!left_is_int && left_float != 0.0f);

            if (!condition)
                break;

            ast_interpret_node(node->for_loop.body, int_result, float_result, is_int);

            ast_interpret_node(node->for_loop.update, int_result, float_result, is_int);
        }

        *is_int = true;
        *int_result = 0;
        *float_result = 0.0f;
        break;
    }
    case AST_FUNCTION_CALL:
    {
        if (node->func_call.func_name == NULL)
        {
            fprintf(stderr, "Error: NULL function name\n");
            exit(1);
        }

        if (strcmp(node->func_call.func_name, "print") == 0)
        {
            if (node->func_call.arg_count != 1)
            {
                fprintf(stderr, "Error: print function expects exactly 1 argument\n");
                exit(1);
            }

            if (node->func_call.args[0] == NULL)
            {
                fprintf(stderr, "Error: NULL argument in function call\n");
                exit(1);
            }

            char temp_output[256];
            if (node->func_call.args[0]->type == AST_STRING)
            {
                snprintf(temp_output, sizeof(temp_output), "%s\n", node->func_call.args[0]->string_value);
                // 立即输出
                printf("%s\n", node->func_call.args[0]->string_value);
            }
            else
            {
                int temp_int;
                float temp_float;
                bool temp_is_int;
                ast_interpret_node(node->func_call.args[0], &temp_int, &temp_float, &temp_is_int);
                if (temp_is_int)
                {
                    snprintf(temp_output, sizeof(temp_output), "%d\n", temp_int);
                    // 立即输出
                    printf("%d\n", temp_int);
                }
                else
                {
                    snprintf(temp_output, sizeof(temp_output), "%f\n", temp_float);
                    // 立即输出
                    printf("%f\n", temp_float);
                }
            }

            // 保存到缓冲区
            append_to_print_buffer(temp_output);

            *is_int = true;
            *int_result = 0;
            *float_result = 0.0f;
        }
        else
        {

            // 查找函数符号
            Symbol
                *func_sym = find_symbol(node->func_call.func_name);
            if (func_sym == NULL || !func_sym->is_function)
            {
                fprintf(stderr, "Error: Unknown function '%s'\n", node->func_call.func_name);
                exit(1);
            }

            // 检查函数定义是否存在
            if (func_sym->function_def == NULL)
            {
                fprintf(stderr, "Error: Function '%s' has no definition\n", node->func_call.func_name);
                exit(1);
            }

            ASTNode
                *func_def = func_sym->function_def;
            if (func_def->type != AST_FUNCTION_DEF)
            {
                fprintf(stderr, "Error: Function '%s' definition is invalid\n", node->func_call.func_name);
                exit(1);
            }

            // 检查参数数量
            if (node->func_call.arg_count != func_def->func_def.param_count)
            {
                fprintf(stderr, "Error: Function '%s' expects %d arguments, got %d\n",
                        node
                            ->func_call.func_name,
                        func_def
                            ->func_def.param_count,
                        node->func_call.arg_count);
                exit(1);
            }

            printf("Function %s has %d parameters, calling with %d arguments\n",
                   node
                       ->func_call.func_name,
                   func_def
                       ->func_def.param_count,
                   node->func_call.arg_count);

            // 在当前环境中计算参数值
            int *arg_values_int = malloc(node->func_call.arg_count * sizeof(int));
            float *arg_values_float = malloc(node->func_call.arg_count * sizeof(float));
            bool
                *arg_is_int = malloc(node->func_call.arg_count * sizeof(bool));

            if (arg_values_int == NULL || arg_values_float == NULL || arg_is_int == NULL)
            {
                fprintf(stderr, "Error: Memory allocation failed for function arguments\n");
                exit(1);
            }

            for (int i = 0; i < node->func_call.arg_count; i++)
            {
                if (node->func_call.args[i] == NULL)
                {
                    fprintf(stderr, "Error: NULL argument in function call\n");
                    exit(1);
                }
                ast_interpret_node(node->func_call.args[i], &arg_values_int[i],
                                   &arg_values_float[i], &arg_is_int[i]);
            }

            // 保存当前符号表状态
            Symbol
                *old_symbol_table = symbol_table;
            int old_symbol_count = symbol_count;
            int old_symbol_capacity = symbol_capacity;

            // 创建新的符号表用于函数执行
            symbol_table = NULL;
            symbol_count = 0;
            symbol_capacity = 0;

            // 将计算好的参数值设置到新的符号表中
            for (int i = 0; i < node->func_call.arg_count; i++)
            {
                // 确保参数节点是参数声明类型
                if (func_def->func_def.params[i]->type != AST_PARAM_DECLARATION)
                {
                    fprintf(stderr, "Error: Parameter %d in function '%s' is not a parameter declaration\n",
                            i, node->func_call.func_name);
                    exit(1);
                }

                char *param_name = func_def->func_def.params[i]->decl.var_name;
                char *param_type_str = func_sym->param_types[i];

                if (param_name == NULL)
                {
                    fprintf(stderr, "Error: Parameter name is NULL for parameter %d in function '%s'\n",
                            i, node->func_call.func_name);
                    exit(1);
                }

                if (param_type_str == NULL)
                {
                    fprintf(stderr, "Error: Parameter type is NULL for parameter %d in function '%s'\n",
                            i, node->func_call.func_name);
                    exit(1);
                }

                int param_type = get_type_from_string(param_type_str);

                if (arg_is_int[i])
                {
                    set_symbol(param_name, arg_values_int[i], 0.0f, NULL, param_type, false);
                }
                else
                {
                    set_symbol(param_name, 0, arg_values_float[i], NULL, param_type, false);
                }
            }

            // 执行函数体并获取返回值
            int func_result_int;
            float func_result_float;
            bool func_result_is_int;

            ast_interpret_node(func_def->func_def.body, &func_result_int, &func_result_float, &func_result_is_int);

            // 将返回值传递给调用者
            *int_result = func_result_int;
            *float_result = func_result_float;
            *is_int = func_result_is_int;

            // 恢复原来的符号表
            free_current_symbol_table();
            symbol_table = old_symbol_table;
            symbol_count = old_symbol_count;
            symbol_capacity = old_symbol_capacity;

            // 释放临时内存
            free(arg_values_int);
            free(arg_values_float);
            free(arg_is_int);

            printf("Function %s returned: ", node->func_call.func_name);
            if (*is_int)
            {
                printf("%d\n", *int_result);
            }
            else
            {
                printf("%f\n", *float_result);
            }
        }
        break;
    }
    case AST_RETURN:
    {
        ast_interpret_node(node->binary.left, int_result, float_result, is_int);
        printf("Return ");
        if (*is_int)
        {
            printf("%d\n", *int_result);
        }
        else
        {
            printf("%f\n", *float_result);
        }
        break;
    }
    case AST_EMPTY:
    {
        *is_int = true;
        *int_result = 0;
        *float_result = 0.0f;
        break;
    }
    default:
        fprintf(stderr, "Error: Unknown AST node type\n");
        exit(1);
    }
}

// 解释整个AST
void ast_interpret(ASTNode *root)
{
    if (!root)
        return;

    // 重置print缓冲区
    if (print_buffer)
    {
        free(print_buffer);
        print_buffer = NULL;
    }
    print_buffer_size = 0;
    print_buffer_capacity = 0;

    int int_result;
    float float_result;
    bool is_int;

    printf("\n=== Program Execution ===\n");
    ast_interpret_node(root, &int_result, &float_result, &is_int);

    printf("\n=== Final Variable Values ===\n");
    for (int i = 0; i < symbol_count; i++)
    {
        switch (symbol_table[i].type)
        {
        case TYPE_INT:
            printf("%s = %d\n", symbol_table[i].name, symbol_table[i].int_value);
            break;
        case TYPE_FLOAT:
            printf("%s = %f\n", symbol_table[i].name, symbol_table[i].float_value);
            break;
        case TYPE_STRING:
            printf("%s = \"%s\"\n", symbol_table[i].name, symbol_table[i].string_value);
            break;
        case TYPE_INT_ARRAY:
            printf("%s = int[%d]\n", symbol_table[i].name, symbol_table[i].array_size);
            break;
        case TYPE_FLOAT_ARRAY:
            printf("%s = float[%d]\n", symbol_table[i].name, symbol_table[i].array_size);
            break;
        case TYPE_STRING_ARRAY:
            printf("%s = string[%d]\n", symbol_table[i].name, symbol_table[i].array_size);
            break;
        }
    }

    printf("\n=== Print Output ===\n");
    if (print_buffer && print_buffer_size > 0)
    {
        printf("%s", print_buffer);
    }
    else
    {
        printf("(No print output)\n");
    }

    printf("\n=== Program Execution Result ===\n");
    if (is_int)
    {
        printf("Program result: %d\n", int_result);
    }
    else
    {
        printf("Program result: %f\n", float_result);
    }

    // 释放print缓冲区
    if (print_buffer)
    {
        free(print_buffer);
        print_buffer = NULL;
    }
    print_buffer_size = 0;
    print_buffer_capacity = 0;
}