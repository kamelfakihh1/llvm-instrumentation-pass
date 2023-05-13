#include "InstrumentFunction.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Passes/PassBuilder.h"

using namespace llvm;

//-----------------------------------------------------------------------------
// InstrumentFunction implementation
//-----------------------------------------------------------------------------
PreservedAnalyses InstrumentFunction::run(llvm::Module &M,
                                       llvm::ModuleAnalysisManager &) {
  bool Changed =  runOnModule(M);

  return (Changed ? llvm::PreservedAnalyses::none()
                  : llvm::PreservedAnalyses::all());
}

bool InstrumentFunction::runOnModule(llvm::Module &M){

  bool instrumentedOneFunction = false;

  declareFunction(M);

  for (auto &F : M) {
    if (F.isDeclaration()){
      continue;
    }

    instrumentFunction(M, F);
    instrumentedOneFunction = true;
  }

  return instrumentedOneFunction;
}

void InstrumentFunction::instrumentFunction(llvm::Module &M, llvm::Function &F){

  // Get an IR builder. Sets the insertion point to the top of the function
  IRBuilder<> Builder(&*F.getEntryBlock().getFirstInsertionPt());

  // Inject a global variable that contains the function name
  auto FunctionName = Builder.CreateGlobalStringPtr(F.getName());

  // inject call to my function
  Builder.CreateCall(
            InstrumentFunction::myFunction, 
            {FunctionName}
            // {}
  );
}

void InstrumentFunction::declareFunction(llvm::Module &m){
  
  LLVMContext &ctx = m.getContext(); 

  // void type (to specify function return type)
  Type *voidTy = Type::getVoidTy(ctx);              
  // string type (to specify function parameters)
  Type *StringType = Type::getInt8PtrTy(ctx);

  // what is this ???
  bool isVarArg = false;

  // specify function parameters (only one in this case)
  std::vector<Type*> functionCallParams;
  functionCallParams.push_back(StringType);
  FunctionType *functionCallType = FunctionType::get(
                voidTy, functionCallParams, isVarArg
                );

  // insert function to module
  InstrumentFunction::myFunction = m.getOrInsertFunction(InstrumentFunction::FUNCTION_STR, functionCallType);

}

//-----------------------------------------------------------------------------
// PM Registration
//-----------------------------------------------------------------------------
llvm::PassPluginLibraryInfo getInstrumentFunctionPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "instrument-function", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "instrument-function") {
                    MPM.addPass(InstrumentFunction());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getInstrumentFunctionPluginInfo();
}