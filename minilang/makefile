CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./src
BISON = bison
FLEX = flex

SRCDIR = src
BUILDDIR = build
TARGET = $(BUILDDIR)/minilang

SRCS = $(SRCDIR)/ast.c $(SRCDIR)/symbol.c $(SRCDIR)/interpreter.c $(SRCDIR)/main.c
OBJS = $(BUILDDIR)/ast.o $(BUILDDIR)/symbol.o $(BUILDDIR)/interpreter.o $(BUILDDIR)/main.o
PARSER_SRCS = $(BUILDDIR)/parser.tab.c $(BUILDDIR)/lex.yy.c
PARSER_OBJS = $(BUILDDIR)/parser.tab.o $(BUILDDIR)/lex.yy.o

.PHONY: all build run test clean interactive help

all: build

build: $(TARGET)

$(TARGET): $(OBJS) $(PARSER_OBJS)
	$(CC) -o $@ $^ -lm
	@echo "✅ 编译完成: $(TARGET)"

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/parser.tab.c: $(SRCDIR)/parser.y
	$(BISON) -d $< -o $@

$(BUILDDIR)/lex.yy.c: $(SRCDIR)/lexer.l
	$(FLEX) -o $@ $<

$(BUILDDIR)/parser.tab.o: $(BUILDDIR)/parser.tab.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/lex.yy.o: $(BUILDDIR)/lex.yy.c
	$(CC) $(CFLAGS) -c $< -o $@

# 运行单个文件
run: build
ifneq ($(FILE),)
	@if [ -f "$(FILE)" ]; then \
		echo "🚀 运行: $(FILE)"; \
		echo "========================================"; \
		./$(TARGET) "$(FILE)"; \
		echo "========================================"; \
	else \
		echo "❌ 文件不存在: $(FILE)"; \
		echo "可用文件:"; \
		ls examples/*.mylang 2>/dev/null || echo "没有找到示例文件"; \
	fi
else
	@echo "❌ 请指定文件: make run FILE=examples/hello.mylang"
	@echo "可用文件:"; \
	ls examples/*.mylang 2>/dev/null || echo "没有找到示例文件"
endif

# 运行所有测试
test: build
	@echo "🧪 运行所有测试..."
	@if [ -d "examples" ]; then \
		for file in examples/*.mylang; do \
			if [ -f "$$file" ]; then \
				echo ""; \
				echo "📁 运行: $$file"; \
				echo "========================================"; \
				./$(TARGET) "$$file"; \
				echo "========================================"; \
				echo ""; \
			fi; \
		done; \
	else \
		echo "ℹ️  示例目录不存在"; \
	fi

# 清理
clean:
	@rm -rf $(BUILDDIR)/
	@echo "🧹 清理完成"

# 交互模式
interactive:
	@chmod +x build.sh
	@./build.sh interactive

# 帮助
help:
	@echo "📖 MyLang 编译器 Makefile 帮助"
	@echo "用法: make [目标]"
	@echo ""
	@echo "目标:"
	@echo "  build        编译项目"
	@echo "  run FILE=... 运行指定文件"
	@echo "  test         运行所有测试"
	@echo "  clean        清理构建文件"
	@echo "  interactive  交互模式"
	@echo "  help         显示帮助"
	@echo ""
	@echo "示例:"
	@echo "  make build"
	@echo "  make run FILE=examples/hello.mylang"
	@echo "  make test"
	@echo "  make interactive"