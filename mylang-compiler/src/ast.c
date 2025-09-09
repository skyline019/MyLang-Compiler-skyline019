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

// 简单的符号表结构
typedef struct {
    char* name;
    int int_value;
    float float_value;
    char* string_value;
    int type;  // 使用明确的类型定义
    int array_size;
    void* array_data;
    bool is_initialized;
} Symbol;

// 全局符号表
static Symbol* symbol_table = NULL;
static int symbol_count = 0;
static int symbol_capacity = 0;

static char* print_buffer = NULL;
static int print_buffer_size = 0;
static int print_buffer_capacity = 0;

static void append_to_print_buffer(const char* str) {
    if (!str) return;
    
    int len = strlen(str);
    if (print_buffer_size + len + 1 > print_buffer_capacity) {
        int new_capacity = print_buffer_capacity == 0 ? 256 : print_buffer_capacity * 2;
        while (print_buffer_size + len + 1 > new_capacity) {
            new_capacity *= 2;
        }
        print_buffer = realloc(print_buffer, new_capacity);
        if (!print_buffer) {
            perror("Memory allocation failed for print buffer");
            exit(1);
        }
        print_buffer_capacity = new_capacity;
    }
    
    strcpy(print_buffer + print_buffer_size, str);
    print_buffer_size += len;
}

// 查找符号
static Symbol* find_symbol(const char* name) {
    if (name == NULL) {
        fprintf(stderr, "Error: NULL variable name\n");
        return NULL;
    }
    
    if (symbol_table == NULL || symbol_count == 0) {
        fprintf(stderr, "Error: Variable '%s' not declared (symbol table is empty)\n", name);
        return NULL;
    }
    
    for (int i = 0; i < symbol_count; i++) {
        if (symbol_table[i].name != NULL && strcmp(symbol_table[i].name, name) == 0) {
            return &symbol_table[i];
        }
    }
    
    fprintf(stderr, "Error: Variable '%s' not declared\n", name);
    return NULL;
}

// 添加或更新符号
static void set_symbol(const char* name, int int_value, float float_value, char* string_value, int type) {
    if (name == NULL) {
        fprintf(stderr, "Error: NULL variable name\n");
        exit(1);
    }
    
    for (int i = 0; i < symbol_count; i++) {
        if (symbol_table[i].name != NULL && strcmp(symbol_table[i].name, name) == 0) {
            symbol_table[i].int_value = int_value;
            symbol_table[i].float_value = float_value;
            if (symbol_table[i].string_value != NULL) {
                free(symbol_table[i].string_value);
            }
            symbol_table[i].string_value = string_value ? strdup(string_value) : NULL;
            symbol_table[i].type = type;
            symbol_table[i].is_initialized = true;
            return;
        }
    }
    
    if (symbol_count >= symbol_capacity) {
        symbol_capacity = symbol_capacity == 0 ? 8 : symbol_capacity * 2;
        symbol_table = realloc(symbol_table, symbol_capacity * sizeof(Symbol));
        if (!symbol_table) {
            perror("Memory allocation failed");
            exit(1);
        }
    }
    
    symbol_table[symbol_count].name = strdup(name);
    symbol_table[symbol_count].int_value = int_value;
    symbol_table[symbol_count].float_value = float_value;
    symbol_table[symbol_count].string_value = string_value ? strdup(string_value) : NULL;
    symbol_table[symbol_count].type = type;
    symbol_table[symbol_count].array_size = 0;
    symbol_table[symbol_count].array_data = NULL;
    symbol_table[symbol_count].is_initialized = false;
    symbol_count++;
}

// 释放符号表
void ast_free_symbol_table() {
    // 释放print缓冲区
    if (print_buffer) {
        free(print_buffer);
        print_buffer = NULL;
    }
    print_buffer_size = 0;
    print_buffer_capacity = 0;
    
    for (int i = 0; i < symbol_count; i++) {
        free(symbol_table[i].name);
        if (symbol_table[i].string_value != NULL) {
            free(symbol_table[i].string_value);
        }
        if (symbol_table[i].array_data != NULL) {
            if (symbol_table[i].type == TYPE_STRING_ARRAY) {
                char** str_array = (char**)symbol_table[i].array_data;
                for (int j = 0; j < symbol_table[i].array_size; j++) {
                    if (str_array[j] != NULL) {
                        free(str_array[j]);
                    }
                }
            }
            free(symbol_table[i].array_data);
        }
    }
    free(symbol_table);
    symbol_table = NULL;
    symbol_count = 0;
    symbol_capacity = 0;
}

ASTNode *ast_new_integer(int value, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_INTEGER;
    node->line_no = line_no;
    node->int_value = value;
    return node;
}

ASTNode *ast_new_float(float value, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_FLOAT;
    node->line_no = line_no;
    node->float_value = value;
    return node;
}

ASTNode *ast_new_string(char *value, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_STRING;
    node->line_no = line_no;
    node->string_value = strdup(value);
    return node;
}

ASTNode *ast_new_variable(char *name, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_VARIABLE;
    node->line_no = line_no;
    node->string_value = strdup(name);
    return node;
}

ASTNode *ast_new_array_declaration(char *var_name, ASTNode *size, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ARRAY_DECLARATION;
    node->line_no = line_no;
    node->array_decl.var_name = strdup(var_name);
    node->array_decl.size = size;
    return node;
}

ASTNode *ast_new_array_access(char *var_name, ASTNode *index, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ARRAY_ACCESS;
    node->line_no = line_no;
    node->array_access.var_name = strdup(var_name);
    node->array_access.index = index;
    return node;
}

ASTNode *ast_new_array_assignment(ASTNode *array_access, ASTNode *value, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ARRAY_ASSIGNMENT;
    node->line_no = line_no;
    node->array_assignment.array_access = array_access;
    node->array_assignment.value = value;
    return node;
}

ASTNode *ast_new_binary_op(char *op, ASTNode *left, ASTNode *right, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY_OP;
    node->line_no = line_no;
    node->binary.op = strdup(op);
    node->binary.left = left;
    node->binary.right = right;
    return node;
}

ASTNode *ast_new_assignment(ASTNode *var, ASTNode *expr, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_ASSIGNMENT;
    node->line_no = line_no;
    node->binary.left = var;
    node->binary.right = expr;
    return node;
}

ASTNode *ast_new_if(ASTNode *cond, ASTNode *then_body, ASTNode *else_body, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_IF;
    node->line_no = line_no;
    node->if_stmt.cond = cond;
    node->if_stmt.then_body = then_body;
    node->if_stmt.else_body = else_body;
    return node;
}

ASTNode *ast_new_while(ASTNode *cond, ASTNode *body, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_WHILE;
    node->line_no = line_no;
    node->while_loop.cond = cond;
    node->while_loop.body = body;
    return node;
}

ASTNode *ast_new_for(ASTNode *init, ASTNode *cond, ASTNode *update, ASTNode *body, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_FOR;
    node->line_no = line_no;
    node->for_loop.init = init;
    node->for_loop.cond = cond;
    node->for_loop.update = update;
    node->for_loop.body = body;
    return node;
}

ASTNode *ast_new_block(ASTNode **statements, int count, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BLOCK;
    node->line_no = line_no;
    node->block.statements = statements;
    node->block.count = count;
    return node;
}

ASTNode *ast_new_declaration(char *var_name, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_DECLARATION;
    node->line_no = line_no;
    node->decl.var_name = strdup(var_name);
    node->decl.init_value = NULL;
    return node;
}

ASTNode *ast_new_declaration_init(char *var_name, ASTNode *init_value, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_DECLARATION_INIT;
    node->line_no = line_no;
    node->decl.var_name = strdup(var_name);
    node->decl.init_value = init_value;
    return node;
}

ASTNode *ast_new_function_call(char *func_name, ASTNode **args, int arg_count, int line_no) {
    if (func_name == NULL) {
        fprintf(stderr, "Error: NULL function name\n");
        exit(1);
    }
    
    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    
    memset(node, 0, sizeof(ASTNode));
    
    node->type = AST_FUNCTION_CALL;
    node->line_no = line_no;
    node->func_call.func_name = strdup(func_name);
    if (node->func_call.func_name == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        free(node);
        exit(1);
    }
    
    if (arg_count > 0) {
        node->func_call.args = malloc(sizeof(ASTNode*) * arg_count);
        if (node->func_call.args == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            free(node->func_call.func_name);
            free(node);
            exit(1);
        }
        
        for (int i = 0; i < arg_count; i++) {
            node->func_call.args[i] = args[i];
        }
    } else {
        node->func_call.args = NULL;
    }
    
    node->func_call.arg_count = arg_count;
    
    return node;
}

ASTNode *ast_new_return(ASTNode *expr, int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_RETURN;
    node->line_no = line_no;
    node->binary.left = expr;
    return node;
}

ASTNode *ast_new_empty(int line_no) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_EMPTY;
    node->line_no = line_no;
    return node;
}

void ast_print_indent(int indent) {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}

void ast_print(ASTNode *node, int indent) {
    if (!node) return;
    
    ast_print_indent(indent);
    printf("Line %d: ", node->line_no);
    
    switch (node->type) {
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
        case AST_ARRAY_DECLARATION:
            printf("ARRAY_DECLARATION(%s", node->array_decl.var_name);
            if (node->array_decl.size) {
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
            if (node->if_stmt.else_body) {
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
            for (int i = 0; i < node->block.count; i++) {
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
            if (node->func_call.args != NULL) {
                for (int i = 0; i < node->func_call.arg_count; i++) {
                    if (node->func_call.args[i] != NULL) {
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

void ast_free(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case AST_STRING:
            free(node->string_value);
            break;
        case AST_VARIABLE:
            free(node->string_value);
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
            for (int i = 0; i < node->block.count; i++) {
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
        case AST_FUNCTION_CALL: {
            if (node->func_call.func_name != NULL) {
                free(node->func_call.func_name);
            }
            if (node->func_call.args != NULL) {
                for (int i = 0; i < node->func_call.arg_count; i++) {
                    if (node->func_call.args[i] != NULL) {
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
void ast_interpret_node(ASTNode* node, int* int_result, float* float_result, bool* is_int) {
    if (!node) {
        *is_int = true;
        *int_result = 0;
        *float_result = 0.0f;
        return;
    }
    
    printf("Interpreting node type: %d at line %d\n", node->type, node->line_no);
    
    int left_int, right_int;
    float left_float, right_float;
    bool left_is_int, right_is_int;
    
    switch (node->type) {
        case AST_BLOCK: {
            for (int i = 0; i < node->block.count; i++) {
                ast_interpret_node(node->block.statements[i], int_result, float_result, is_int);
            }
            break;
        }
        case AST_DECLARATION: {
            set_symbol(node->decl.var_name, 0, 0.0f, NULL, TYPE_INT);
            printf("Declared variable %s\n", node->decl.var_name);
            *is_int = true;
            *int_result = 0;
            *float_result = 0.0f;
            break;
        }
        case AST_DECLARATION_INIT: {
            int temp_int;
            float temp_float;
            bool temp_is_int;
            
            ast_interpret_node(node->decl.init_value, &temp_int, &temp_float, &temp_is_int);
            
            int type;
            char* string_value = NULL;
            if (node->decl.init_value->type == AST_STRING) {
                type = TYPE_STRING;
                string_value = node->decl.init_value->string_value;
            } else if (temp_is_int) {
                type = TYPE_INT;
            } else {
                type = TYPE_FLOAT;
            }
            
            set_symbol(node->decl.var_name, temp_int, temp_float, string_value, type);
            
            if (type == TYPE_INT) {
                printf("Variable %s = %d\n", node->decl.var_name, temp_int);
            } else if (type == TYPE_FLOAT) {
                printf("Variable %s = %f\n", node->decl.var_name, temp_float);
            } else {
                printf("Variable %s = \"%s\"\n", node->decl.var_name, string_value);
            }
            
            *int_result = temp_int;
            *float_result = temp_float;
            *is_int = temp_is_int;
            break;
        }
        case AST_ASSIGNMENT: {
            int temp_int;
            float temp_float;
            bool temp_is_int;
            
            ast_interpret_node(node->binary.right, &temp_int, &temp_float, &temp_is_int);
            char* var_name = node->binary.left->string_value;
            
            int type;
            char* string_value = NULL;
            if (node->binary.right->type == AST_STRING) {
                type = TYPE_STRING;
                string_value = node->binary.right->string_value;
            } else if (temp_is_int) {
                type = TYPE_INT;
            } else {
                type = TYPE_FLOAT;
            }
            
            set_symbol(var_name, temp_int, temp_float, string_value, type);
            
            if (type == TYPE_INT) {
                printf("Assigned %s = %d\n", var_name, temp_int);
            } else if (type == TYPE_FLOAT) {
                printf("Assigned %s = %f\n", var_name, temp_float);
            } else {
                printf("Assigned %s = \"%s\"\n", var_name, string_value);
            }
            
            *int_result = temp_int;
            *float_result = temp_float;
            *is_int = temp_is_int;
            break;
        }
        case AST_INTEGER: {
            *is_int = true;
            *int_result = node->int_value;
            *float_result = (float)node->int_value;
            break;
        }
        case AST_FLOAT: {
            *is_int = false;
            *int_result = (int)node->float_value;
            *float_result = node->float_value;
            break;
        }
        case AST_STRING: {
            *is_int = false;
            *int_result = 0;
            *float_result = 0.0f;
            break;
        }
        case AST_VARIABLE: {
            Symbol* sym = find_symbol(node->string_value);
            if (sym == NULL) {
                fprintf(stderr, "Error: Variable '%s' not found\n", node->string_value);
                exit(1);
            }
            *is_int = (sym->type == TYPE_INT);
            *int_result = sym->int_value;
            *float_result = sym->float_value;
            break;
        }
        case AST_ARRAY_DECLARATION: {
            int size = 0;
            if (node->array_decl.size) {
                int temp_int;
                float temp_float;
                bool temp_is_int;
                ast_interpret_node(node->array_decl.size, &temp_int, &temp_float, &temp_is_int);
                size = temp_int;
                if (size <= 0) {
                    fprintf(stderr, "Error: Array size must be positive\n");
                    exit(1);
                }
            }
            
            // 假设所有数组都是int类型（在实际实现中应该在语法分析时记录类型）
            int type = TYPE_INT_ARRAY;
            void* array_data = NULL;
            
            if (size > 0) {
                switch (type) {
                    case TYPE_INT_ARRAY:
                        array_data = calloc(size, sizeof(int));
                        break;
                    case TYPE_FLOAT_ARRAY:
                        array_data = calloc(size, sizeof(float));
                        break;
                    case TYPE_STRING_ARRAY:
                        array_data = calloc(size, sizeof(char*));
                        break;
                    default:
                        fprintf(stderr, "Error: Unknown array type\n");
                        exit(1);
                }
                
                if (!array_data) {
                    fprintf(stderr, "Error: Memory allocation failed for array %s\n", node->array_decl.var_name);
                    exit(1);
                }
            }
            
            set_symbol(node->array_decl.var_name, 0, 0.0f, NULL, type);
            symbol_table[symbol_count-1].array_size = size;
            symbol_table[symbol_count-1].array_data = array_data;
            symbol_table[symbol_count-1].is_initialized = true;
            
            printf("Declared array %s with size %d (type: %d)\n", node->array_decl.var_name, size, type);
            *is_int = true;
            *int_result = 0;
            *float_result = 0.0f;
            break;
        }
        case AST_ARRAY_ACCESS: {
            Symbol* sym = find_symbol(node->array_access.var_name);
            if (sym == NULL) {
                fprintf(stderr, "Error: Array '%s' not declared\n", node->array_access.var_name);
                exit(1);
            }
            
            if (!sym->is_initialized) {
                fprintf(stderr, "Error: Array '%s' not initialized\n", node->array_access.var_name);
                exit(1);
            }
            
            if (node->array_access.index == NULL) {
                fprintf(stderr, "Error: Array index expression is NULL\n");
                exit(1);
            }
            
            int index;
            float temp_float;
            bool temp_is_int;
            ast_interpret_node(node->array_access.index, &index, &temp_float, &temp_is_int);
            
            if (index < 0 || index >= sym->array_size) {
                fprintf(stderr, "Error: Array index out of bounds (index: %d, size: %d)\n", index, sym->array_size);
                exit(1);
            }
            
            if (sym->array_data == NULL) {
                fprintf(stderr, "Error: Array '%s' data is NULL\n", node->array_access.var_name);
                exit(1);
            }
            
            switch (sym->type) {
                case TYPE_INT_ARRAY:
                    *is_int = true;
                    *int_result = ((int*)sym->array_data)[index];
                    *float_result = (float)*int_result;
                    printf("Array access: %s[%d] = %d\n", node->array_access.var_name, index, *int_result);
                    break;
                case TYPE_FLOAT_ARRAY:
                    *is_int = false;
                    *float_result = ((float*)sym->array_data)[index];
                    *int_result = (int)*float_result;
                    printf("Array access: %s[%d] = %f\n", node->array_access.var_name, index, *float_result);
                    break;
                case TYPE_STRING_ARRAY:
                    *is_int = false;
                    *int_result = 0;
                    *float_result = 0.0f;
                    if (((char**)sym->array_data)[index] != NULL) {
                        printf("Array access: %s[%d] = \"%s\"\n", node->array_access.var_name, index, 
                               ((char**)sym->array_data)[index]);
                    } else {
                        printf("Array access: %s[%d] = NULL\n", node->array_access.var_name, index);
                    }
                    break;
                default:
                    fprintf(stderr, "Error: '%s' is not an array\n", node->array_access.var_name);
                    exit(1);
            }
            break;
        }
        case AST_ARRAY_ASSIGNMENT: {
            if (!node->array_assignment.array_access || !node->array_assignment.value) {
                fprintf(stderr, "Error: Invalid array assignment\n");
                exit(1);
            }
            
            char* var_name = node->array_assignment.array_access->array_access.var_name;
            Symbol* sym = find_symbol(var_name);
            if (sym == NULL) {
                fprintf(stderr, "Error: Array '%s' not declared\n", var_name);
                exit(1);
            }
            
            if (!sym->is_initialized) {
                fprintf(stderr, "Error: Array '%s' not initialized\n", var_name);
                exit(1);
            }
            
            if (node->array_assignment.array_access->array_access.index == NULL) {
                fprintf(stderr, "Error: Array index expression is NULL\n");
                exit(1);
            }
            
            int index;
            float temp_float;
            bool temp_is_int;
            ast_interpret_node(node->array_assignment.array_access->array_access.index, &index, &temp_float, &temp_is_int);
            
            if (index < 0 || index >= sym->array_size) {
                fprintf(stderr, "Error: Array index out of bounds (index: %d, size: %d)\n", index, sym->array_size);
                exit(1);
            }
            
            if (sym->array_data == NULL) {
                fprintf(stderr, "Error: Array '%s' data is NULL\n", var_name);
                exit(1);
            }
            
            int value_int;
            float value_float;
            bool value_is_int;
            ast_interpret_node(node->array_assignment.value, &value_int, &value_float, &value_is_int);
            
            switch (sym->type) {
                case TYPE_INT_ARRAY:
                    if (value_is_int) {
                        ((int*)sym->array_data)[index] = value_int;
                    } else {
                        ((int*)sym->array_data)[index] = (int)value_float;
                    }
                    printf("Assigned %s[%d] = %d\n", var_name, index, ((int*)sym->array_data)[index]);
                    break;
                case TYPE_FLOAT_ARRAY:
                    if (value_is_int) {
                        ((float*)sym->array_data)[index] = (float)value_int;
                    } else {
                        ((float*)sym->array_data)[index] = value_float;
                    }
                    printf("Assigned %s[%d] = %f\n", var_name, index, ((float*)sym->array_data)[index]);
                    break;
                case TYPE_STRING_ARRAY: {
                    char** str_array = (char**)sym->array_data;
                    if (str_array[index] != NULL) {
                        free(str_array[index]);
                    }
                    
                    if (node->array_assignment.value->type == AST_STRING) {
                        str_array[index] = strdup(node->array_assignment.value->string_value);
                    } else {
                        char buffer[256];
                        if (value_is_int) {
                            snprintf(buffer, sizeof(buffer), "%d", value_int);
                        } else {
                            snprintf(buffer, sizeof(buffer), "%f", value_float);
                        }
                        str_array[index] = strdup(buffer);
                    }
                    
                    if (str_array[index] == NULL) {
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
        case AST_BINARY_OP: {
            ast_interpret_node(node->binary.left, &left_int, &left_float, &left_is_int);
            ast_interpret_node(node->binary.right, &right_int, &right_float, &right_is_int);
            
            *is_int = left_is_int && right_is_int;
            
            if (strcmp(node->binary.op, "+") == 0) {
                if (*is_int) {
                    *int_result = left_int + right_int;
                    *float_result = (float)*int_result;
                    printf("Binary operation: %d + %d = %d\n", left_int, right_int, *int_result);
                } else {
                    *float_result = left_float + right_float;
                    *int_result = (int)*float_result;
                    printf("Binary operation: %f + %f = %f\n", left_float, right_float, *float_result);
                }
            } else if (strcmp(node->binary.op, "-") == 0) {
                if (*is_int) {
                    *int_result = left_int - right_int;
                    *float_result = (float)*int_result;
                    printf("Binary operation: %d - %d = %d\n", left_int, right_int, *int_result);
                } else {
                    *float_result = left_float - right_float;
                    *int_result = (int)*float_result;
                    printf("Binary operation: %f - %f = %f\n", left_float, right_float, *float_result);
                }
            } else if (strcmp(node->binary.op, "*") == 0) {
                if (*is_int) {
                    *int_result = left_int * right_int;
                    *float_result = (float)*int_result;
                    printf("Binary operation: %d * %d = %d\n", left_int, right_int, *int_result);
                } else {
                    *float_result = left_float * right_float;
                    *int_result = (int)*float_result;
                    printf("Binary operation: %f * %f = %f\n", left_float, right_float, *float_result);
                }
            } else if (strcmp(node->binary.op, "/") == 0) {
                if (right_int == 0 && right_float == 0.0f) {
                    fprintf(stderr, "Error: Division by zero\n");
                    exit(1);
                }
                *is_int = false;
                *float_result = left_float / right_float;
                *int_result = (int)*float_result;
                printf("Binary operation: %f / %f = %f\n", left_float, right_float, *float_result);
            } else if (strcmp(node->binary.op, "<") == 0) {
                *is_int = true;
                if (left_is_int && right_is_int) {
                    *int_result = left_int < right_int ? 1 : 0;
                } else {
                    *int_result = left_float < right_float ? 1 : 0;
                }
                *float_result = (float)*int_result;
                printf("Binary operation: < comparison result = %d\n", *int_result);
            } else if (strcmp(node->binary.op, "<=") == 0) {
                *is_int = true;
                if (left_is_int && right_is_int) {
                    *int_result = left_int <= right_int ? 1 : 0;
                } else {
                    *int_result = left_float <= right_float ? 1 : 0;
                }
                *float_result = (float)*int_result;
                printf("Binary operation: <= comparison result = %d\n", *int_result);
            } else if (strcmp(node->binary.op, ">") == 0) {
                *is_int = true;
                if (left_is_int && right_is_int) {
                    *int_result = left_int > right_int ? 1 : 0;
                } else {
                    *int_result = left_float > right_float ? 1 : 0;
                }
                *float_result = (float)*int_result;
                printf("Binary operation: > comparison result = %d\n", *int_result);
            } else if (strcmp(node->binary.op, ">=") == 0) {
                *is_int = true;
                if (left_is_int && right_is_int) {
                    *int_result = left_int >= right_int ? 1 : 0;
                } else {
                    *int_result = left_float >= right_float ? 1 : 0;
                }
                *float_result = (float)*int_result;
                printf("Binary operation: >= comparison result = %d\n", *int_result);
            } else if (strcmp(node->binary.op, "==") == 0) {
                *is_int = true;
                if (left_is_int && right_is_int) {
                    *int_result = left_int == right_int ? 1 : 0;
                } else {
                    *int_result = left_float == right_float ? 1 : 0;
                }
                *float_result = (float)*int_result;
                printf("Binary operation: == comparison result = %d\n", *int_result);
            } else if (strcmp(node->binary.op, "!=") == 0) {
                *is_int = true;
                if (left_is_int && right_is_int) {
                    *int_result = left_int != right_int ? 1 : 0;
                } else {
                    *int_result = left_float != right_float ? 1 : 0;
                }
                *float_result = (float)*int_result;
                printf("Binary operation: != comparison result = %d\n", *int_result);
            } else {
                fprintf(stderr, "Error: Unknown operator %s\n", node->binary.op);
                exit(1);
            }
            break;
        }
        case AST_IF: {
            ast_interpret_node(node->if_stmt.cond, &left_int, &left_float, &left_is_int);
            bool condition = (left_is_int && left_int != 0) || (!left_is_int && left_float != 0.0f);
            
            if (condition) {
                ast_interpret_node(node->if_stmt.then_body, int_result, float_result, is_int);
            } else if (node->if_stmt.else_body) {
                ast_interpret_node(node->if_stmt.else_body, int_result, float_result, is_int);
            } else {
                *is_int = true;
                *int_result = 0;
                *float_result = 0.0f;
            }
            break;
        }
        case AST_WHILE: {
            *is_int = true;
            *int_result = 0;
            *float_result = 0.0f;
            
            while (true) {
                ast_interpret_node(node->while_loop.cond, &left_int, &left_float, &left_is_int);
                bool condition = (left_is_int && left_int != 0) || (!left_is_int && left_float != 0.0f);
                
                if (!condition) break;
                
                ast_interpret_node(node->while_loop.body, int_result, float_result, is_int);
            }
            break;
        }
        case AST_FOR: {
            ast_interpret_node(node->for_loop.init, int_result, float_result, is_int);
            
            while (true) {
                ast_interpret_node(node->for_loop.cond, &left_int, &left_float, &left_is_int);
                bool condition = (left_is_int && left_int != 0) || (!left_is_int && left_float != 0.0f);
                
                if (!condition) break;
                
                ast_interpret_node(node->for_loop.body, int_result, float_result, is_int);
                
                ast_interpret_node(node->for_loop.update, int_result, float_result, is_int);
            }
            
            *is_int = true;
            *int_result = 0;
            *float_result = 0.0f;
            break;
        }
        case AST_FUNCTION_CALL: {
            if (strcmp(node->func_call.func_name, "print") == 0) {
                if (node->func_call.arg_count != 1) {
                    fprintf(stderr, "Error: print function expects exactly 1 argument\n");
                    exit(1);
                }
                
                if (node->func_call.args[0] == NULL) {
                    fprintf(stderr, "Error: NULL argument in function call\n");
                    exit(1);
                }
                
                char temp_output[256];
                if (node->func_call.args[0]->type == AST_STRING) {
                    snprintf(temp_output, sizeof(temp_output), "%s\n", node->func_call.args[0]->string_value);
                    // 立即输出
                    printf("%s\n", node->func_call.args[0]->string_value);
                } else {
                    int temp_int;
                    float temp_float;
                    bool temp_is_int;
                    ast_interpret_node(node->func_call.args[0], &temp_int, &temp_float, &temp_is_int);
                    if (temp_is_int) {
                        snprintf(temp_output, sizeof(temp_output), "%d\n", temp_int);
                        // 立即输出
                        printf("%d\n", temp_int);
                    } else {
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
            } else {
                fprintf(stderr, "Error: Unknown function '%s'\n", node->func_call.func_name);
                exit(1);
            }
            break;
        }
        case AST_RETURN: {
            ast_interpret_node(node->binary.left, int_result, float_result, is_int);
            printf("Return ");
            if (*is_int) {
                printf("%d\n", *int_result);
            } else {
                printf("%f\n", *float_result);
            }
            break;
        }
        case AST_EMPTY: {
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
void ast_interpret(ASTNode* root) {
    if (!root) return;
    
    // 重置print缓冲区
    if (print_buffer) {
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
    for (int i = 0; i < symbol_count; i++) {
        switch (symbol_table[i].type) {
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
    if (print_buffer && print_buffer_size > 0) {
        printf("%s", print_buffer);
    } else {
        printf("(No print output)\n");
    }
    
    printf("\n=== Program Execution Result ===\n");
    if (is_int) {
        printf("Program result: %d\n", int_result);
    } else {
        printf("Program result: %f\n", float_result);
    }
    
    // 释放print缓冲区
    if (print_buffer) {
        free(print_buffer);
        print_buffer = NULL;
    }
    print_buffer_size = 0;
    print_buffer_capacity = 0;
}
