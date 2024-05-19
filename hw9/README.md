这个 lab 是研究 ` Deterministic Execution`。

首先来逐行看一下给出的 Makefile:

```makefile

all: hashtable undef user
# all 目标， 依赖于 hashtable, undef, user 这三个可执行文件
# 所以 make / make all 的时候会构建这三个可执行文件

#DBG=-DVERBOSE
CC=clang
# 指定编译器为 clang

CFLAGS=-std=gnu99 -gdwarf-3 -Wall -O1 -fdetach
# 指定编译参数

LDFLAGS += -ldl
# 指定 Linker 参数， 这里表示链接时添加 libdl 库。
# libdl 库是处理动态链接的标准库之一

REPLAY=0


# non-cilk, single-threaded only record replay support with gdb 7.5
ifeq ($(REPLAY),0)
hashtable: CFLAGS += -fcilkplus -DCILK
hashtable: LDFLAGS +=-lcilkrts
hashlock-unit: CFLAGS += -fcilkplus -DCILK
hashlock-unit: LDFLAGS +=-lcilkrts
endif
# 一段条件编译

hashtable: LDFLAGS += -lpthread
hashlock-unit: LDFLAGS += -lpthread
# 对于 hashtable， hashlock-unit 这两个可执行文件， 链接参数加上 `-lpthread` 来支持多线程

hashtable1: ./hashtable
	./hashtable
hashtable10: ./hashtable
	bash -c "for((i=0;i<10;i++)) do ./hashtable; done"
hashtable1000: ./hashtable
	bash -c "for((i=0;i<1000;i++)) do ./hashtable; done"
# 一些执行可执行文件的目标
# 所以它们的依赖都是可执行文件 ./hashtable

# XXX modify this target so that 1000 runs all succeed
hashtable1000-good: ./hashtable
	bash -c "for((i=0;i<1000;i++)) do ./hashtable; done"
# XXX modify this target so the program always fails
hashtable1-bad: ./hashtable
	./hashtable

hashtable-mt: ./hashtable
	./hashtable 10
hashtable-mt1000: ./hashtable
	bash -c "for((i=0;i<1000;i++)) do ./hashtable 10; done"

whoami: ./user
	@./user

clean: 
	@-rm -f ./undef ./hashtable ./hashlock-unit ./user

undef-run: ./undef
	./undef
	./undef

undef-compare: 
	make CFLAGS=-O3 clean undef; ./undef
	make CFLAGS=-O1 clean undef; ./undef

undef-noaslr: ./undef
	setarch x86_64 -R ./undef
	setarch x86_64 -R ./undef
# 特别注意这个: setarch 指令用于设置系统体系结构， 这里设置为 x86_64
# -R 参数表示禁止 ASLR（Address Space Layout Randomization）

undef-env: ./undef
	setarch x86_64 -R env USER=me ./undef
	setarch x86_64 -R env USER=professoramarasinghe ./undef

undef-all: undef-run undef-compare undef-env undef-noaslr 

hashtable: hashtable.c hashlock.c common.h
	$(CC) -o $@ $(DBG) $(CFLAGS) hashtable.c hashlock.c $(LDFLAGS)
# $@ 表示当前目标的名称， 这里会自动替换成 hashtable
# 这样写可以使得 makefile 更通用且易于维护

%: %.c
	$(CC) -o $@ $(DBG) $(CFLAGS) $^ $(LDFLAGS)

hashlock-unit: hashlock.c Makefile
	$(CC) -o $@ -Wall -g -fdetach -DUNIT_TEST $< $(LDFLAGS)

testall: hashtable10 hashlock-unit undef-all

```

因此， 如果我运行 `make undef-compare`, 那么得到的地址是随机的， 但如果运行 `make undef-noaslr`， 那么得到的地址是确定的。

另外吐槽一下， 新版本的 `clang` 已经不支持这个 lab 里面的那些编译参数了， 这个 lab 有点过时了。