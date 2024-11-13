# Repository example1 - Windows Console

A performance test of a naive std::distance function implementation vs an advanced one using constexpr to avoid branching at runtime. It also compares perfromance against the STL implementation.
 

## Requirements

* Windows 10 x64
* Visual Studio 2022 (download latest version from [Visual Studio Subscriptions page][15])
  * Use `Desktop Development with C++` during installation
* [CMake 3.15.1][2] or newer and make sure it is part of the PATH environment variable
* [Git 2.25.0][1] or newer
* [Git Bash][1] - optional, installed by Git 

## Building

**Clone the repo**

```$ git clone https://github.com/tbruckschlegel/example1.git```


**Setup dependencies and create the project** 

**Windows**

```
$ cd /path/to/repo
$ ./create_vs_solution.bat
```

Open the solution file `./build/example1.sln` in Visual Studio 2022 to build
Debug or Release targets, build artifacts will be available in `./build/Debug`
and/or `./build/Release` directory. 


**Tests**

I use [Gtest][3] for Unit Testing.


[1]: https://git-scm.com/downloads
[2]: https://cmake.org/download/
[3]: ./UNIT_TESTING.md
