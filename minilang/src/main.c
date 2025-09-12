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
extern char *yytext;

// 添加函数声明
void ast_write_to_file(ASTNode *node, const char *filename);

int main(int argc, char *argv[])
{
    printf("=== MyLang Compiler ===\n");

    bool generate_asm = false;
    char *input_file = NULL;

    // 解析命令行参数
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-S") == 0)
        {
            generate_asm = true;
        }
        else if (!input_file)
        {
            input_file = argv[i];
        }
    }

    if (input_file)
    {
        printf("Parsing file: %s\n", input_file);
        yyin = fopen(input_file, "r");
        if (!yyin)
        {
            fprintf(stderr, "Error: Cannot open file %s\n", input_file);
            return 1;
        }
    }
    else
    {
        printf("Please input code (Ctrl+D to end):\n");
    }

    int result = yyparse();

    if (input_file)
    {
        fclose(yyin);
    }

    if (result == 0)
    {
        printf("\n=== Parse Success ===\n");
        if (program_root)
        {
            printf("\nGenerated AST:\n");
            ast_print(program_root, 0);

            // 如果需要生成汇编文件
            if (generate_asm)
            {
                char *asm_filename = NULL;
                if (input_file)
                {
                    // 获取文件名（不含路径）
                    char *basename = strrchr(input_file, '/');
                    if (!basename)
                        basename = strrchr(input_file, '\\');
                    basename = basename ? basename + 1 : input_file;

                    // 去掉扩展名
                    char *dot = strrchr(basename, '.');
                    size_t base_len = dot ? dot - basename : strlen(basename);

                    // 分配内存并构建完整路径
                    asm_filename = malloc(strlen(input_file) + 4);
                    if (dot)
                    {
                        strncpy(asm_filename, input_file, dot - input_file);
                        strcpy(asm_filename + (dot - input_file), ".s");
                    }
                    else
                    {
                        strcpy(asm_filename, input_file);
                        strcat(asm_filename, ".s");
                    }
                }
                else
                {
                    asm_filename = strdup("output.s"); // 默认输出文件名
                }

                printf("\n=== Generating Assembly ===\n");
                printf("Attempting to create file: %s\n", asm_filename);

                FILE *asm_file = fopen(asm_filename, "w");
                if (asm_file)
                {
                    ast_write_to_file(program_root, asm_filename);
                    fclose(asm_file);
                    printf("Assembly file generated: %s\n", asm_filename);
                }
                else
                {
                    fprintf(stderr, "Error: Cannot open assembly file %s for writing\n", asm_filename);
                    perror("fopen failed");
                }

                free(asm_filename);
            }

            else
            {
                printf("\n=== Program Execution ===\n");
                ast_interpret(program_root);
            }

            // 清理资源
            ast_free(program_root);
            ast_free_symbol_table();
            free_print_buffer();
        }
    }
    else
    {
        printf("\n=== Parse Failed ===\n");
    }

    return result;
}
