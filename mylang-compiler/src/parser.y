%{
#include "../src/ast.h"
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yylineno;
extern FILE *yyin;
extern char* yytext;  // 添加这行以获取当前token

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
%token IF ELSE WHILE FOR INT RETURN PRINT
%token EQ NE LE GE AND OR
%token LBRACKET RBRACKET
%token IFX

%type <ast> program stmt_list stmt expr decl block if_stmt while_stmt for_stmt
%type <ast> logical_expr equality_expr rel_expr add_expr mul_expr primary expr_list
%type <ast> array_access type

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
| if_stmt { $$ = $1; }
| while_stmt { $$ = $1; }
| for_stmt { $$ = $1; }
| RETURN expr ';' { $$ = ast_new_return($2, yylineno); }
| PRINT expr ';' { 
    ASTNode *args[] = {$2};
    $$ = ast_new_function_call("print", args, 1, yylineno);
}
| block { $$ = $1; }
| ';' { $$ = ast_new_empty(yylineno); }
;

type: INT { /* 无需操作，类型信息在AST节点中处理 */ }
| FLOAT { /* 无需操作，类型信息在AST节点中处理 */ }
| STRING { /* 无需操作，类型信息在AST节点中处理 */ }
;

decl: type IDENTIFIER { $$ = ast_new_declaration($2, yylineno); }
| type IDENTIFIER '=' expr { $$ = ast_new_declaration_init($2, $4, yylineno); }
| type IDENTIFIER '[' expr ']' { $$ = ast_new_array_declaration($2, $4, yylineno); }
;

array_access: IDENTIFIER '[' expr ']' { $$ = ast_new_array_access($1, $3, yylineno); }
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
| array_access '=' expr { $$ = ast_new_array_assignment($1, $3, yylineno); }
| logical_expr { $$ = $1; }
| array_access { $$ = $1; }
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
| '(' expr ')' { $$ = $2; }
| IDENTIFIER '[' expr ']' { $$ = ast_new_array_access($1, $3, yylineno); }
| IDENTIFIER '(' ')' {
    ASTNode **args = NULL;
    $$ = ast_new_function_call($1, args, 0, yylineno);
}
| IDENTIFIER '(' expr_list ')' {
    // 这里需要正确处理参数计数
    int arg_count = 0;
    ASTNode *current = $3;
    while (current != NULL) {
        arg_count++;
        current = current->binary.right; // 假设参数以链表形式存储
    }
    
    // 将参数链表转换为数组
    ASTNode **args = malloc(arg_count * sizeof(ASTNode *));
    current = $3;
    for (int i = 0; i < arg_count; i++) {
        args[i] = current->binary.left;
        current = current->binary.right;
    }
    
    $$ = ast_new_function_call($1, args, arg_count, yylineno);
    
    // 释放临时链表结构
    ast_free($3);
}
;

expr_list: expr {
    // 创建链表节点来存储参数
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY_OP;
    node->line_no = yylineno;
    node->binary.op = strdup(",");
    node->binary.left = $1;
    node->binary.right = NULL;
    $$ = node;
}
| expr_list ',' expr {
    // 添加到链表末尾
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = AST_BINARY_OP;
    node->line_no = yylineno;
    node->binary.op = strdup(",");
    node->binary.left = $3;
    node->binary.right = NULL;
    
    // 找到链表末尾
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
