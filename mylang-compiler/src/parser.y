%{
#include "../src/ast.h"
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yylineno;
extern FILE *yyin;
extern char* yytext;

void yyerror(const char *s);
ASTNode *program_root;

%}

%union {
    int int_val;
    float float_val;
    char *string;
    ASTNode *ast;
}

%token <int_val> INTEGER
%token <float_val> FLOAT
%token <string> IDENTIFIER STRING
%token IF ELSE WHILE FOR INT RETURN PRINT FUNCTION
%token INT_ARRAY FLOAT_ARRAY STRING_ARRAY
%token EQ NE LE GE AND OR
%token IFX '[' ']'

%type <ast> program stmt_list stmt expr decl block if_stmt while_stmt for_stmt
%type <ast> logical_expr equality_expr rel_expr add_expr mul_expr primary expr_list
%type <ast> array_decl array_access function_def formatted_print param_list param
%type <string> type_specifier

%%

program: stmt_list {
    program_root = $1;
    printf("Parse successful! Generated AST:\n");
    ast_print(program_root, 0);
    YYACCEPT;
}
;

stmt_list: stmt { 
    ASTNode **stmts = malloc(sizeof(ASTNode *));
    stmts[0] = $1;
    $$ = ast_new_block(stmts, 1, yylineno);
}
| stmt_list stmt {
    ASTNode **new_stmts = realloc($1->block.statements, 
                                 ($1->block.count + 1) * sizeof(ASTNode *));
    new_stmts[$1->block.count] = $2;
    $1->block.statements = new_stmts;
    $1->block.count++;
    $$ = $1;
}
;

stmt: expr ';' { $$ = $1; }
| decl ';' { $$ = $1; }
| array_decl ';' { $$ = $1; }
| if_stmt { $$ = $1; }
| while_stmt { $$ = $1; }
| for_stmt { $$ = $1; }
| RETURN expr ';' { $$ = ast_new_return($2, yylineno); }
| PRINT expr ';' { 
    ASTNode *args[] = {$2};
    $$ = ast_new_function_call("print", args, 1, yylineno);
}
| formatted_print ';' { $$ = $1; }  // 添加格式化输出
| function_def { $$ = $1; }         // 添加函数定义
| block { $$ = $1; }
| ';' { $$ = ast_new_empty(yylineno); }
;

// 添加格式化输出语法
formatted_print: PRINT '(' STRING ')' { 
    // 检查格式字符串是否包含格式说明符
    char *format = $3;
    int format_count = 0;
    for (int i = 0; format[i]; i++) {
        if (format[i] == '%' && format[i + 1] != '%') {
            format_count++;
        }
    }
    
    if (format_count > 0) {
        fprintf(stderr, "Error: Format string contains %d format specifiers but no arguments provided\n", format_count);
        YYERROR;
    }
    
    ASTNode **args = NULL;
    $$ = ast_new_formatted_print($3, args, 0, yylineno);
}
| PRINT '(' STRING ',' expr ')' { 
    // 检查格式字符串中的格式说明符数量
    char *format = $3;
    int format_count = 0;
    for (int i = 0; format[i]; i++) {
        if (format[i] == '%' && format[i + 1] != '%') {
            format_count++;
        }
    }
    
    if (format_count != 1) {
        fprintf(stderr, "Error: Format string should contain exactly 1 format specifier\n");
        YYERROR;
    }
    
    // 创建参数数组
    ASTNode **args = malloc(sizeof(ASTNode *));
    args[0] = $5;
    
    $$ = ast_new_formatted_print($3, args, 1, yylineno);
}
| PRINT '(' STRING ',' expr ',' expr ')' { 
    // 检查格式字符串中的格式说明符数量
    char *format = $3;
    int format_count = 0;
    for (int i = 0; format[i]; i++) {
        if (format[i] == '%' && format[i + 1] != '%') {
            format_count++;
        }
    }
    
    if (format_count != 2) {
        fprintf(stderr, "Error: Format string should contain exactly 2 format specifiers\n");
        YYERROR;
    }
    
    // 创建参数数组
    ASTNode **args = malloc(2 * sizeof(ASTNode *));
    args[0] = $5;
    args[1] = $7;
    
    $$ = ast_new_formatted_print($3, args, 2, yylineno);
}
;

function_def: FUNCTION IDENTIFIER '(' param_list ')' ':' type_specifier block {
    $$ = ast_new_function_def($2, $7, $4, $8, yylineno);
}
;

param_list: /* empty */ { 
    ASTNode **params = NULL;
    $$ = ast_new_block(params, 0, yylineno);
}
| param { 
    ASTNode **params = malloc(sizeof(ASTNode *));
    params[0] = $1;
    $$ = ast_new_block(params, 1, yylineno);
}
| param_list ',' param {
    ASTNode **new_params = realloc($1->block.statements, 
                                  ($1->block.count + 1) * sizeof(ASTNode *));
    new_params[$1->block.count] = $3;
    $1->block.statements = new_params;
    $1->block.count++;
    $$ = $1;
}
;

param: IDENTIFIER ':' type_specifier {
    $$ = ast_new_param_declaration($1, $3, yylineno);
}
;

// 添加类型说明符规则
type_specifier: INT { $$ = strdup("int"); }
| FLOAT { $$ = strdup("float"); }
| STRING { $$ = strdup("string"); }
| INT_ARRAY { $$ = strdup("int[]"); }
| FLOAT_ARRAY { $$ = strdup("float[]"); }  // 添加 $$ =
| STRING_ARRAY { $$ = strdup("string[]"); }  // 添加 $$ =
;

decl: INT IDENTIFIER { $$ = ast_new_declaration($2, yylineno); }
| INT IDENTIFIER '=' expr { $$ = ast_new_declaration_init($2, $4, yylineno); }
| FLOAT IDENTIFIER { $$ = ast_new_declaration($2, yylineno); }
| FLOAT IDENTIFIER '=' expr { $$ = ast_new_declaration_init($2, $4, yylineno); }
| STRING IDENTIFIER { $$ = ast_new_declaration($2, yylineno); }
| STRING IDENTIFIER '=' expr { $$ = ast_new_declaration_init($2, $4, yylineno); }
;

array_decl: INT_ARRAY IDENTIFIER { $$ = ast_new_array_declaration($2, NULL, yylineno); }
| INT_ARRAY IDENTIFIER '[' expr ']' { $$ = ast_new_array_declaration($2, $4, yylineno); }
| FLOAT_ARRAY IDENTIFIER { $$ = ast_new_array_declaration($2, NULL, yylineno); }
| FLOAT_ARRAY IDENTIFIER '[' expr ']' { $$ = ast_new_array_declaration($2, $4, yylineno); }
| STRING_ARRAY IDENTIFIER { $$ = ast_new_array_declaration($2, NULL, yylineno); }
| STRING_ARRAY IDENTIFIER '[' expr ']' { $$ = ast_new_array_declaration($2, $4, yylineno); }
;

block: '{' stmt_list '}' { $$ = $2; }
| '{' '}' { 
    ASTNode **stmts = NULL;
    $$ = ast_new_block(stmts, 0, yylineno);
}
;

if_stmt: IF '(' expr ')' stmt %prec IFX { 
    $$ = ast_new_if($3, $5, NULL, yylineno); 
}
| IF '(' expr ')' stmt ELSE stmt { 
    $$ = ast_new_if($3, $5, $7, yylineno); 
}
;

while_stmt: WHILE '(' expr ')' stmt { 
    $$ = ast_new_while($3, $5, yylineno); 
}
;

for_stmt: FOR '(' expr ';' expr ';' expr ')' stmt { 
    $$ = ast_new_for($3, $5, $7, $9, yylineno); 
}
;

expr: IDENTIFIER '=' expr { 
    $$ = ast_new_assignment(ast_new_variable($1, yylineno), $3, yylineno); 
}
| array_access '=' expr {
    $$ = ast_new_array_assignment($1, $3, yylineno);
}
| logical_expr { $$ = $1; }
;

logical_expr: logical_expr AND equality_expr { 
    $$ = ast_new_binary_op("&&", $1, $3, yylineno); 
}
| logical_expr OR equality_expr { 
    $$ = ast_new_binary_op("||", $1, $3, yylineno); 
}
| equality_expr { $$ = $1; }
;

equality_expr: equality_expr EQ rel_expr { 
    $$ = ast_new_binary_op("==", $1, $3, yylineno); 
}
| equality_expr NE rel_expr { 
    $$ = ast_new_binary_op("!=", $1, $3, yylineno); 
}
| rel_expr { $$ = $1; }
;

rel_expr: rel_expr '<' add_expr { 
    $$ = ast_new_binary_op("<", $1, $3, yylineno); 
}
| rel_expr '>' add_expr { 
    $$ = ast_new_binary_op(">", $1, $3, yylineno); 
}
| rel_expr LE add_expr { 
    $$ = ast_new_binary_op("<=", $1, $3, yylineno); 
}
| rel_expr GE add_expr { 
    $$ = ast_new_binary_op(">=", $1, $3, yylineno); 
}
| add_expr { $$ = $1; }
;

add_expr: add_expr '+' mul_expr { 
    $$ = ast_new_binary_op("+", $1, $3, yylineno); 
}
| add_expr '-' mul_expr { 
    $$ = ast_new_binary_op("-", $1, $3, yylineno); 
}
| mul_expr { $$ = $1; }
;

mul_expr: mul_expr '*' primary { 
    $$ = ast_new_binary_op("*", $1, $3, yylineno); 
}
| mul_expr '/' primary { 
    $$ = ast_new_binary_op("/", $1, $3, yylineno); 
}
| mul_expr '%' primary { 
    $$ = ast_new_binary_op("%", $1, $3, yylineno); 
}
| primary { $$ = $1; }
;

primary: INTEGER { $$ = ast_new_integer($1, yylineno); }
| FLOAT { $$ = ast_new_float($1, yylineno); }
| STRING { $$ = ast_new_string($1, yylineno); }
| IDENTIFIER { $$ = ast_new_variable($1, yylineno); }
| array_access { $$ = $1; }
| '(' expr ')' { $$ = $2; }
| IDENTIFIER '(' ')' {
    ASTNode **args = NULL;
    $$ = ast_new_function_call($1, args, 0, yylineno);
}
| IDENTIFIER '(' expr_list ')' {
    // 正确解析参数列表
    int arg_count = 0;
    ASTNode *current = $3;
    
    // 计算参数数量
    while (current != NULL) {
        arg_count++;
        current = current->binary.right;
    }
    
    // 创建参数数组
    ASTNode **args = malloc(arg_count * sizeof(ASTNode *));
    current = $3;
    for (int i = 0; i < arg_count; i++) {
        args[i] = current->binary.left;
        current = current->binary.right;
    }
    
    $$ = ast_new_function_call($1, args, arg_count, yylineno);
    
    // 释放临时的表达式列表节点
    current = $3;
    while (current != NULL) {
        ASTNode *next = current->binary.right;
        free(current->binary.op);
        free(current);
        current = next;
    }
}
;

array_access: IDENTIFIER '[' expr ']' {
    $$ = ast_new_array_access($1, $3, yylineno);
}
;

expr_list: expr {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY_OP;
    node->line_no = yylineno;
    node->binary.op = strdup(",");
    node->binary.left = $1;
    node->binary.right = NULL;
    $$ = node;
}
| expr_list ',' expr {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY_OP;
    node->line_no = yylineno;
    node->binary.op = strdup(",");
    node->binary.left = $3;
    node->binary.right = NULL;
    
    ASTNode *current = $1;
    while (current->binary.right != NULL) {
        current = current->binary.right;
    }
    current->binary.right = node;
    
    $$ = $1;
}
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error at line %d near token: %s\n", yylineno, yytext);
}
