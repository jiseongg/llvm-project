#include "llvm/ADT/Statistic.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "BBStats"

STATISTIC(BBStatsCounter, "Counts number of functions greeted");

namespace {
  struct BBStats : public ModulePass {
    static char ID; // Pass identification, replacement for typeid
    BBStats() : ModulePass(ID) {}

    bool runOnModule(Module &M) override {
      
      for (Function &f : M.getFunctionList()) {
        for (BasicBlock &bb : f.getBasicBlockList()) {
          ++BBStatsCounter;
          errs() << bb.sizeWithoutDebug() << '\n';
        }
      }
      return false;
    }
  };
}

char BBStats::ID = 0;
static RegisterPass<BBStats> X("bbstats", "Basic block instruction statistics");

