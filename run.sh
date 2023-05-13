PROOT=$(pwd)
LLVM_DIR="/home/kamel/llvm/llvm-project/build/"

mkdir build
cd build
cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../
cmake --build .
# objdump -t instrumentFunction/libMyFunction.a 

cd ../example
$LLVM_DIR/bin/clang -emit-llvm -S main.cpp -o main.ll
$LLVM_DIR/bin/opt -S -load-pass-plugin $PROOT/build/instrumentFunction/libInstrumentFunction.so --passes=instrument-function main.ll -o instrumented-main.ll
$LLVM_DIR/bin/clang++ instrumented-main.ll -o main -L$PROOT/build/instrumentFunction -lMyFunction -g
./main