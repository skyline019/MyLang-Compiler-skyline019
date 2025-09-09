#!/bin/bash

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 函数：显示用法
show_usage() {
    echo -e "${BLUE}=== MyLang Compiler Build Script ===${NC}"
    echo "用法: ./build.sh [选项]"
    echo "选项:"
    echo "  build       只编译项目（不运行测试）"
    echo "  run [文件]  运行指定的.mylang文件"
    echo "  asm [文件]  生成指定文件的汇编代码"
    echo "  obj [文件]  生成指定文件的目标文件"
    echo "  test        编译并运行所有示例"
    echo "  clean       清理构建文件"
    echo "  interactive 交互模式（默认）"
    echo ""
    echo "示例:"
    echo "  ./build.sh build"
    echo "  ./build.sh run examples/hello.mylang"
    echo "  ./build.sh asm examples/hello.mylang"
    echo "  ./build.sh obj examples/hello.mylang"
    echo "  ./build.sh test"
    echo "  ./build.sh clean"
}

# 函数：编译项目
compile_only() {
    echo -e "${BLUE}=== 编译项目 ===${NC}"
    
    # 创建构建目录
    mkdir -p build
    cd build
    
    # 生成解析器
    echo -e "${YELLOW}生成解析器...${NC}"
    bison -d ../src/parser.y -o parser.tab.c
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ Bison 失败！${NC}"
        exit 1
    fi
    
    # 生成词法分析器
    echo -e "${YELLOW}生成词法分析器...${NC}"
    flex -o lex.yy.c ../src/lexer.l
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ Flex 失败！${NC}"
        exit 1
    fi
    
    # 编译所有文件
    echo -e "${YELLOW}编译源文件...${NC}"
    gcc -c ../src/ast.c -I../src
    gcc -c ../src/symbol.c -I../src
    gcc -c ../src/interpreter.c -I../src
    gcc -c parser.tab.c -I../src
    gcc -c lex.yy.c -I../src
    gcc -c ../src/main.c -I../src

    # 链接
    echo -e "${YELLOW}链接...${NC}"
    gcc -o minilang ast.o symbol.o interpreter.o parser.tab.o lex.yy.o main.o -lm
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ 编译成功！可执行文件: build/minilang${NC}"
        cd ..
    else
        echo -e "${RED}❌ 链接失败！${NC}"
        exit 1
    fi
}

# 函数：运行单个文件
run_file() {
    local file=$1
    local generate_asm=$2
    
    if [ ! -f "$file" ]; then
        echo -e "${RED}❌ 文件不存在: $file${NC}"
        exit 1
    fi

    if [ ! -f "build/minilang" ]; then
        echo -e "${YELLOW}编译器未编译，正在编译...${NC}"
        compile_only
    fi
    
    echo -e "${BLUE}=== 运行: $file ===${NC}"
    echo "========================================"
    
    if [ "$generate_asm" = true ]; then
        ./build/minilang -S "$file"
        # 获取不带扩展名的文件名
        local base_name=$(basename "$file" .mylang)
        # 如果汇编文件生成成功，则编译为目标文件
        if [ -f "${base_name}.s" ]; then
            echo -e "${YELLOW}编译汇编文件...${NC}"
            as -o "${base_name}.o" "${base_name}.s"
            if [ $? -eq 0 ]; then
                echo -e "${GREEN}✅ 目标文件生成成功: ${base_name}.o${NC}"
            else
                echo -e "${RED}❌ 汇编文件编译失败${NC}"
            fi
        fi
    else
        ./build/minilang "$file"
    fi
    
    echo "========================================"
}

# 函数：运行所有测试
run_tests() {
    echo -e "${BLUE}=== 运行所有示例 ===${NC}"
    
    if [ ! -d "examples" ]; then
        echo -e "${YELLOW}ℹ️  示例目录不存在，跳过测试${NC}"
        return
    fi
    
    # 先编译
    compile_only
    
    # 运行所有示例文件
    for file in examples/*.mylang; do
        if [ -f "$file" ]; then
            echo ""
            echo -e "${GREEN}📁 运行: $file${NC}"
            echo "========================================"
            ./build/minilang "$file"
            echo "========================================"
            echo ""
            read -p "按回车继续下一个示例..." -r
        fi
    done
}

# 函数：生汇编文件
generate_assembly() {
    local file=$1
    
    if [ ! -f "$file" ]; then
        echo -e "${RED}❌ 文件不存在: $file${NC}"
        exit 1
    fi

    if [ ! -f "build/minilang" ]; then
        echo -e "${YELLOW}编译器未编译，正在编译...${NC}"
        compile_only
    fi
    
    echo -e "${BLUE}=== 生成汇编文件: $file ===${NC}"
    echo "========================================"
    
    # 生成汇编文件
    ./build/minilang -S "$file"
    
    # 获取不带扩展名的文件名
    local base_name=$(basename "$file" .mylang)
    
    # 如果汇编文件生成成功，则编译为目标文件
    if [ -f "${base_name}.s" ]; then
        echo -e "${YELLOW}编译汇编文件...${NC}"
        as -o "${base_name}.o" "${base_name}.s"
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}✅ 目标文件生成成功: ${base_name}.o${NC}"
        else
            echo -e "${RED}❌ 汇编文件编译失败${NC}"
        fi
    fi
    
    echo "========================================"
}

# 函数：生成目标文件
generate_object() {
    local file=$1
    
    if [ ! -f "$file" ]; then
        echo -e "${RED}❌ 文件不存在: $file${NC}"
        exit 1
    fi

    if [ ! -f "build/minilang" ]; then
        echo -e "${YELLOW}编译器未编译，正在编译...${NC}"
        compile_only
    fi
    
    echo -e "${BLUE}=== 生成目标文件: $file ===${NC}"
    echo "========================================"
    
    # 获取不带扩展名的文件名
    local base_name=$(basename "$file" .mylang)
    
    # 生成汇编文件
    ./build/minilang -S "$file"
    
    # 如果汇编文件生成成功，则编译为目标文件
    if [ -f "${base_name}.s" ]; then
        echo -e "${YELLOW}编译汇编文件...${NC}"
        as -o "${base_name}.o" "${base_name}.s"
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}✅ 目标文件生成成功: ${base_name}.o${NC}"
            # 清理中间文件
            rm -f "${base_name}.s"
        else
            echo -e "${RED}❌ 汇编文件编译失败${NC}"
        fi
    else
        echo -e "${RED}❌ 汇编文件生成失败${NC}"
    fi
    
    echo "========================================"
}

# 函数：清理
clean_build() {
    echo -e "${YELLOW}清理构建文件...${NC}"
    rm -rf build/
    rm -f *.s *.o
    echo -e "${GREEN}✅ 清理完成${NC}"
}

# 函数：交互模式
interactive_mode() {
    while true; do
        echo -e "${BLUE}=== MyLang 编译器交互菜单 ===${NC}"
        echo "1. 编译项目"
        echo "2. 运行示例文件"
        echo "3. 运行所有测试"
        echo "4. 生成汇编文件"
        echo "5. 生成目标文件"
        echo "6. 清理构建"
        echo "7. 退出"
        echo -n "请选择 (1-7): "
        
        read choice
        
        case $choice in
            1)
                compile_only
                ;;
            2)
                echo -e "${YELLOW}可用的示例文件:${NC}"
                ls examples/*.mylang 2>/dev/null | nl
                echo -n "请输入文件编号: "
                read file_num
                files=($(ls examples/*.mylang 2>/dev/null))
                if [ ${#files[@]} -eq 0 ]; then
                    echo -e "${RED}❌ 没有找到示例文件${NC}"
                elif [ $file_num -ge 1 ] && [ $file_num -le ${#files[@]} ]; then
                    run_file "${files[$((file_num-1))]}" false
                else
                    echo -e "${RED}❌ 无效的选择${NC}"
                fi
                ;;
            3)
                run_tests
                ;;
            4)
                echo -e "${YELLOW}可用的示例文件:${NC}"
                ls examples/*.mylang 2>/dev/null | nl
                echo -n "请输入文件编号: "
                read file_num
                files=($(ls examples/*.mylang 2>/dev/null))
                if [ ${#files[@]} -eq 0 ]; then
                    echo -e "${RED}❌ 没有找到示例文件${NC}"
                elif [ $file_num -ge 1 ] && [ $file_num -le ${#files[@]} ]; then
                    generate_assembly "${files[$((file_num-1))]}"
                else
                    echo -e "${RED}❌ 无效的选择${NC}"
                fi
                ;;
            5)
                echo -e "${YELLOW}可用的示例文件:${NC}"
                ls examples/*.mylang 2>/dev/null | nl
                echo -n "请输入文件编号: "
                read file_num
                files=($(ls examples/*.mylang 2>/dev/null))
                if [ ${#files[@]} -eq 0 ]; then
                    echo -e "${RED}❌ 没有找到示例文件${NC}"
                elif [ $file_num -ge 1 ] && [ $file_num -le ${#files[@]} ]; then
                    generate_object "${files[$((file_num-1))]}"
                else
                    echo -e "${RED}❌ 无效的选择${NC}"
                fi
                ;;
            6)
                clean_build
                ;;
            7)
                echo -e "${GREEN}再见！👋${NC}"
                exit 0
                ;;
            *)
                echo -e "${RED}❌ 无效的选择${NC}"
                ;;
        esac
        
        echo ""
    done
}

# 主程序
case "$1" in
    "build")
        compile_only
        ;;
    "run")
        if [ -z "$2" ]; then
            echo -e "${RED}❌ 请指定要运行的文件${NC}"
            show_usage
            exit 1
        fi
        run_file "$2" false
        ;;
    "asm")
        if [ -z "$2" ]; then
            echo -e "${RED}❌ 请指定要生成汇编的文件${NC}"
            show_usage
            exit 1
        fi
        generate_assembly "$2"
        ;;
    "obj")
        if [ -z "$2" ]; then
            echo -e "${RED}❌ 请指定要生成目标文件的文件${NC}"
            show_usage
            exit 1
        fi
        generate_object "$2"
        ;;
    "test")
        run_tests
        ;;
    "clean")
        clean_build
        ;;
    "interactive"|"")
        interactive_mode
        ;;
    "-h"|"--help")
        show_usage
        ;;
    *)
        echo -e "${RED}❌ 未知选项: $1${NC}"
        show_usage
        exit 1
        ;;
esac
