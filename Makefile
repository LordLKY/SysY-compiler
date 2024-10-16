EXEC     = SysYVM              # 项目名
SCANNER  = scanner.l                  # *.l
PARSER   = parser.y                   # *.y
CC       = g++                        # 编译器
# C_SRC    = $(wildcard *.c)
CPP_SRC  = $(wildcard *.cpp ./opt/*.cpp)
OBJ      = $(CPP_SRC) parser.tab.c scanner.yy.c

# INPUT    = ./test/... .sy              # 测试程序

# output: $(INPUT) $(EXEC)
# 	$(EXEC) $<

$(EXEC): $(OBJ)
	$(CC) -w $^ -o $@

scanner.yy.c: $(SCANNER) parser.tab.c
	win_flex --wincompat --outfile=$@ $<

parser.tab.c: $(PARSER)
	win_bison -d $<
