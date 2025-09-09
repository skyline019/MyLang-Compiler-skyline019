#include "ast.h"
#include "interpreter.h"
#include "symbol.h"
#include "../build/parser.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>


extern ASTNode *program_root;
extern int yyparse();
extern FILE *yyin;
extern char* yytext;

// 添加函数声明
void ast_write_to_file(ASTNode *node, const char *filename);

int main(int argc, char *argv[]) {
    printf("=== MyLang Compiler ===\n");
    
    bool generate_asm = false;
    char *input_file = NULL;
    
    // 解析命令行参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-S") == 0) {
            generate_asm = true;
        } else if (!input_file) {
            input_file = argv[i];
        }
    }
    
    if (input_file) {
        printf("Parsing file: %s\n", input_file);
        yyin = fopen(input_file, "r");
        if (!yyin) {
            fprintf(stderr, "Error: Cannot open file %s\n", input_file);
            return 1;
        }
    } else {
        printf("Please input code (Ctrl+D to end):\n");
    }
    
    int result = yyparse();
    
    if (input_file) {
        fclose(yyin);
    }
    
    if (result == 0) {
        printf("\n=== Parse Success ===\n");
        if (program_root) {
            printf("\nGenerated AST:\n");
            ast_print(program_root, 0);
            
            // 如果需要生成汇编文件
            if (generate_asm) {
                // 直接解释examples文件夹下的.s文件
                char *asm_filename = NULL;
                if (input_file) {
                    // 从输入文件名生成对应的.s文件路径
                    char *dot = strrchr(input_file, '.');
                    if (dot) {
                        size_t base_len = dot - input_file;
                        asm_filename = malloc(base_len + 15); // 预留足够空间
                        strncpy(asm_filename, "examples/", 9);
                        strncat(asm_filename, input_file, base_len);
                        strcat(asm_filename, ".s");
                    } else {
                        asm_filename = malloc(strlen(input_file) + 15);
                        strcpy(asm_filename, "examples/");
                        strcat(asm_filename, input_file);
                        strcat(asm_filename, ".s");
                    }
                } else {
                    asm_filename = strdup("examples/output.s");
                }
                printf("\n=== Program Execution ===\n");
                ast_interpret(program_root);
                
                free(asm_filename);
            } else {
                printf("\n=== Program Execution ===\n");
                ast_interpret(program_root);
            }
            
            // 清理资源
            ast_free(program_root);
            ast_free_symbol_table();
            free_print_buffer();
        }
    } else {
        printf("\n=== Parse Failed ===\n");
    }
    
    return result;
}
