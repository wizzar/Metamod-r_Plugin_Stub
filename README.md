# StubPlugin

This is intended to be a starting point for new Metamod-r plugins.<br/>
This stub is already integrated with ReHLDS (3.14) and ReGame (5.27) APIs.

The SDK files are the minimum needed to compile the stub.<br/>
I'm still in the processes of writing how all of these pieces fit together, but you can already find very helpful comments in the code. Notice, however, that this files are not exactly organized: they exact origin is mostly unkown.<br/>
The files come from the original **HLSDK** (https://github.com/ValveSoftware/halflife/), **ReHLDS** (https://github.com/rehlds/ReHLDS/), **ReGameDLL_CS** (https://github.com/rehlds/ReGameDLL_CS), and **ReAPI** (https://github.com/rehlds/ReAPI/).<br/>
However, they have been largely modified over time, and all of these projects share some of the same files, such as `dlls/basemonster.h`, but they differ largely, which makes it difficult to organize without knowing how all of these pieces work.
