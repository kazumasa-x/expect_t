![cover](misc/calligraphy.jpg)

expect\<T>
==========

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![Docker](https://img.shields.io/badge/docker-%230db7ed.svg?style=for-the-badge&logo=docker&logoColor=white)

`expect<T>` project defines {strongly typed} error handling facilities alternative to raising expections
and {weakly typed} return codes.

`expect<T>` is aimed at being more of a research project providing transparent findings on the cost and
semantics on standard error handling mechanisms expressed in Modern C++ -- rather than claiming any would
be always superior to the others.

`C++20`

compiler coverage
-----------------

| compiler | version | image |
| -------- | ------- | ----- |
| `gcc` | `11.4` | `ubuntu:22.04` |
| `clang` | `14.0` | `ubuntu:22.04` |
| `gcc` | `14.1.1` | `archlinux:base-20240101.0.204074` |
| `clang` | `17.0.6` | `archlinux:base-20240101.0.204074` |

examples
--------

```cpp
/* public header including *all* features */
#include <mry/expect.h>
#include <string_view>
#include <cctype>

using std::string_literals::operator"" s;

auto my_atoi( std::string_view s ) noexcept
  -> mry::expect<int>
{
  int parsed =0;

  /* try parsing `s`... */
  for (auto c : s)
    if (! std::isdigit(c))
      return mry::error_t{"non digit char "s + c};

    else
    /* convert `c` to its numeric value and accumulate
     * into `parsed` */

  return parsed /* when no error detected */;
}
```

quickstart
----------

in order to get the full feature set of `expect<T>`

  1. `#include <mry/expect.h>` 
  2. link to the `mry::expect_t` [CMake](https://cmake.org/) target

thats it.

should you want to run the test and benchmark suites associated with
the `expect<T>` project follow the example instructions below.

```sh
$ pushd container && docker buildx build -t builder . && popd
$ mkdir build
$ docker run --rm                         \
             --it                         \
             --volume  "$(pwd):/ci-build" \
             --workdir "/ci-build/build"  \
             --cap-add SYS_PTRACE         \
             builder                      \
             bash                         \
               "-GNinja"                  \
               "-DCMAKE_CXX_COMPILER=g++" \
               "-DCMAKE_BUILD_TYPE=Debug" \
               "-DSanitize="              \
               "-DBuildBenchmarks=OFF"
$ ninja && ctest -VV
```

motivation
----------

aside from education purposes on the forms of various error "reporting" facilities
(beyond the scope of logging) their respective performance landscape was the main
driving factor in experimenting with `expect<T>` and comparing it with raising
`std::exception`s and implementing `expect<T>`-like facility in terms of `std::variant`.

```sh
benchmark name                       samples       iterations    test run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
{baseline}: {surely} return success            100         61398          0 ns
                                       0.514808 ns   0.514381 ns   0.516515 ns
                                     0.00420414 ns 8.3431e-05 ns  0.0100295 ns
                                                                              
{baseline}: try-catch : {surely}
return success                                 100         41444          0 ns
                                       0.771928 ns   0.771281 ns   0.775131 ns
                                     0.00638399 ns 8.64867e-05 ns  0.0152327 ns
                                                                               
{baseline}: std::variant : {surely}
return success                                 100         28915     2.8915 ms
                                        1.16273 ns     1.1361 ns    1.19353 ns
                                       0.146982 ns   0.130118 ns   0.199427 ns
                                                                              
expect<T> : {surely} return success            100         39032          0 ns
                                       0.771901 ns   0.771437 ns    0.77417 ns
                                     0.00454405 ns 0.000110613 ns  0.0108374 ns
```

```sh
benchmark name                       samples       iterations    test run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
{baseline}: try-catch : {surely}
throw                                          100            23     3.2637 ms 
                                        1.55446 us    1.53851 us    1.59188 us 
                                        117.288 ns     37.672 ns    206.781 ns 
                                                                                
{baseline}: variant : {surely}
return fail                                    100         17790      1.779 ms 
                                        1.82064 ns    1.80101 ns    1.86928 ns 
                                        0.13852 ns  0.0133309 ns   0.254908 ns 
                                                                                
expect<T> : {surely} return fail               100         20697     2.0697 ms 
                                        1.56907 ns    1.54743 ns    1.62737 ns 
                                        0.16151 ns  0.0548286 ns   0.332577 ns 

std::variant : 48 [B]
expect<T>    : 48 [B]
```

build
-----

even though `expect<T>` is a header only library the test and benchmark binaries
associated with with the project require [Catch2](https://github.com/catchorg/Catch2).

for convenience sake `expect<T>` exposes a [Dockerfile](https://docs.docker.com/reference/dockerfile/) describing a self-contained
image suitable for building and running the tests and benchmarks associated
with `expect<T>`.

### prerequisites

* [Docker](https://www.docker.com/)

### builder commandline

there is a small set of [CMake](https://cmake.org/) flags which the `builder` expects
or could take in order to configure the build environment.

please see those flags in the table below.

| cmake variable | description | choices | ?optional | default |
| -------------- | ----------- | ------- | --------- | ------- |
| `CMAKE_CXX_COMPILER`| specifies the command to be used as the C++ compiler | `g++`, `clang++` | __required__ | |
| `CMAKE_BUILD_TYPE` | specifies the build type on single-configuration generators | `Debug`, `Release` | __required__ | |
| `Sanitize` | specifies the sanitizer for runtime instrumentation | `address`, `thread` | _optional_ | `<none>` |
| `BuildBenchmarks` | specifies whether to build the benchmark suites | `ON`, `OFF` | _optional_ | `OFF` |

### builder run examples

```sh
# 1. using clang :
#
#   -DCMAKE_CXX_COMPILER="clang++"
#
$ docker run --rm                         \
             --it                         \
             --volume  "$(pwd):/ci-build" \
             --workdir "/ci-build/build"  \
             --cap-add SYS_PTRACE         \
             builder                      \
             bash                         \
               "-GNinja"                  \
               "-DCMAKE_CXX_COMPILER=clang++" \
               "-DCMAKE_BUILD_TYPE=Debug" \
               "-DSanitize="              \
               "-DBuildBenchmarks=OFF"

# 2. enabling address sanitizer
#
#   -DSanitize="address"
#
$ docker run --rm                         \
             --it                         \
             --volume  "$(pwd):/ci-build" \
             --workdir "/ci-build/build"  \
             --cap-add SYS_PTRACE         \
             builder                      \
             bash                         \
               "-GNinja"                  \
               "-DCMAKE_CXX_COMPILER=g++" \
               "-DCMAKE_BUILD_TYPE=Debug" \
               "-DSanitize=address"       \
               "-DBuildBenchmarks=OFF"

# 3. building benchmarks in 'release' build
#
#   -DCMAKE_BUILD_TYPE=Release
#   -DBuildBenchmarks=ON
#
$ docker run --rm                           \
             --it                           \
             --volume  "$(pwd):/ci-build"   \
             --workdir "/ci-build/build"    \
             --cap-add SYS_PTRACE           \
             builder                        \
             bash                           \
               "-GNinja"                    \
               "-DCMAKE_CXX_COMPILER=g++"   \
               "-DCMAKE_BUILD_TYPE=Release" \
               "-DSanitize="                \
               "-DBuildBenchmarks=ON"
```

contact
-------

iszekeres.x@gmail.com
