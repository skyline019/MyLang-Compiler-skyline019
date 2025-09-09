#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode *ast_new_integer(int value, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_INTEGER;
    node->line_no = line_no;
    node->int_value = value;
    return node;
}

ASTNode *ast_new_float(float value, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_FLOAT;
    node->line_no = line_no;
    node->float_value = value;
    return node;
}

ASTNode *ast_new_string(char *value, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_STRING;
    node->line_no = line_no;
    node->string_value = strdup(value);
    return node;
}

ASTNode *ast_new_variable(char *name, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_VARIABLE;
    node->line_no = line_no;
    node->string_value = strdup(name);
    return node;
}

ASTNode *ast_new_function_def(char *func_name, char *return_type, ASTNode *param_block,
                              ASTNode *body, int line_no)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for function definition\n");
        exit(1);
    }

    node->type = AST_FUNCTION_DEF;
    node->line_no = line_no;
    node->func_def.func_name = strdup(func_name);
    node->func_def.return_type = strdup(return_type);

    // 从block节点中提取参数
    if (param_block && param_block->type == AST_BLOCK)
    {
        node->func_def.params = param_block->block.statements;
        node->func_def.param_count = param_block->block.count;

        // 释放block节点但不释放statements数组
        free(param_block);
    }
    else
    {
        node->func_def.params = NULL;
        node->func_def.param_count = 0;
    }

    node->func_def.body = body;

    printf("Creating function %s with %d parameters\n", func_name, node->func_def.param_count);
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
    node->type = AST_PARAM_DECLARATION;
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
void ast_generate_assembly(ASTNode *node, FILE *output) {
    if (!node) return;

    switch (node->type) {
        case AST_INTEGER:
            fprintf(output, "    movl $%d, %%eax\n", node->int_value);
            break;
            
        case AST_FLOAT:
            fprintf(output, "    movss $.LC%d, %%xmm0\n", (int)(node->float_value * 1000));
            break;
            
        case AST_STRING:
            fprintf(output, "    leaq .LC%d(%%rip), %%rax\n", node->string_value);
            break;
            
        case AST_VARIABLE:
            fprintf(output, "    movl %s(%%rip), %%eax\n", node->string_value);
            break;
            
        case AST_FUNCTION_DEF:
            fprintf(output, "%s:\n", node->func_def.func_name);
            fprintf(output, "    pushq %%rbp\n");
            fprintf(output, "    movq %%rsp, %%rbp\n");
            
            // 为局部变量分配空间
            fprintf(output, "    subq $32, %%rsp\n");
            
            // 保存寄存器
            fprintf(output, "    movq %%rbx, -8(%%rbp)\n");
            fprintf(output, "    movq %%r12, -16(%%rbp)\n");
            fprintf(output, "    movq %%r13, -24(%%rbp)\n");
            fprintf(output, "    movq %%r14, -32(%%rbp)\n");
            
            // 处理参数
            for (int i = 0; i < node->func_def.param_count; i++) {
                if (i < 6) {
                    fprintf(output, "    movq %%%s, %d(%%rbp)\n", 
                            i == 0 ? "rdi" : i == 1 ? "rsi" : i == 2 ? "rdx" : 
                            i == 3 ? "rcx" : i == 4 ? "r8" : "r9", 
                            (i + 1) * -8);
                }
            }
            
            // 生成函数体
            ast_generate_assembly(node->func_def.body, output);
            
            // 恢复寄存器
            fprintf(output, "    movq -8(%%rbp), %%rbx\n");
            fprintf(output, "    movq -16(%%rbp), %%r12\n");
            fprintf(output, "    movq -24(%%rbp), %%r13\n");
            fprintf(output, "    movq -32(%%rbp), %%r14\n");
            
            // 返回
            fprintf(output, "    leave\n");
            fprintf(output, "    ret\n");
            break;
            
        case AST_FORMATTED_PRINT:
            fprintf(output, "    leaq .LC%d(%%rip), %%rdi\n", node->formatted_print.format_string);
            for (int i = 0; i < node->formatted_print.arg_count; i++) {
                ast_generate_assembly(node->formatted_print.args[i], output);
                if (i == 0) fprintf(output, "    movl %%eax, %%esi\n");
                else if (i == 1) fprintf(output, "    movl %%eax, %%edx\n");
                else if (i == 2) fprintf(output, "    movl %%eax, %%ecx\n");
                else if (i == 3) fprintf(output, "    movl %%eax, %%r8d\n");
                else if (i == 4) fprintf(output, "    movl %%eax, %%r9d\n");
            }
            fprintf(output, "    movl $0, %%eax\n");
            fprintf(output, "    call printf\n");
            break;
            
        case AST_PARAM_DECLARATION:
            // 参数声明在函数定义中处理
            break;
            
        case AST_ARRAY_DECLARATION:
            fprintf(output, "    .comm %s, %d\n", node->array_decl.var_name, 
                    node->array_decl.size ? node->array_decl.size->int_value * 4 : 0);
            break;
            
        case AST_ARRAY_ACCESS:
            fprintf(output, "    movl %s(%%rip), %%eax\n", node->array_access.var_name);
            ast_generate_assembly(node->array_access.index, output);
            fprintf(output, "    cltq\n");
            fprintf(output, "    movl %d(%%rax), %%eax\n", node->array_access.var_name);
            break;
            
        case AST_ARRAY_ASSIGNMENT:
            ast_generate_assembly(node->array_assignment.array_access, output);
            fprintf(output, "    pushq %%rax\n");
            ast_generate_assembly(node->array_assignment.value, output);
            fprintf(output, "    popq %%rcx\n");
            fprintf(output, "    movl %%eax, (%%rcx)\n");
            break;
            
        case AST_BINARY_OP:
            ast_generate_assembly(node->binary.left, output);
            fprintf(output, "    pushq %%rax\n");
            ast_generate_assembly(node->binary.right, output);
            fprintf(output, "    popq %%rcx\n");
            
            if (strcmp(node->binary.op, "+") == 0) {
                fprintf(output, "    addl %%ecx, %%eax\n");
            } else if (strcmp(node->binary.op, "-") == 0) {
                fprintf(output, "    subl %%ecx, %%eax\n");
            } else if (strcmp(node->binary.op, "*") == 0) {
                fprintf(output, "    imull %%ecx, %%eax\n");
            } else if (strcmp(node->binary.op, "/") == 0) {
                fprintf(output, "    cdq\n");
                fprintf(output, "    idivl %%ecx\n");
            } else if (strcmp(node->binary.op, "==") == 0) {
                fprintf(output, "    cmpl %%ecx, %%eax\n");
                fprintf(output, "    sete %%al\n");
                fprintf(output, "    movzbl %%al, %%eax\n");
            } else if (strcmp(node->binary.op, "!=") == 0) {
                fprintf(output, "    cmpl %%ecx, %%eax\n");
                fprintf(output, "    setne %%al\n");
                fprintf(output, "    movzbl %%al, %%eax\n");
            } else if (strcmp(node->binary.op, "<") == 0) {
                fprintf(output, "    cmpl %%ecx, %%eax\n");
                fprintf(output, "    setl %%al\n");
                fprintf(output, "    movzbl %%al, %%eax\n");
            } else if (strcmp(node->binary.op, "<=") == 0) {
                fprintf(output, "    cmpl %%ecx, %%eax\n");
                fprintf(output, "    setle %%al\n");
                fprintf(output, "    movzbl %%al, %%eax\n");
            } else if (strcmp(node->binary.op, ">") == 0) {
                fprintf(output, "    cmpl %%ecx, %%eax\n");
                fprintf(output, "    setg %%al\n");
                fprintf(output, "    movzbl %%al, %%eax\n");
            } else if (strcmp(node->binary.op, ">=") == 0) {
                fprintf(output, "    cmpl %%ecx, %%eax\n");
                fprintf(output, "    setge %%al\n");
                fprintf(output, "    movzbl %%al, %%eax\n");
            }
            break;
            
        case AST_ASSIGNMENT:
            ast_generate_assembly(node->binary.right, output);
            fprintf(output, "    movl %%eax, %s(%%rip)\n", node->binary.left->string_value);
            break;
            
        case AST_IF:
            ast_generate_assembly(node->if_stmt.cond, output);
            fprintf(output, "    cmpl $0, %%eax\n");
            fprintf(output, "    je .Lelse%d\n", node->line_no);
            ast_generate_assembly(node->if_stmt.then_body, output);
            fprintf(output, "    jmp .Lendif%d\n", node->line_no);
            fprintf(output, ".Lelse%d:\n", node->line_no);
            if (node->if_stmt.else_body) {
                ast_generate_assembly(node->if_stmt.else_body, output);
            }
            fprintf(output, ".Lendif%d:\n", node->line_no);
            break;
            
        case AST_WHILE:
            fprintf(output, ".Lwhile%d:\n", node->line_no);
            ast_generate_assembly(node->while_loop.cond, output);
            fprintf(output, "    cmpl $0, %%eax\n");
            fprintf(output, "    je .Lendwhile%d\n", node->line_no);
            ast_generate_assembly(node->while_loop.body, output);
            fprintf(output, "    jmp .Lwhile%d\n", node->line_no);
            fprintf(output, ".Lendwhile%d:\n", node->line_no);
            break;
            
        case AST_FOR:
            ast_generate_assembly(node->for_loop.init, output);
            fprintf(output, ".Lfor%d:\n", node->line_no);
            ast_generate_assembly(node->for_loop.cond, output);
            fprintf(output, "    cmpl $0, %%eax\n");
            fprintf(output, "    je .Lendfor%d\n", node->line_no);
            ast_generate_assembly(node->for_loop.body, output);
            ast_generate_assembly(node->for_loop.update, output);
            fprintf(output, "    jmp .Lfor%d\n", node->line_no);
            fprintf(output, ".Lendfor%d:\n", node->line_no);
            break;
            
        case AST_BLOCK:
            for (int i = 0; i < node->block.count; i++) {
                ast_generate_assembly(node->block.statements[i], output);
            }
            break;
            
        case AST_DECLARATION:
            fprintf(output, "    .comm %s, 4\n", node->decl.var_name);
            break;
            
        case AST_DECLARATION_INIT:
            fprintf(output, "    .comm %s, 4\n", node->decl.var_name);
            ast_generate_assembly(node->decl.init_value, output);
            fprintf(output, "    movl %%eax, %s(%%rip)\n", node->decl.var_name);
            break;
            
        case AST_FUNCTION_CALL:
            // 保存寄存器
            fprintf(output, "    pushq %%rdi\n");
            fprintf(output, "    pushq %%rsi\n");
            fprintf(output, "    pushq %%rdx\n");
            fprintf(output, "    pushq %%rcx\n");
            fprintf(output, "    pushq %%r8\n");
            fprintf(output, "    pushq %%r9\n");
            
            // 处理参数
            for (int i = node->func_call.arg_count - 1; i >= 0; i--) {
                ast_generate_assembly(node->func_call.args[i], output);
                if (i == 0) fprintf(output, "    movl %%eax, %%edi\n");
                else if (i == 1) fprintf(output, "    movl %%eax, %%esi\n");
                else if (i == 2) fprintf(output, "    movl %%eax, %%edx\n");
                else if (i == 3) fprintf(output, "    movl %%eax, %%ecx\n");
                else if (i == 4) fprintf(output, "    movl %%eax, %%r8d\n");
                else if (i == 5) fprintf(output, "    movl %%eax, %%r9d\n");
                else fprintf(output, "    pushq %%rax\n");
            }
            
            // 调用函数
            fprintf(output, "    call %s\n", node->func_call.func_name);
            
            // 恢复栈
            if (node->func_call.arg_count > 6) {
                fprintf(output, "    addq $%d, %%rsp\n", (node->func_call.arg_count - 6) * 8);
            }
            
            // 恢复寄存器
            fprintf(output, "    popq %%r9\n");
            fprintf(output, "    popq %%r8\n");
            fprintf(output, "    popq %%rcx\n");
            fprintf(output, "    popq %%rdx\n");
            fprintf(output, "    popq %%rsi\n");
            fprintf(output, "    popq %%rdi\n");
            break;
            
        case AST_RETURN:
            ast_generate_assembly(node->binary.left, output);
            fprintf(output, "    jmp .Lreturn%d\n", node->line_no);
            break;
            
        case AST_EMPTY:
            // 空语句不生成任何代码
            break;
            
        default:
            fprintf(stderr, "Error: Unknown AST node type %d\n", node->type);
            break;
    }
}

void ast_write_to_file(ASTNode *node, const char *filename) {
    FILE *output = fopen(filename, "w");
    if (!output) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return;
    }

    // 写入汇编文件头部
    fprintf(output, ".section .rodata\n");
    fprintf(output, ".LC0:\n");
    fprintf(output, "    .string \"%%d\\n\"\n");
    fprintf(output, ".text\n");
    fprintf(output, ".globl main\n");
    fprintf(output, "main:\n");
    fprintf(output, "    pushq %%rbp\n");
    fprintf(output, "    movq %%rsp, %%rbp\n");

    // 生成汇编代码
    ast_generate_assembly(node, output);

    // 写入汇编文件尾部
    fprintf(output, "    popq %%rbp\n");
    fprintf(output, "    ret\n");

    fclose(output);
}
