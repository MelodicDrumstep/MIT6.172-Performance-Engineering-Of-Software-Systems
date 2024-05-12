这个 hw 的主要内容是基于 CSI 编写一个通过编译时插入信息来分析程序性能的工具。

### CSI 是什么

CSI 是 Tapir / LLVM 编译器支持的一个框架， 可以协助开发者编写动态的性能分析工具。 CSI 将编译细节通过框架隐藏了起来， 提供了一系列可以在编译时插入可执行文件的函数 (此处被称为 "hook")。

工具的编写者会利用 CSI 编写 hook, 之后编译后的工具和待分析程序链接起来， 产生一个 tool-instrumented executable(TIX)。 TIX 执行时， 原先的程序正常运行， 每当 hook 被调用的时候， 会额外进行后台计算。 

### CSI 的特性

CSI 有四大特性： hooks, flat and compact CSI ID spaces, forensic tables, properties. 下面逐条解释：

#### Flat CSI ID Spaces

CSI API 是基于 LLVM IR 做的。 对于每个 IR object, CSI 都会为它分配一个 CSI ID 用以唯一标识。 因此运行时， 传给 hooks 的也是这个 CSI ID。 CSI 分配 ID 的策略是 "flat and compact", 即 ID 可存入数组（而不是必须存入哈希表）， 这种策略让 CSI ID 高效且易于维护（尤其是多线程环境下）。 

#### Hooks

CSI 有两种 hook 类型： initialization hooks 和 IR-object hooks.

##### Initialization hooks

这种 hook 允许我们的工具在 main 函数被调用的那一瞬间执行一些任务。

##### IR-object hooks

这种 hook 允许我们的工具在一个 IR object 被执行之前 / 之后执行一些任务。

##### Forensic tables

Forensic table 可以存储编译器产生的静态信息。比如， 我可以存源代码的位置， 或者某个 block 内的 LLVM IR 指令个数等等。



##### Properties
 
对于每个 IR object, CSI 都计算对应的一个编译时常量 —— property， 包含了编译器分析出的一些信息。 比如， 对于 memory operation, 这个信息就包括地址是否在栈上， 是否有一些对齐信息等。 CSI 框架用这些 property 来优化插入过程。

比如这个例子， 就使用了 property 来优化插入过程：

```c
 void __csi_before_load(const csi_id_t load_id, const void *addr,  const int32_t num_bytes, const load_prop_t prop) 
 { 
    if (prop.is_const || prop.is_not_shared || prop.is_read_before_write_in_bb) 
    {
        return; 
    }
    check_race_on_load(addr, num_bytes);
}
```

#### CSI 程序示例


来看看这个例子，这是利用 CSI 的 API 编写的一个分析代码执行的覆盖情况的工具：

```c
#include <stdio.h> 
#include <stdlib.h> 
#include <csi.h> 

long *block_executed = NULL; 
csi_id_t num_basic_blocks = 0; 

void report() 
{ 
    csi_id_t num_basic_blocks_executed = 0; 
    fprintf(stderr, "CSI-cov report:\n"); 
    for (csi_id_t i = 0; i < num_basic_blocks; i++) 
    {   //遍历所有 Block
        if (block_executed[i] > 0) 
        {
            num_basic_blocks_executed++; 
            //记录是否被执行了
        }
        const source_loc_t *source_loc = __csi_get_bb_source_loc(i); //从 forensic table 中拿到 source location
        if (NULL != source_loc) 
        {
            fprintf(stderr, "%s:%d:%d executed %d times\n", 
            source_loc->filename, source_loc->line_number, 
            source_loc->column_number, block_executed[i]); 
            //报告输出
        }
    } 
    fprintf(stderr, "Total: %ld of %ld basic blocks executed\n", 
    num_basic_blocks_executed, num_basic_blocks); 
    free(block_executed); 
} 

//这个 Hook 会在 main 函数调用之前执行
void __csi_init() 
{ 
    atexit(report); //注册 report 函数， 它会在被测试程序结束之后执行， 输出报告
} 

//这个 hook 在每个 unit 被 load 的时候执行
void __csi_unit_init(const char * const name, const instrumentation_counts_t counts) 
{   
    block_executed = (long *)realloc(block_executed, (num_basic_blocks + counts.num_bb) * sizeof(long)); 
    //给 block_executed 数组分配新空间
    memset(block_executed + num_basic_blocks, 0, counts.num_bb * sizeof(long)); 
    num_basic_blocks += counts.num_bb; 
} 

// 这个 Hook 在每个 basic block 被执行的时候执行
void __csi_bb_entry(const csi_id_t bb_id, const bb_prop_t prop) 
{ 
    block_executed[bb_id]++; 
} 
```