# A Clang Tool
For this tutorial we will use the libtooling clang library to write a calng tool. A clang tool is a standalone executable, unlike a clang plugin.<br/>
Like previously stated, we will be using the libtooling library so we will write our code in C++.<br/>
Before we start our tutorial, let's talk about libtooling and libclang.<br/>
The first obvious difference is the language of course. libtooling is C++, libclang is C.<br/>
The second difference is that libclang's API is stable while libtooling's API is ever-changing. libtooling does not guarantee any sort of backwards-compatibility whatsoever.<br/>
The advantage that libtooling has is that evrything is exposed under its API. Under libclang you get a lot of things but it's not everything.<br/>
You can get a better description [here](https://clang.llvm.org/docs/Tooling.html).<br/>
All that being said, since we are brave(~~masochistic~~), we will be using libtooling to write our awesome, awesome clang tool.<br/>
Also since we actually care about open-source software and the importance of early testers(~~masochistic~~) we will use a dev version of clang/llvm, which at the time of writing this would be 8.0.<br/>
