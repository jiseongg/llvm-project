#include "llvm/ADT/Statistic.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include <vector>
#include <fstream>

using namespace llvm;

#define DEBUG_TYPE "BBTracer"

STATISTIC(BBTracerCounter, "Counts number of functions greeted");

namespace {

  void insertTracerCall(Module *M, LLVMContext &C, BasicBlock &bb, std::string bb_id, FunctionCallee fc) {
    Instruction *firstInst =  (Instruction*) &(*bb.begin());
    std::vector<Value*> args;
    IRBuilder<> Builder(&bb, bb.begin());
    StringRef str = StringRef(bb_id);
    Constant *BBIdReference = Builder.CreateGlobalStringPtr(str, "", 0, M);
    args.push_back(BBIdReference);
    CallInst::Create(fc, args, "", firstInst); 
  }

  struct BBTracer : public ModulePass {
    static char ID;
    BBTracer() : ModulePass(ID) {}

    bool runOnModule(Module &M) override {
      ++BBTracerCounter;

      std::string module_name =  M.getName().str();

      errs() << "====----- Entered Module " << module_name << '\n';
      LLVMContext &context = M.getContext();

      FunctionCallee fc = M.getOrInsertFunction("_Z7bb_execPv",
          Type::getVoidTy(context), Type::getInt8PtrTy(context));
      
      for (Function &f : M.getFunctionList()) {
        if (f.getName() == "_Z7bb_execPv") {
          continue;
        }

        int i = 0;
        for (BasicBlock &bb : f.getBasicBlockList()) {
          std::string bb_id = f.getName().str() + "_bb_" + std::to_string(i++);
          errs() << bb_id << '\n';
          insertTracerCall(&M, context, bb, bb_id, fc);
        }
      }
      return true;
    }
  };
}

char BBTracer::ID = 0;
static RegisterPass<BBTracer> X("bbtracer", "Dynamically tracing basic block");

