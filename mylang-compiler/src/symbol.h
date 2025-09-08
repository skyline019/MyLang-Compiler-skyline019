#ifndef SYMBOL_H
#define SYMBOL_H

typedef enum {
    SYM_INT,
    SYM_FLOAT,
    SYM_STRING,
    SYM_ARRAY,
    SYM_FUNCTION
} SymbolType;

typedef struct Symbol {
    char *name;
    SymbolType type;
    int defined;
    char *string_value;  // 用于存储字符串类型的值
    int *array_data;     // 用于存储数组数据
    int array_size;      // 数组大小
    SymbolType array_type; // 数组元素类型
} Symbol;

typedef struct SymbolTable {
    Symbol *symbols;
    int count;
    int capacity;
} SymbolTable;

void symtab_init();
void symtab_add(char *name, SymbolType type);
Symbol *symtab_find(char *name);
void symtab_dump();

#endif
