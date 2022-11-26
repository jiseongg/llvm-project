#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "BBInstructionCounter"

STATISTIC(BBInstructionCounter, "Counts number of functions greeted");

namespace {
  struct BBInstructionCounter : public ModulePass {
    static char ID; // Pass identification, replacement for typeid
    Hello() : FunctionPass(ID) {}

    bool runOnModule(Module &M) override {
      
      for (Function &f : M.getFunctionList()) {
        for (BasicBlock &bb : f.getBasicBlockList()) {
          ++BBInstructionCounter;
          errs() << bb.sizeWithoutDebug();
        }
      }
    }
    return false;
  };
}

char BBInstructionCounter::ID = 0;
static RegisterPass<BBInstructionCounter> X("BBInstructionCounter", "Basic block instruction counter");

