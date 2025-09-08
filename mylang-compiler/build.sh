#!/bin/bash

set -e

echo "=== 构建 MyLang 编译器 ==="

SRC_DIR="src"
BUILD_DIR="build"
EXAMPLES_DIR="examples"

mkdir -p $BUILD_DIR

# 生成语法分析器
echo "1. 生成语法分析器..."
bison -d -o $BUILD_DIR/parser.tab.c $SRC_DIR/parser.y

# 生成词法分析器
echo "2. 生成词法分析器..."
flex -o $BUILD_DIR/lexer.yy.c $SRC_DIR/lexer.l

# 编译编译器
echo "3. 编译编译器..."
gcc -I$SRC_DIR \
    -o $BUILD_DIR/mylang \
    $BUILD_DIR/parser.tab.c \
    $BUILD_DIR/lexer.yy.c \
    $SRC_DIR/ast.c \
    $SRC_DIR/main.c \
    -lm

echo "4. 构建完成！可执行文件: $BUILD_DIR/mylang"

# 测试示例
if [ -d "$EXAMPLES_DIR" ]; then
    echo ""
    echo "=== 测试示例文件 ==="
    
    for example in $EXAMPLES_DIR/*.mylang; do
        if [ -f "$example" ]; then
            echo "测试: $example"
            $BUILD_DIR/mylang "$example"
            echo "----------------------------------------"
        fi
    done
fi

echo "=== 完成 ==="
