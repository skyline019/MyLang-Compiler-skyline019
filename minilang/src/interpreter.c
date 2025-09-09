#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

extern Symbol *symbol_table;
extern int symbol_count;
extern int symbol_capacity;

static char *print_buffer = NULL;
static int print_buffer_size = 0;
static int print_buffer_capacity = 0;

void append_to_print_buffer(const char *str)
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

void free_print_buffer()
{
    if (print_buffer)
    {
        free(print_buffer);
        print_buffer = NULL;
    }
    print_buffer_size = 0;
    print_buffer_capacity = 0;
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

            // 创建新的符号表用于函数执行，但保留函数定义
            Symbol *new_symbol_table = NULL;
            int new_symbol_count = 0;
            int new_symbol_capacity = 0;

            // 将所有函数定义复制到新的符号表中
            for (int i = 0; i < symbol_count; i++)
            {
                if (symbol_table[i].is_function)
                {
                    if (new_symbol_count >= new_symbol_capacity)
                    {
                        new_symbol_capacity = new_symbol_capacity == 0 ? 8 : new_symbol_capacity * 2;
                        new_symbol_table = realloc(new_symbol_table, new_symbol_capacity * sizeof(Symbol));
                    }
                    new_symbol_table[new_symbol_count] = symbol_table[i];
                    new_symbol_count++;
                }
            }

            symbol_table = new_symbol_table;
            symbol_count = new_symbol_count;
            symbol_capacity = new_symbol_capacity;

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
void interpret_assembly_instruction(const char *instruction)
{
    if (!instruction)
        return;

    // 数据传送指令
    if (strstr(instruction, "mov") == instruction)
    {
        char src[64], dst[64];
        int value;

        // 立即数到寄存器
        if (sscanf(instruction, "movl $%d, %%%s", &value, dst) == 2)
        {
            printf("Moving immediate value %d to register %s\n", value, dst);
        }
        // 寄存器到寄存器
        else if (sscanf(instruction, "movl %%%s, %%%s", src, dst) == 2)
        {
            printf("Moving register %s to register %s\n", src, dst);
        }
        // 内存到寄存器
        else if (sscanf(instruction, "movl %[^,], %%%s", src, dst) == 2)
        {
            printf("Moving memory %s to register %s\n", src, dst);
        }
        // 寄存器到内存
        else if (sscanf(instruction, "movl %%%s, %s", src, dst) == 2)
        {
            printf("Moving register %s to memory %s\n", src, dst);
        }
    }
    // 栈操作指令
    else if (strstr(instruction, "push") == instruction)
    {
        char reg[64];
        if (sscanf(instruction, "pushq %%%s", reg) == 1)
        {
            printf("Pushing register %s onto stack\n", reg);
        }
    }
    else if (strstr(instruction, "pop") == instruction)
    {
        char reg[64];
        if (sscanf(instruction, "popq %%%s", reg) == 1)
        {
            printf("Popping stack into register %s\n", reg);
        }
    }
    // 算术运算指令
    else if (strstr(instruction, "add") == instruction)
    {
        char src[64], dst[64];
        if (sscanf(instruction, "addl %%%s, %%%s", src, dst) == 2)
        {
            printf("Adding register %s to register %s\n", src, dst);
        }
    }
    else if (strstr(instruction, "sub") == instruction)
    {
        char src[64], dst[64];
        if (sscanf(instruction, "subl %%%s, %%%s", src, dst) == 2)
        {
            printf("Subtracting register %s from register %s\n", src, dst);
        }
    }
    else if (strstr(instruction, "imul") == instruction)
    {
        char src[64], dst[64];
        if (sscanf(instruction, "imull %%%s, %%%s", src, dst) == 2)
        {
            printf("Multiplying register %s by register %s\n", src, dst);
        }
    }
    else if (strstr(instruction, "idiv") == instruction)
    {
        char reg[64];
        if (sscanf(instruction, "idivl %%%s", reg) == 1)
        {
            printf("Dividing EDX:EAX by register %s\n", reg);
        }
    }
    else if (strstr(instruction, "neg") == instruction)
    {
        char reg[64];
        if (sscanf(instruction, "negl %%%s", reg) == 1)
        {
            printf("Negating register %s\n", reg);
        }
    }
    else if (strstr(instruction, "inc") == instruction)
    {
        char reg[64];
        if (sscanf(instruction, "incl %%%s", reg) == 1)
        {
            printf("Incrementing register %s\n", reg);
        }
    }
    else if (strstr(instruction, "dec") == instruction)
    {
        char reg[64];
        if (sscanf(instruction, "decl %%%s", reg) == 1)
        {
            printf("Decrementing register %s\n", reg);
        }
    }
    // 逻辑运算指令
    else if (strstr(instruction, "and") == instruction)
    {
        char src[64], dst[64];
        if (sscanf(instruction, "andl %%%s, %%%s", src, dst) == 2)
        {
            printf("Bitwise AND of register %s and register %s\n", src, dst);
        }
    }
    else if (strstr(instruction, "or") == instruction)
    {
        char src[64], dst[64];
        if (sscanf(instruction, "orl %%%s, %%%s", src, dst) == 2)
        {
            printf("Bitwise OR of register %s and register %s\n", src, dst);
        }
    }
    else if (strstr(instruction, "xor") == instruction)
    {
        char src[64], dst[64];
        if (sscanf(instruction, "xorl %%%s, %%%s", src, dst) == 2)
        {
            printf("Bitwise XOR of register %s and register %s\n", src, dst);
        }
    }
    else if (strstr(instruction, "not") == instruction)
    {
        char reg[64];
        if (sscanf(instruction, "notl %%%s", reg) == 1)
        {
            printf("Bitwise NOT of register %s\n", reg);
        }
    }
    // 移位指令
    else if (strstr(instruction, "shl") == instruction || strstr(instruction, "sal") == instruction)
    {
        char src[64], dst[64];
        if (sscanf(instruction, "shll %%%s, %%%s", src, dst) == 2)
        {
            printf("Shifting register %s left by register %s\n", dst, src);
        }
    }
    else if (strstr(instruction, "shr") == instruction)
    {
        char src[64], dst[64];
        if (sscanf(instruction, "shrl %%%s, %%%s", src, dst) == 2)
        {
            printf("Shifting register %s right by register %s\n", dst, src);
        }
    }
    else if (strstr(instruction, "sar") == instruction)
    {
        char src[64], dst[64];
        if (sscanf(instruction, "sarl %%%s, %%%s", src, dst) == 2)
        {
            printf("Arithmetic shifting register %s right by register %s\n", dst, src);
        }
    }
    // 比较指令
    else if (strstr(instruction, "cmp") == instruction)
    {
        char src[64], dst[64];
        if (sscanf(instruction, "cmpl %%%s, %%%s", src, dst) == 2)
        {
            printf("Comparing register %s with register %s\n", src, dst);
        }
    }
    else if (strstr(instruction, "test") == instruction)
    {
        char src[64], dst[64];
        if (sscanf(instruction, "testl %%%s, %%%s", src, dst) == 2)
        {
            printf("Testing register %s and register %s\n", src, dst);
        }
    }
    // 标志设置指令
    else if (strstr(instruction, "set") == instruction)
    {
        char cond[8], reg[64];
        if (sscanf(instruction, "set%s %%%s", cond, reg) == 2)
        {
            printf("Setting register %s based on condition %s\n", reg, cond);
        }
    }
    // 跳转指令
    else if (strstr(instruction, "jmp") == instruction)
    {
        char label[64];
        if (sscanf(instruction, "jmp %s", label) == 1)
        {
            printf("Jumping to label %s\n", label);
        }
    }
    else if (strstr(instruction, "je") == instruction)
    {
        char label[64];
        if (sscanf(instruction, "je %s", label) == 1)
        {
            printf("Jumping to label %s if equal\n", label);
        }
    }
    else if (strstr(instruction, "jne") == instruction)
    {
        char label[64];
        if (sscanf(instruction, "jne %s", label) == 1)
        {
            printf("Jumping to label %s if not equal\n", label);
        }
    }
    else if (strstr(instruction, "jg") == instruction)
    {
        char label[64];
        if (sscanf(instruction, "jg %s", label) == 1)
        {
            printf("Jumping to label %s if greater\n", label);
        }
    }
    else if (strstr(instruction, "jge") == instruction)
    {
        char label[64];
        if (sscanf(instruction, "jge %s", label) == 1)
        {
            printf("Jumping to label %s if greater or equal\n", label);
        }
    }
    else if (strstr(instruction, "jl") == instruction)
    {
        char label[64];
        if (sscanf(instruction, "jl %s", label) == 1)
        {
            printf("Jumping to label %s if less\n", label);
        }
    }
    else if (strstr(instruction, "jle") == instruction)
    {
        char label[64];
        if (sscanf(instruction, "jle %s", label) == 1)
        {
            printf("Jumping to label %s if less or equal\n", label);
        }
    }
    // 函数调用指令
    else if (strstr(instruction, "call") == instruction)
    {
        char func_name[64];
        if (sscanf(instruction, "call %s", func_name) == 1)
        {
            printf("Calling function: %s\n", func_name);
        }
    }
    else if (strstr(instruction, "ret") == instruction)
    {
        printf("Return from function\n");
    }
    // 栈帧操作指令
    else if (strstr(instruction, "enter") == instruction)
    {
        int size;
        if (sscanf(instruction, "enter $%d", &size) == 1)
        {
            printf("Entering stack frame with size %d\n", size);
        }
    }
    else if (strstr(instruction, "leave") == instruction)
    {
        printf("Leaving stack frame\n");
    }
    // 其他指令
    else if (strstr(instruction, "nop") == instruction)
    {
        printf("No operation\n");
    }
    else if (strstr(instruction, "int") == instruction)
    {
        int num;
        if (sscanf(instruction, "int $%d", &num) == 1)
        {
            printf("Software interrupt %d\n", num);
        }
    }
    else
    {
        printf("Unknown instruction: %s\n", instruction);
    }
}

void interpret_assembly_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Cannot open assembly file %s\n", filename);
        return;
    }

    printf("\n=== Interpreting Assembly File: %s ===\n", filename);
    char line[256];
    int line_num = 0;

    while (fgets(line, sizeof(line), file))
    {
        line_num++;

        // 跳过空行
        if (line[0] == '\n')
            continue;

        // 移除行尾的换行符
        line[strcspn(line, "\n")] = 0;

        // 跳过注释
        char *comment = strchr(line, '#');
        if (comment)
            *comment = 0;

        // 跳过标签行
        if (strchr(line, ':'))
            continue;

        // 跳过数据段声明
        if (strstr(line, ".section") || strstr(line, ".globl") ||
            strstr(line, ".string") || strstr(line, ".comm"))
            continue;

        printf("\nLine %d: %s\n", line_num, line);
        interpret_assembly_instruction(line);
    }

    fclose(file);
}
