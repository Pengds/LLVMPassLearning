# LLVMPassLearning
目前 LLVM 支持不修改编译器后端代码的前提下使用“-fpass-plugin=”编译选项在编译时使用用户自定义的 pass 插件，相对修改编译器后端代码重新编译 LLVM 的方案而言更便捷、简单。如果需要通过编译器传递实现的逻辑不复杂，建议使用该方法。

Skeleton.cpp 是编写 pass 的通用框架，要自定义 pass 的逻辑，只需要按自己的逻辑重写 *run()* 方法。

    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM)();

pass 文件目录下 FirstPass.cpp 是一个在 IR 层面修改程序的示例 pass，你可以按照该文件组织结构定义自己的 pass。

Build:

    $ cd LLVMPassLearning
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ cd ..

Run:

    // linux
    $ clang -fpass-plugin=build/pass/FirstPass.so something.c
    // mac
    $ clang -fpass-plugin=build/pass/FirstPass.dylib something.c

如果你想在大型的 c/c++ 项目编译中加入你自定义的 pass 逻辑，你可以在 CFLAGS 中添加编译选项 -fpass-plugin=/path/to/yourpass.so 确保编译器能够找到自定义的 pass 动态链接库。

注意：请确保编译 pass 动态链接库的 LLVM 与编译 c/c++ 文件的 LLVM 相同，不同会报错。
