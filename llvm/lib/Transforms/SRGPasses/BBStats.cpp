#include "llvm/ADT/Statistic.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include <map>
#include <vector>

using namespace llvm;

#define DEBUG_TYPE "BBStats"

STATISTIC(BBStatsCounter, "Counts number of functions greeted");

namespace {
  struct BBStats : public ModulePass {
    static char ID;
    BBStats() : ModulePass(ID) {}

    bool runOnModule(Module &M) override {
      ++BBStatsCounter;
      
      std::map<int, int> frequency_map;
      std::vector<int> instruction_counts;
      
      for (Function &f : M.getFunctionList()) {
        for (BasicBlock &bb : f.getBasicBlockList()) {
          instruction_counts.push_back(bb.sizeWithoutDebug());
        }
      }

      for (int instruction_count : instruction_counts) {
        if (frequency_map.find(instruction_count) == frequency_map.end()) {
          frequency_map[instruction_count] = 1;
        } else {
          frequency_map[instruction_count]++;
        }
      }

      for (auto& it : frequency_map) {
        errs() << it.first << ' ' << it.second << '\n';
      }
      return false;
    }
  };
}

char BBStats::ID = 0;
static RegisterPass<BBStats> X("bbstats", "Basic block instruction statistics");

