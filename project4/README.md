这是这门课的最后一个也是最难的一个 project,　大致是优化一个下棋的 AI 算法。

## 任务

按照任务书所说, 

```
This project simulates the situations you will encounter in real life. After designing and 
implementing a fairly sizable program, you find that it doesn’t run fast enough. At this point, 
you have to figure out what is slow, and decide how to fix the performance issues. We’ve left in 
some low-hanging fruit, but you’ll find the need to make design changes to the game engine for 
substantial performance improvement.
```

大致的优化方向有两点：

+ 优化搜索算法，想出更好的启发函数。 当前的算法为 PVS (Principal Variation Search) 算法。

+ 进行性能调优 (比如剪枝和一堆调优技巧)， 来扩大搜索效率。

任务书提示说， 原始算法的启发函数已经够好了， 所以最好关注第二点优化。

下面来看看完成本次 project 需要具备的必要知识:

## Alpha-Beta Search

我记得在 UCB CS188 学过这部分内容， 所以对我来说还算简单。

关键是这个 Game Tree:

<img src="https://notes.sjtu.edu.cn/uploads/upload_8bf424c4bd32ca4063ecc755592845ff.png" width="300">

该树会枚举所有可能的对局情况， 剪枝则是避免搜索不可能成为结果的对局情况。 

## Leiserchess 规则

这个还挺复杂的， 推荐看 Lecture 19 + 20 的讲解， 写的时候再结合给出的规则文档。

## 编译 & 运行 & 测试

对于非并行的优化阶段， 采取 `make PARALLEL=0` 进行编译。对于并行优化阶段， 采取 `make PARALLEL=1` 进行编译。

## 文档

`./README.txt`: 总文档， 介绍了如何启动 `game server` 以及如何测试。

`player/README.txt`: 介绍了所有可能修改的文件。

`doc/Leiserchess.pdf`： 介绍了游戏规则。

`doc/engine-interface.txt`： 介绍了可以发送给游戏的命令行指令。

## 项目还挺大的， 如何下手？

这里官方文档给了一些建议， 实际上这也是性能调优经常遇到的问题: 如何抓住首要优化点。



