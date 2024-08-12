**1:**
```
$ export LLVM_DIR=<path to llvm-project/build>
$ cd llvm-counting-instructions-pass/build
$ cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR
$ make
```
**2:**
```
$ cd llvm-project
$ ./build/bin/clang-20 -O0 -S -emit-llvm ./llvm-counting-instructions-pass/tests/input.c -o input.ll
$ ./build/bin/opt -load-pass-plugin ./llvm-counting-instructions-pass/build/libcounting-instructions-pass.so -passes=counting-instructions-pass -disable-output input.ll
```
