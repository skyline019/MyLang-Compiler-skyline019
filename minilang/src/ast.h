#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// AST节点类型
typedef enum {
    AST_INTEGER,
    AST_FLOAT,
    AST_STRING,
    AST_VARIABLE,
    AST_BINARY_OP,
    AST_ASSIGNMENT,
    AST_DECLARATION,
    AST_DECLARATION_INIT,
    AST_PARAM_DECLARATION,
    AST_IF,
    AST_WHILE,
    AST_FOR,
    AST_BLOCK,
    AST_FUNCTION_CALL,
    AST_RETURN,
    AST_EMPTY,
    AST_FORMATTED_PRINT,
    AST_ARRAY_DECLARATION,
    AST_ARRAY_ACCESS,
    AST_ARRAY_ASSIGNMENT,
    AST_FUNCTION_DEF
} ASTNodeType;

// AST节点结构
typedef struct ASTNode {
    ASTNodeType type;
    int line_no;
    
    union {
        int int_value;
        float float_value;
        char* string_value;
        
        struct {
            char* op;
            struct ASTNode* left;
            struct ASTNode* right;
        } binary;
        
        struct {
            char* var_name;
            struct ASTNode* size;
        } array_decl;
        
        struct {
            char* var_name;
            struct ASTNode* index;
        } array_access;
        
        struct {
            struct ASTNode* array_access;
            struct ASTNode* value;
        } array_assignment;
        
        struct {
            struct ASTNode* cond;
            struct ASTNode* then_body;
            struct ASTNode* else_body;
        } if_stmt;
        
        struct {
            struct ASTNode* cond;
            struct ASTNode* body;
        } while_loop;
        
        struct {
            struct ASTNode* init;
            struct ASTNode* cond;
            struct ASTNode* update;
            struct ASTNode* body;
        } for_loop;
        
        struct {
            struct ASTNode** statements;
            int count;
        } block;
        
        struct {
            char* var_name;
            char* var_type;
            struct ASTNode* init_value;
        } decl;
        
        struct {
            char* func_name;
            struct ASTNode** args;
            int arg_count;
        } func_call;
        
        struct {
            char* func_name;
            char* return_type;
            struct ASTNode** params;
            int param_count;
            struct ASTNode* body;
        } func_def;
        
        struct {
            char* format_string;
            struct ASTNode** args;
            int arg_count;
        } formatted_print;
    };
} ASTNode;

// AST节点创建函数
ASTNode *ast_new_integer(int value, int line_no);
ASTNode *ast_new_float(float value, int line_no);
ASTNode *ast_new_string(char *value, int line_no);
ASTNode *ast_new_variable(char *name, int line_no);
ASTNode *ast_new_binary_op(char *op, ASTNode *left, ASTNode *right, int line_no);
ASTNode *ast_new_assignment(ASTNode *var, ASTNode *expr, int line_no);
ASTNode *ast_new_array_declaration(char *var_name, ASTNode *size, int line_no);
ASTNode *ast_new_array_access(char *var_name, ASTNode *index, int line_no);
ASTNode *ast_new_array_assignment(ASTNode *array_access, ASTNode *value, int line_no);
ASTNode *ast_new_if(ASTNode *cond, ASTNode *then_body, ASTNode *else_body, int line_no);
ASTNode *ast_new_while(ASTNode *cond, ASTNode *body, int line_no);
ASTNode *ast_new_for(ASTNode *init, ASTNode *cond, ASTNode *update, ASTNode *body, int line_no);
ASTNode *ast_new_block(ASTNode **statements, int count, int line_no);
ASTNode *ast_new_declaration(char *var_name, int line_no);
ASTNode *ast_new_declaration_init(char *var_name, ASTNode *init_value, int line_no);
ASTNode *ast_new_function_call(char *func_name, ASTNode **args, int arg_count, int line_no);
ASTNode *ast_new_function_def(char *func_name, char *return_type, ASTNode *param_block, ASTNode *body, int line_no);
ASTNode *ast_new_formatted_print(char *format_string, ASTNode **args, int arg_count, int line_no);
ASTNode *ast_new_return(ASTNode *expr, int line_no);
ASTNode *ast_new_empty(int line_no);
ASTNode *ast_new_param_declaration(char *var_name, char *var_type, int line_no);

// AST打印和释放函数
void ast_print(ASTNode *node, int indent);
void ast_free(ASTNode *node);
void ast_generate_assembly(ASTNode *node, FILE *output);
void ast_write_to_file(ASTNode *node, const char *filename);
#endif // AST_H