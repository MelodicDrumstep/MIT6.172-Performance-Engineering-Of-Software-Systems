这个 project 是用 cilk 并行框架编写并行程序， 但是很不幸的是， 在我准备写这个 project 的 2024 年， Intel 早就放弃了对 cilk 的维护和支持， 我现在无法编译整个程序， 因此我没有做这个 project， 之后准备找些 openmp 的 project 来写一写。

```shell
clang -std=gnu99 -Wall -ftapir -O3 -DNDEBUG  -o collision_world.o -c collision_world.c
clang: error: unknown argument: '-ftapir'
make: *** [Makefile:91: collision_world.o] Error 1
```