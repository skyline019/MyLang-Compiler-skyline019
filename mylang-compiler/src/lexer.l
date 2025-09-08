%{
#include "ast.h"
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>

extern int yylineno;

%}

%option noyywrap yylineno

DIGIT   [0-9]
ID      [a-zA-Z_][a-zA-Z0-9_]*

%%

"//".*          ; /* 跳过单行注释 */
"/*"([^*]|"*"+[^*/])*"*"+"/" ; /* 跳过多行注释 */

"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"for"           { return FOR; }
"int"           { return INT; }
"float"         { return FLOAT; }
"return"        { return RETURN; }
"print"         { return PRINT; }

"=="            { return EQ; }
"!="            { return NE; }
"<="            { return LE; }
">="            { return GE; }
"&&"            { return AND; }
"||"            { return OR; }

\"([^"\\\n]|\\["\\nrt])*\" { 
    yytext[yyleng-1] = '\0';  // 去掉结束引号
    yylval.string = strdup(yytext+1);  // 去掉开始引号并复制
    return STRING; 
}

{DIGIT}+        { yylval.int_val = atoi(yytext); return INTEGER; }
{DIGIT}+"."{DIGIT}* { yylval.float_val = atof(yytext); return FLOAT; }
{ID}            { yylval.string = strdup(yytext); return IDENTIFIER; }

[ \t]           ; /* 跳过空白 */
\n              { yylineno++; }
.               { return *yytext; }

%%
