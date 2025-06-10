/**
***  FirstPass
***  This pass example is how to insert a call to a trampoline function 
***  before the function call.  
***  author: PengPeng
**/

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

namespace {

struct FirstPass : public PassInfoMixin<FirstPass> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        uint64_t callInsID = 1;

        // Declare the trampoline function
        LLVMContext &Ctx = M.getContext();
        std::vector<Type*> paramTypes = {Type::getInt64Ty(Ctx)};
        Type *retType = Type::getVoidTy(Ctx);
        FunctionType *FT = FunctionType::get(retType, paramTypes, false);
        FunctionCallee trampolineFunc = M.getOrInsertFunction("trampoline", FT);

        for (Function &F : M) {
            if (F.isDeclaration() || F.getName() == "trampoline") {
                continue;
            }

            for (BasicBlock &BB : F) {
                insertBeforeCallInst(M, BB, trampolineFunc, callInsID);
            }
        }

        return PreservedAnalyses::none();
    }

    void insertBeforeCallInst(Module &M, BasicBlock &BB, FunctionCallee &trampolineFunc, uint64_t &callInsID) {
        for (Instruction &I : BB) {
            if (auto *call = dyn_cast<CallInst>(&I)) {
                Function *Callee = call->getCalledFunction();
                if (Callee && Callee->getName() != "trampoline") {
                    IRBuilder<> builder(call);
                    callInsID++;
                    Constant *param = 
                        ConstantInt::get(Type::getInt64Ty(M.getContext()), callInsID, false);
                    std::vector<Value*> args = {param};
                    builder.CreateCall(trampolineFunc, args);
                }
            }
        }
    }
};

}

// Register the Firstpass with LLVM
extern "C" LLVM_ATTRIBUTE_WEAK::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
        .APIVersion = LLVM_PLUGIN_API_VERSION,
        .PluginName = "FirstPass",
        .PluginVersion = "v1.0",
        .RegisterPassBuilderCallbacks = [](PassBuilder &PB) {
            PB.registerPipelineStartEPCallback(
                [](ModulePassManager &MPM, OptimizationLevel Level) {
                    MPM.addPass(FirstPass());
                }
            );
        }
    };
}