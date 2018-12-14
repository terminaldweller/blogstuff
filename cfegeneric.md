# A Clang Tool
There was a time when, if you wanted to write a tool to make your life easier while writing C/C++, you couldn't.<br/>
I mean, sure, you could, but if was going to be a long, big project. In other words not everyone would do it.<br/>
Those days don't exist any more thanks to LLVM and Clang.<br/>
For this tutorial we will use the libtooling clang library to write a calng tool. A clang tool is a standalone executable, unlike a clang plugin.<br/>
Like previously stated, we will be using the libtooling library so we will write our code in C++.<br/>
Before we start our tutorial, let's talk about libtooling and libclang.<br/>
The first obvious difference is the language of course. libtooling is C++, libclang is C.<br/>
The second difference is that libclang's API is stable while libtooling's API is ever-changing. libtooling does not guarantee any sort of backwards-compatibility whatsoever.<br/>
The advantage that libtooling has is that evrything is exposed under its API. Under libclang you get a lot of things but it's not everything.<br/>
You can get a better description [here](https://clang.llvm.org/docs/Tooling.html).<br/>
All that being said, since we are brave(~~masochistic~~), we will be using libtooling to write our awesome, awesome clang tool.<br/>
Also since we actually care about open-source software and the importance of early testers(~~i.e. masochistic~~) we will use a dev version of clang/llvm, which at the time of writing this would be 8.0.<br/>
I'll try to cover as many pain points as I encountered in both the text here and the comments in the source code which you can find [here](https://github.com/bloodstalker/blogstuff/tree/master/src/clang-tool).<br/>
Also for our awesome tool, we will use ASTmatchers and not visitors.<br/>
If you are writing a tool that uses ASTmathcers, it's a really good idea to use a tool from `clang extra tools` called `clang-query`. `calng_query` is a clang tool itself that let's you type in ASTmatchers in a REPL-like environment so you won't have to write a matcher, recompile your tool and look at the results and make changes ad nauseam. Also, while clang-query is awesome, there have been some instances in previous versions(rare I might add), where libtooling and clang-query built from the same source would require you to write slightly different matchers to get the same result so keep that in mind.<br/>

If you already know why you are here, skip this part, otherwise let's talk a little bit about LLVM/Clang.<br/>

FIXME -- maybe just turn this into a cgrep tutorial, plus some pch
