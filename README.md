# test-llvm-msvc-issue-57

Simple [`cmkr`](https://cmkr.build) template for a WDK kernel driver for Windows. The `WDK` package is based on [FindWDK](https://github.com/SergiusTheBest/FindWDK).

## Requirements

- Visual Studio 2022
- [WDK 11](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk)
- [llvm-msvc](https://github.com/NewWorldComingSoon/llvm-msvc-build/releases)
- CMake 3.15+

## Building with llvm-msvc

```
cmake -Bbuild -TLLVM-MSVC_v143 -DCMAKE_WIN64_DRIVER=1
cmake --build build --config Release
```

## Building with msvc

```
cmake -Bbuild -DCMAKE_WIN64_DRIVER=1
cmake --build build --config Release
```

You can open `build\test-llvm-msvc-issue-57.sln` in Visual Studio and work there.
