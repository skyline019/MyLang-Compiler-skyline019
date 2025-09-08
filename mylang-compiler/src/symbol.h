#ifndef SYMBOL_H
#define SYMBOL_H

typedef enum {
    SYM_INT,
    SYM_FLOAT,
    SYM_STRING,
    SYM_INT_ARRAY,
    SYM_FLOAT_ARRAY,
    SYM_STRING_ARRAY,
    SYM_FUNCTION
} SymbolType;

typedef struct Symbol {
    char *name;
    SymbolType type;
    int defined;
    char *string_value;  // 用于存储字符串类型的值
    int array_size;      // 数组大小
    void *array_data;    // 数组数据指针
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
