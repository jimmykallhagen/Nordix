# Nordix znver4 optimizations

## Compiler
- CC=clang
- CXX=clang++

## C-Flags
- -march=znver4
- -mtune=znver4
- -O3
- -pipe
- -fno-plt
- -mavx512f
- -mavx512dq
- -mavx512bw
- -mavx512vl
- -mavx512cd
- -mavx512vbmi
- -mavx512vbmi2
- -mavx512vnni
- -mvaes
- -mvpclmulqdq
- -flto=full
- -fvectorize
- -fslp-vectorize
- -ftree-vectorize
- -funroll-loops
- -falign-functions=64
- -falign-loops=64
- -fno-math-errno
- -fno-trapping-math
- -fmerge-all-constants
- -fno-stack-protector
- -D_FORTIFY_SOURCE=0

## LD-Flags
- -flto=full
- -fuse-ld=lld
- -Wl,-O3,--relax
- -Wl,--lto-O3
- -Wl,--lto-whole-program-visibility"
  
**I will probably replace lto full with lto thin, as I suspect lto thin will be more robust**
**The idea is to also create PGO profiles in the future.**

 
