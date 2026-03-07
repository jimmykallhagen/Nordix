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

### Performance flags with security impact
- -fstack-protector
- -D_FORTIFY_SOURCE=1

**Read the guide below, so you can decide for yourself what you think is right for you.**
**Personally, I go all in.**

## LD-Flags
- -flto=full
- -fuse-ld=lld
- -Wl,-O3,--relax
- -Wl,--lto-O3
- -Wl,--lto-whole-program-visibility

**I will probably replace lto full with lto thin, as I suspect lto thin will be more robust.**<br>
**The idea is to create PGO profiles in the future.**

### Security vs Performance trade-off

| Level | Flags | Pipeline cost |
|-------|-------|---------------|
| Maximum performance | `-fno-stack-protector -D_FORTIFY_SOURCE=0` | 0 extra instructions |
| Minimal security | `-fstack-protector -D_FORTIFY_SOURCE=1` | ~1 check per risky function |
| Balanced | `-fstack-protector-strong -D_FORTIFY_SOURCE=2` | ~1 check per function with arrays |
| Maximum security | `-fstack-protector-all -D_FORTIFY_SOURCE=3` | Check on every function call |

Choose based on your threat model and performance requirements.
