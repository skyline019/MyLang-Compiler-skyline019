#include "symbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// 全局符号表
Symbol *symbol_table = NULL;
int symbol_count = 0;
int symbol_capacity = 0;

Symbol *find_symbol(const char *name)
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

void set_symbol(const char *name, int int_value, float float_value, char *string_value, int type, bool is_function)
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
            symbol_table[i].int_value = int_value;
            symbol_table[i].float_value = float_value;
            if (symbol_table[i].string_value != NULL)
            {
                free(symbol_table[i].string_value);
            }
            symbol_table[i].string_value = string_value ? strdup(string_value) : NULL;
            symbol_table[i].type = type;
            symbol_table[i].is_initialized = true;
            symbol_table[i].is_function = is_function;
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

    symbol_table[symbol_count].name = strdup(name);
    symbol_table[symbol_count].int_value = int_value;
    symbol_table[symbol_count].float_value = float_value;
    symbol_table[symbol_count].string_value = string_value ? strdup(string_value) : NULL;
    symbol_table[symbol_count].type = type;
    symbol_table[symbol_count].array_size = 0;
    symbol_table[symbol_count].array_data = NULL;
    symbol_table[symbol_count].is_initialized = false;
    symbol_table[symbol_count].is_function = is_function;
    symbol_table[symbol_count].param_types = NULL;
    symbol_table[symbol_count].param_count = 0;
    symbol_table[symbol_count].function_def = NULL;
    symbol_count++;
}

void set_function_params(const char *name, char **param_types, int param_count)
{
    if (name == NULL || param_types == NULL)
        return;

    Symbol *sym = find_symbol(name);
    if (sym && sym->is_function)
    {
        sym->param_types = param_types;
        sym->param_count = param_count;
    }
}

void set_function_def(const char *name, ASTNode *func_def)
{
    if (name == NULL || func_def == NULL || func_def->type != AST_FUNCTION_DEF)
        return;

    Symbol *sym = find_symbol(name);
    if (sym && sym->is_function)
    {
        sym->function_def = func_def;
    }
}

int get_type_from_string(const char *type_str)
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

void free_current_symbol_table()
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
    }
    if (symbol_table != NULL)
    {
        free(symbol_table);
    }
    symbol_table = NULL;
    symbol_count = 0;
    symbol_capacity = 0;
}

void ast_free_symbol_table()
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