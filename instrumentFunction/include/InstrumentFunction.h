#ifndef LLVM_PASS_INSTRUMENTATION_H
#define LLVM_PASS_INSTRUMENTATION_H

#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"

#endif

struct InstrumentFunction : public llvm::PassInfoMixin<InstrumentFunction> {
  
  // name of instrumentation functions
  const char *FUNCTION_STR = "_Z9printNamePKc";  
  llvm::FunctionCallee myFunction;

  llvm::PreservedAnalyses run(llvm::Module &M,
                              llvm::ModuleAnalysisManager &);
  
  bool runOnModule(llvm::Module &M);
  void instrumentFunction(llvm::Module &M, llvm::Function &f);
  void declareFunction(llvm::Module &M);

  // Without isRequired returning true, this pass will be skipped for functions
  // decorated with the optnone LLVM attribute. Note that clang -O0 decorates
  // all functions with optnone.
  static bool isRequired() { return true; }
};
