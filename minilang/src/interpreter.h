#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "symbol.h"

// 打印缓冲区管理
void append_to_print_buffer(const char *str);
void free_print_buffer();

// 解释器函数
void ast_interpret_node(ASTNode *node, int *int_result, float *float_result, bool *is_int);
void ast_interpret(ASTNode *root);
void interpret_assembly_instruction(const char *instruction);
void interpret_assembly_file(const char *filename);
#endif // INTERPRETER_H