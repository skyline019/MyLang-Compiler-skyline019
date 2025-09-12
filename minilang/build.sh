#!/bin/bash

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 函数：检查必需的工具
check_tools() {
    local tools=("gcc" "bison" "flex" "gdb")
    for tool in "${tools[@]}"; do
        if ! command -v "$tool" &> /dev/null; then
            echo -e "${RED}❌ 错误: 未找到必需的工具 '$tool'${NC}"
            exit 1
        fi
    done
    echo -e "${GREEN}✅ 所有必需的工具都已安装${NC}"
}

# 函数：显示用法
show_usage() {
    echo -e "${BLUE}=== MyLang Compiler Build Script ===${NC}"
    echo "用法: ./build.sh [选项]"
    echo "选项:"
    echo "  build       只编译项目（不运行测试）"
    echo "  run [文件]  运行指定的.mylang文件"
    echo "  asm [文件]  生成指定文件的汇编代码"
    echo "  obj [文件]  生成指定文件的目标文件"
    echo "  exe [文件]  生成指定文件的可执行文件"
    echo "  debug [文件] 调试指定文件的可执行文件"
    echo "  test        编译并运行所有示例"
    echo "  clean       清理构建文件"
    echo "  interactive 交互模式（默认）"
    echo ""
    echo "示例:"
    echo "  ./build.sh build"
    echo "  ./build.sh run examples/hello.mylang"
    echo "  ./build.sh asm examples/hello.mylang"
    echo "  ./build.sh obj examples/hello.mylang"
    echo "  ./build.sh exe examples/hello.mylang"
    echo "  ./build.sh debug examples/hello.mylang"
    echo "  ./build.sh test"
    echo "  ./build.sh clean"
}

# 函数：编译项目
compile_only() {
    echo -e "${BLUE}=== 编译项目 ===${NC}"
    
    # 检查必需的工具
    check_tools
    
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
    gcc -o minilang ast.o symbol.o interpreter.o parser.tab.o lex.yy.o main.o
    
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
            gcc -c -o "${base_name}.o" "${base_name}.s"
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
    
    # 检查必需的工具
    check_tools
    
    # 生成汇编文件
    echo -e "${YELLOW}正在生成汇编文件...${NC}"
    ./build/minilang -S "$file"
    
    # 获取文件的基本名称和目录
    local base_name=$(basename "$file" .mylang)
    local file_dir=$(dirname "$file")
    
    # 检查汇编文件的位置
    local asm_file
    if [ "$file_dir" = "." ]; then
        asm_file="${base_name}.s"
    else
        asm_file="${file_dir}/${base_name}.s"
    fi
    
    if [ ! -f "$asm_file" ]; then
        echo -e "${RED}❌ 汇编文件未生成${NC}"
        echo -e "${YELLOW}调试信息:${NC}"
        echo "  当前目录: $(pwd)"
        echo "  期望的汇编文件: $asm_file"
        echo "  目录内容:"
        ls -la
        echo "  $file_dir 目录内容:"
        ls -la "$file_dir/"
        exit 1
    fi
    
    if [ ! -s "$asm_file" ]; then
        echo -e "${RED}❌ 汇编文件为空${NC}"
        echo -e "${YELLOW}文件内容:${NC}"
        cat "$asm_file"
        rm -f "$asm_file"
        exit 1
    fi
    
    # 显示汇编文件内容
    echo -e "${GREEN}✅ 汇编文件生成成功: $asm_file${NC}"
    echo -e "${YELLOW}文件内容:${NC}"
    cat "$asm_file"
    
    # 尝试编译汇编文件以验证其正确性
    echo -e "${YELLOW}验证汇编文件...${NC}"
    local obj_file="${base_name}.o"
    # 使用 gcc 来汇编（适用于 UCRT64）
    if gcc -c -o "$obj_file" "$asm_file" 2>&1; then
        echo -e "${GREEN}✅ 汇编文件验证成功${NC}"
        rm -f "$obj_file"
    else
        echo -e "${RED}❌ 汇编文件验证失败${NC}"
        echo -e "${YELLOW}保留汇编文件以便调试: $asm_file${NC}"
        exit 1
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
    
    # 检查必需的工具
    check_tools
    
    # 确保在正确的目录中
    local current_dir=$(pwd)
    local file_dir=$(dirname "$file")
    local file_name=$(basename "$file")
    local base_name=$(basename "$file_name" .mylang)
    
    # 切换到文件所在目录
    if [ "$file_dir" != "." ]; then
        cd "$file_dir"
    fi
    
    # 先删除可能存在的旧文件
    rm -f "${base_name}.s" "${base_name}.o"
    
    # 生成汇编文件
    echo -e "${YELLOW}正在生成汇编文件...${NC}"
    echo -e "${YELLOW}执行命令: ${current_dir}/build/minilang -S ${file_name}${NC}"
    
    # 保存编译器输出
    local compiler_output
    if [ "$file_dir" != "." ]; then
        compiler_output=$("${current_dir}/build/minilang" -S "$file_name" 2>&1)
    else
        compiler_output=$("./build/minilang" -S "$file_name" 2>&1)
    fi
    
    # 显示编译器输出
    if [ -n "$compiler_output" ]; then
        echo -e "${YELLOW}编译器输出:${NC}"
        echo "$compiler_output"
    fi
    
    # 检查编译器返回值
    if [ ${PIPESTATUS[0]} -ne 0 ]; then
        echo -e "${RED}❌ 汇编文件生成失败，编译器返回错误${NC}"
        cd "$current_dir"
        exit 1
    fi
    
    # 检查生成的汇编文件
    if [ ! -f "${base_name}.s" ]; then
        echo -e "${RED}❌ 汇编文件未生成${NC}"
        echo -e "${YELLOW}调试信息:${NC}"
        echo "  当前目录: $(pwd)"
        echo "  期望的汇编文件: ${base_name}.s"
        echo "  目录内容:"
        ls -la
        echo -e "${YELLOW}编译器完整输出:${NC}"
        echo "$compiler_output"
        cd "$current_dir"
        exit 1
    fi
    
    # 检查汇编文件是否为空
    if [ ! -s "${base_name}.s" ]; then
        echo -e "${RED}❌ 生成的汇编文件为空${NC}"
        echo -e "${YELLOW}汇编文件内容:${NC}"
        cat "${base_name}.s"
        rm -f "${base_name}.s"
        cd "$current_dir"
        exit 1
    fi
    
    # 显示汇编文件内容（前几行）
    echo -e "${YELLOW}汇编文件内容预览:${NC}"
    head -n 5 "${base_name}.s"
    
    # 编译汇编文件
    echo -e "${YELLOW}编译汇编文件...${NC}"
    echo -e "${YELLOW}执行命令: gcc -c -o ${base_name}.o ${base_name}.s${NC}"
    
    # 保存编译器输出
    local compiler_output
    compiler_output=$(gcc -c -o "${base_name}.o" "${base_name}.s" 2>&1)
    
    # 显示编译器输出
    if [ -n "$compiler_output" ]; then
        echo -e "${YELLOW}编译器输出:${NC}"
        echo "$compiler_output"
    fi
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ 目标文件生成成功: ${base_name}.o${NC}"
        # 如果不是在当前目录，移动生成的文件
        if [ "$file_dir" != "." ]; then
            mv "${base_name}.o" "$current_dir/"
        fi
        # 清理中间文件
        rm -f "${base_name}.s"
    else
        echo -e "${RED}❌ 汇编文件编译失败${NC}"
        echo -e "${YELLOW}保留汇编文件以便调试: ${base_name}.s${NC}"
        cd "$current_dir"
        exit 1
    fi
    
    # 返回原始目录
    cd "$current_dir"
    echo "========================================"
}

# 函数：生成可执行文件
generate_executable() {
    local file=$1
    
    if [ ! -f "$file" ]; then
        echo -e "${RED}❌ 文件不存在: $file${NC}"
        exit 1
    fi

    if [ ! -f "build/minilang" ]; then
        echo -e "${YELLOW}编译器未编译，正在编译...${NC}"
        compile_only
    fi
    
    echo -e "${BLUE}=== 生成可执行文件: $file ===${NC}"
    echo "========================================"
    
    # 确保在正确的目录中
    local current_dir=$(pwd)
    local file_dir=$(dirname "$file")
    local file_name=$(basename "$file")
    local base_name=$(basename "$file_name" .mylang)
    
    # 切换到文件所在目录
    if [ "$file_dir" != "." ]; then
        cd "$file_dir"
    fi
    
    # 先删除可能存在的旧文件
    rm -f "${base_name}.s" "${base_name}.o" "${base_name}.exe"
    
    # 生成汇编文件
    echo -e "${YELLOW}正在生成汇编文件...${NC}"
    "$current_dir/build/minilang" -S "$file_name"
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ 汇编文件生成失败${NC}"
        cd "$current_dir"
        exit 1
    fi
    
    # 编译汇编文件为目标文件
    echo -e "${YELLOW}编译为目标文件...${NC}"
    gcc -c -o "${base_name}.o" "${base_name}.s"
    
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ 目标文件生成失败${NC}"
        cd "$current_dir"
        exit 1
    fi
    
    # 链接生成可执行文件
    echo -e "${YELLOW}生成可执行文件...${NC}"
    gcc -o "${base_name}.exe" "${base_name}.o" -lmsvcrt
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✅ 可执行文件生成成功: ${base_name}.exe${NC}"
        # 如果不是在当前目录，移动生成的文件
        if [ "$file_dir" != "." ]; then
            mv "${base_name}.exe" "$current_dir/"
        fi
        # 清理中间文件
        rm -f "${base_name}.s" "${base_name}.o"
    else
        echo -e "${RED}❌ 可执行文件生成失败${NC}"
        cd "$current_dir"
        exit 1
    fi
    
    # 返回原始目录
    cd "$current_dir"
    echo "========================================"
}

# 函数：调试可执行文件
debug_executable() {
    local file=$1
    
    if [ ! -f "$file" ]; then
        echo -e "${RED}❌ 文件不存在: $file${NC}"
        exit 1
    fi

    if [ ! -f "build/minilang" ]; then
        echo -e "${YELLOW}编译器未编译，正在编译...${NC}"
        compile_only
    fi
    
    echo -e "${BLUE}=== 调试可执行文件: $file ===${NC}"
    echo "========================================"
    
    # 确保在正确的目录中
    local current_dir=$(pwd)
    local file_dir=$(dirname "$file")
    local file_name=$(basename "$file")
    local base_name=$(basename "$file_name" .mylang)
    
    # 切换到文件所在目录
    if [ "$file_dir" != "." ]; then
        cd "$file_dir"
    fi
    
    # 检查可执行文件是否存在
    if [ ! -f "${base_name}.exe" ]; then
        echo -e "${YELLOW}可执行文件不存在，正在生成...${NC}"
        # 先删除可能存在的旧文件
        rm -f "${base_name}.s" "${base_name}.o"
        
        # 生成汇编文件
        echo -e "${YELLOW}正在生成汇编文件...${NC}"
        "$current_dir/build/minilang" -S "$file_name"
        
        if [ $? -ne 0 ]; then
            echo -e "${RED}❌ 汇编文件生成失败${NC}"
            cd "$current_dir"
            exit 1
        fi
        
        # 编译汇编文件为目标文件
        echo -e "${YELLOW}编译为目标文件...${NC}"
        gcc -c -g -o "${base_name}.o" "${base_name}.s"
        
        if [ $? -ne 0 ]; then
            echo -e "${RED}❌ 目标文件生成失败${NC}"
            cd "$current_dir"
            exit 1
        fi
        
        # 链接生成可执行文件
        echo -e "${YELLOW}生成可执行文件...${NC}"
        gcc -g -o "${base_name}.exe" "${base_name}.o" -lmsvcrt
        
        if [ $? -ne 0 ]; then
            echo -e "${RED}❌ 可执行文件生成失败${NC}"
            cd "$current_dir"
            exit 1
        fi
        
        # 清理中间文件
        rm -f "${base_name}.s" "${base_name}.o"
    fi
    
    # 返回原始目录
    cd "$current_dir"
    
    # 启动 GDB 调试
    echo -e "${YELLOW}启动 GDB 调试...${NC}"
    echo -e "${YELLOW}常用的 GDB 命令:${NC}"
    echo "  run       - 运行程序"
    echo "  break     - 设置断点"
    echo "  step      - 单步执行"
    echo "  next      - 单步执行（跳过函数）"
    echo "  continue  - 继续执行"
    echo "  print     - 打印变量值"
    echo "  backtrace - 显示调用栈"
    echo "  quit      - 退出 GDB"
    echo "========================================"
    
    # 启动 GDB
    gdb "${base_name}.exe"
    
    echo "========================================"
}

# 函数：清理
clean_build() {
    echo -e "${YELLOW}清理构建文件...${NC}"
    rm -rf build/
    rm -f *.s *.o *.exe
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
        echo "6. 生成可执行文件"
        echo "7. 调试可执行文件"
        echo "8. 清理构建"
        echo "9. 退出"
        echo -n "请选择 (1-9): "
        
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
                echo -e "${YELLOW}可用的示例文件:${NC}"
                ls examples/*.mylang 2>/dev/null | nl
                echo -n "请输入文件编号: "
                read file_num
                files=($(ls examples/*.mylang 2>/dev/null))
                if [ ${#files[@]} -eq 0 ]; then
                    echo -e "${RED}❌ 没有找到示例文件${NC}"
                elif [ $file_num -ge 1 ] && [ $file_num -le ${#files[@]} ]; then
                    generate_executable "${files[$((file_num-1))]}"
                else
                    echo -e "${RED}❌ 无效的选择${NC}"
                fi
                ;;
            7)
                echo -e "${YELLOW}可用的示例文件:${NC}"
                ls examples/*.mylang 2>/dev/null | nl
                echo -n "请输入文件编号: "
                read file_num
                files=($(ls examples/*.mylang 2>/dev/null))
                if [ ${#files[@]} -eq 0 ]; then
                    echo -e "${RED}❌ 没有找到示例文件${NC}"
                elif [ $file_num -ge 1 ] && [ $file_num -le ${#files[@]} ]; then
                    debug_executable "${files[$((file_num-1))]}"
                else
                    echo -e "${RED}❌ 无效的选择${NC}"
                fi
                ;;
            8)
                clean_build
                ;;
            9)
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
    "exe")
        if [ -z "$2" ]; then
            echo -e "${RED}❌ 请指定要生成可执行文件的文件${NC}"
            show_usage
            exit 1
        fi
        generate_executable "$2"
        ;;
    "debug")
        if [ -z "$2" ]; then
            echo -e "${RED}❌ 请指定要调试的文件${NC}"
            show_usage
            exit 1
        fi
        debug_executable "$2"
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
