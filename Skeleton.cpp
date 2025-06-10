/**
***  Skeleton.cpp
***  This program is used to learn how to write the LLVM Pass Framework  
***  author: PengPeng
**/

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

struct SkeletonPass : public PassInfoMixin<SkeletonPass> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        for (Function &F : M) {
            // Here you can implement your pass logic.
            // For example, you can analyze or modify the function F.
            // Example: Print the name of each function in the module
            errs() << "Function: " << F.getName() << "\n";
        }

        // If you modify the module, you should return PreservedAnalyses::none().
        // If you do not modify the module, you can return PreservedAnalyses::all().
        // In this case, we are not modifying the module, so we return all preserved analyses.
        return PreservedAnalyses::all();
    };
};

}

// Register the current pass with LLVM
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
        .APIVersion = LLVM_PLUGIN_API_VERSION,
        .PluginName = "Skeleton pass",
        .PluginVersion = "v0.1",
        .RegisterPassBuilderCallbacks = [](PassBuilder &PB) {
            PB.registerPipelineStartEPCallback(
                [](ModulePassManager &MPM, OptimizationLevel Level) {
                    MPM.addPass(SkeletonPass());
                });
        }
    };
}