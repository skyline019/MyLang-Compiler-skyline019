#include "ast.h"
#include "../build/parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>  // 添加这行以包含dup, dup2和close函数的声明

extern ASTNode *program_root;
extern int yyparse();
extern FILE *yyin;
extern char* yytext;  // 添加这行以获取当前token

int main(int argc, char *argv[]) {
    printf("=== MyLang Compiler ===\n");
    
    if (argc > 1) {
        printf("Parsing file: %s\n", argv[1]);
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
            return 1;
        }
    } else {
        printf("Please input code (Ctrl+D to end):\n");
    }
    
    int result = yyparse();
    
    if (argc > 1) {
        fclose(yyin);
    }
    
    if (result == 0) {
        printf("\n=== Parse Success ===\n");
        if (program_root) {
            printf("\nGenerated AST:\n");
            ast_print(program_root, 0);
                // 在打印后进行解释
            printf("\n=== Program Execution ===\n");
            ast_interpret(program_root);
            // 清理资源
            ast_free(program_root);
            ast_free_symbol_table();
        }
    } else {
        printf("\n=== Parse Failed ===\n");
    }
    
    return result;
}
