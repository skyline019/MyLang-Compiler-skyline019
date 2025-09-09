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
"string"        { return STRING; }
"int[]"         { return INT_ARRAY; }
"float[]"       { return FLOAT_ARRAY; }
"string[]"      { return STRING_ARRAY; }
"return"        { return RETURN; }
"print"         { return PRINT; }
"function"      { return FUNCTION; }
":"             { return ':'; }


"=="            { return EQ; }
"!="            { return NE; }
"<="            { return LE; }
">="            { return GE; }
"&&"            { return AND; }
"||"            { return OR; }

"["             { return '['; }
"]"             { return ']'; }

\"([^"\\\n]|\\["\\nrt])*\" { 
    // 计算实际需要的缓冲区大小
    size_t buffer_size = yyleng - 1; // 减去开始和结束引号
    char *buffer = malloc(buffer_size);
    size_t buf_index = 0;
    
    // 跳过开始引号，处理字符串内容
    for (int i = 1; i < yyleng - 1 && buf_index < buffer_size - 1; i++) {
        if (yytext[i] == '\\' && i + 1 < yyleng - 1) {
            // 处理转义字符
            switch (yytext[i + 1]) {
                case '"':  buffer[buf_index++] = '"'; break;
                case '\\': buffer[buf_index++] = '\\'; break;
                case 'n':  buffer[buf_index++] = '\n'; break;
                case 'r':  buffer[buf_index++] = '\r'; break;
                case 't':  buffer[buf_index++] = '\t'; break;
                default:   buffer[buf_index++] = yytext[i + 1]; break;
            }
            i++; // 跳过转义字符的下一个字符
        } else {
            // 直接复制字符
            buffer[buf_index++] = yytext[i];
        }
    }
    
    // 添加字符串结束符
    buffer[buf_index] = '\0';
    
    // 将处理后的字符串保存到yylval
    yylval.string = buffer;
    return STRING; 
}

{DIGIT}+        { yylval.int_val = atoi(yytext); return INTEGER; }
{DIGIT}+"."{DIGIT}* { yylval.float_val = atof(yytext); return FLOAT; }
{ID}            { yylval.string = strdup(yytext); return IDENTIFIER; }

[ \t]           ; /* 跳过空白 */
\n              { yylineno++; }
.               { return *yytext; }

%%