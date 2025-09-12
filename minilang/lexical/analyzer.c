#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TOKEN_LEN 1000
#define MAX_KEYWORDS 100
#define MAX_LINE_LENGTH 1024

typedef enum {
    TOKEN_EOF = 0,
    TOKEN_IDENTIFIER = 1,
    TOKEN_CONSTANT = 2,
    TOKEN_KEYWORD = 3,
    TOKEN_OPERATOR = 10,
    TOKEN_DELIMITER = 21,
    TOKEN_STRING = 25,
    TOKEN_COMMENT = 26,
    TOKEN_PREPROCESSOR = 27,
    TOKEN_ERROR = -1
} TokenType;

typedef struct {
    int code;
    char name[20];
} Keyword;

static Keyword keywords[MAX_KEYWORDS] = {
    {3, "int"}, {4, "if"}, {5, "else"}, {6, "while"}, {7, "for"},
    {8, "read"}, {9, "write"}, {13, "return"}, {11, "float"},
    {12, "void"}, {13, "char"}, {14, "double"}, {15, "long"},
    {16, "short"}, {17, "unsigned"}, {18, "signed"}, {19, "static"},
    {20, "const"}, {0, ""}
};

static char current_char = '\0';
static char token[MAX_TOKEN_LEN] = "";
static int line_number = 1;
static bool in_comment = false;
static bool in_preprocessor = false;

static void get_char() {
    current_char = getchar();
    if (current_char == '\n') {
        line_number++;
    }
}

static void skip_whitespace() {
    while (isspace(current_char) && !in_comment && !in_preprocessor) {
        get_char();
    }
}

static bool append_char() {
    size_t len = strlen(token);
    if (len < MAX_TOKEN_LEN - 1) {
        token[len] = current_char;
        token[len + 1] = '\0';
        return true;
    }
    return false;
}

static void retract() {
    if (current_char != EOF) {
        ungetc(current_char, stdin);
        if (current_char == '\n') {
            line_number--;
        }
        current_char = '\0';
    }
}

static int is_keyword() {
    for (int i = 0; keywords[i].code != 0; i++) {
        if (strcmp(keywords[i].name, token) == 0) {
            return keywords[i].code;
        }
    }
    return 0;
}

static void handle_string() {
    append_char();
    get_char();
    while (current_char != EOF && current_char != '"') {
        if (current_char == '\\') {
            append_char();
            get_char();
        }
        if (!append_char()) {
            printf("Error at line %d: String too long\n", line_number);
            return;
        }
        get_char();
    }
    if (current_char == '"') {
        append_char();
        printf("( %d , %s )\n", TOKEN_STRING, token);
    } else {
        printf("Error at line %d: Unterminated string\n", line_number);
    }
}

static void handle_comment() {
    if (current_char == '/') {
        get_char();
        if (current_char == '/') {
            // Single line comment
            while (current_char != EOF && current_char != '\n') {
                get_char();
            }
        } else if (current_char == '*') {
            // Multi-line comment
            in_comment = true;
            get_char();
            while (current_char != EOF && in_comment) {
                if (current_char == '*') {
                    get_char();
                    if (current_char == '/') {
                        in_comment = false;
                        get_char();
                    }
                } else {
                    get_char();
                }
            }
            if (in_comment) {
                printf("Error at line %d: Unterminated comment\n", line_number);
                in_comment = false;
            }
        } else {
            retract();
            printf("( 13 , - )\n"); // /
        }
    }
}

static void handle_preprocessor() {
    in_preprocessor = true;
    while (current_char != EOF && current_char != '\n' && in_preprocessor) {
        if (!append_char()) {
            printf("Error at line %d: Preprocessor directive too long\n", line_number);
            return;
        }
        get_char();
    }
    in_preprocessor = false;
    printf("( %d , %s )\n", TOKEN_PREPROCESSOR, token);
}

static void handle_number() {
    bool is_float = false;
    bool has_exponent = false;
    
    // Handle integer part
    while (isdigit(current_char)) {
        append_char();
        get_char();
    }
    
    // Handle decimal part
    if (current_char == '.') {
        is_float = true;
        append_char();
        get_char();
        while (isdigit(current_char)) {
            append_char();
            get_char();
        }
    }
    
    // Handle exponent part
    if (current_char == 'e' || current_char == 'E') {
        has_exponent = true;
        append_char();
        get_char();
        if (current_char == '+' || current_char == '-') {
            append_char();
            get_char();
        }
        while (isdigit(current_char)) {
            append_char();
            get_char();
        }
    }
    
    retract();
    printf("( %d , %s )\n", TOKEN_CONSTANT, token);
}

static TokenType analyze_token() {
    strcpy(token, "");
    get_char();
    skip_whitespace();

    if (current_char == EOF) {
        return TOKEN_EOF;
    }

    // Handle comments
    if (current_char == '/') {
        handle_comment();
        return TOKEN_COMMENT;
    }

    // Handle preprocessor directives
    if (current_char == '#') {
        handle_preprocessor();
        return TOKEN_PREPROCESSOR;
    }

    // Handle strings
    if (current_char == '"') {
        handle_string();
        return TOKEN_STRING;
    }

    // Handle identifiers and keywords
    if (isalpha(current_char) || current_char == '_') {
        while (isalnum(current_char) || current_char == '_') {
            if (!append_char()) {
                printf("Error at line %d: Identifier too long\n", line_number);
                return TOKEN_ERROR;
            }
            get_char();
        }
        retract();
        int keyword_code = is_keyword();
        if (keyword_code) {
            printf("( %d , - )\n", keyword_code);
            return TOKEN_KEYWORD;
        }
        printf("( %d , %s )\n", TOKEN_IDENTIFIER, token);
        return TOKEN_IDENTIFIER;
    }

    // Handle numbers
    if (isdigit(current_char)) {
        handle_number();
        return TOKEN_CONSTANT;
    }

    // Handle operators
    switch (current_char) {
        case '+':
            append_char();
            get_char();
            if (current_char == '=') {
                printf("( 28 , - )\n");
                return TOKEN_OPERATOR;
            } else if (current_char == '+') {
                printf("( 29 , - )\n");
                return TOKEN_OPERATOR;
            }
            retract();
            printf("( 10 , - )\n");
            return TOKEN_OPERATOR;
        case '-':
            append_char();
            get_char();
            if (current_char == '=') {
                printf("( 30 , - )\n");
                return TOKEN_OPERATOR;
            } else if (current_char == '-') {
                printf("( 31 , - )\n");
                return TOKEN_OPERATOR;
            }
            retract();
            printf("( 11 , - )\n");
            return TOKEN_OPERATOR;
        case '*':
            append_char();
            get_char();
            if (current_char == '=') {
                printf("( 32 , - )\n");
                return TOKEN_OPERATOR;
            }
            retract();
            printf("( 12 , - )\n");
            return TOKEN_OPERATOR;
        case '<':
            append_char();
            get_char();
            if (current_char == '=') {
                printf("( 15 , - )\n");
                return TOKEN_OPERATOR;
            }
            retract();
            printf("( 14 , - )\n");
            return TOKEN_OPERATOR;
        case '>':
            append_char();
            get_char();
            if (current_char == '=') {
                printf("( 17 , - )\n");
                return TOKEN_OPERATOR;
            }
            retract();
            printf("( 16 , - )\n");
            return TOKEN_OPERATOR;
        case '!':
            append_char();
            get_char();
            if (current_char == '=') {
                printf("( 18 , - )\n");
                return TOKEN_OPERATOR;
            }
            retract();
            printf("Error at line %d: Invalid operator '!'\n", line_number);
            return TOKEN_ERROR;
        case '=':
            append_char();
            get_char();
            if (current_char == '=') {
                printf("( 19 , - )\n");
                return TOKEN_OPERATOR;
            }
            retract();
            printf("( 20 , - )\n");
            return TOKEN_OPERATOR;
    }

    // Handle delimiters
    switch (current_char) {
        case '(': printf("( 21 , - )\n"); return TOKEN_DELIMITER;
        case ')': printf("( 22 , - )\n"); return TOKEN_DELIMITER;
        case ',': printf("( 23 , - )\n"); return TOKEN_DELIMITER;
        case ';': printf("( 24 , - )\n"); return TOKEN_DELIMITER;
        case '{': printf("( 25 , - )\n"); return TOKEN_DELIMITER;
        case '}': printf("( 26 , - )\n"); return TOKEN_DELIMITER;
    }

    // Error handling
    printf("Error at line %d: Invalid character '%c'\n", line_number, current_char);
    return TOKEN_ERROR;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s input_file\n", argv[0]);
        exit(1);
    }

    if (freopen(argv[1], "r", stdin) == NULL) {
        printf("Error: Cannot open file \"%s\"\n", argv[1]);
        exit(1);
    }

    while (analyze_token() != TOKEN_EOF) {
        continue;
    }

    return 0;
}
