# Function instrumentation test

a simple llvm pass the injects a function call to every function inside a module. The injected function prints the calling function name

the project is based on [this](https://github.com/banach-space/llvm-tutor.git) llvm tutorial by [@_banach_space](https://github.com/banach-space).


# usage ?

## setup environment

```
PROOT=$(pwd)
LLVM_DIR=<installation/dir/of/llvm/17>
```

## build pass

```
mkdir build
cd build
cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../
cmake --build .
```

## use pass on the provided example

```
cd $PROOT/example
$LLVM_DIR/bin/clang -emit-llvm -S main.cpp -o main.ll
$LLVM_DIR/bin/opt -S -load-pass-plugin $PROOT/build/instrumentFunction/libInstrumentFunction.so --passes=instrument-function main.ll -o instrumented-main.ll
$LLVM_DIR/bin/clang++ instrumented-main.ll -o main -L$PROOT/build/instrumentFunction -lMyFunction -g
```