// Copyright (c) 2024 Imre Szekeres <iszekeres.x@gmail.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include "mry/expect.h"
#include "mry/error_t.h"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <stdexcept>
#include <iostream>
#include <variant>
#include <vector>

namespace {

/**
 * @brief plain_success describes a pure operation runtime
 *        where no errors occure
 */
auto plain_success()
  -> void
{}

/**
 * @brief success_try_catch describes an operation runtime
 *        in a try-catch context where no errors occure
 */
auto success_try_catch()
  -> void
try { return ; }
catch( ... ) { return; }


TEST_CASE( "benchmark error_t", "[benchmark][error_t]" )
{
  using success_t =
    std::vector<int>;

  SECTION( "success" )
  {
    /* @note: binding to function pointers is strictly used
     *        to circumvent compiler optimization
     *        although resulting benchmarks show mostly
     *        the overhead implied by applying this construct */
    auto success =
      ::plain_success;
    auto success_try_catch =
      ::success_try_catch;
    auto no_error =
      []() noexcept -> mry::error_t
        { return {}; };

    BENCHMARK( "{baseline}: {surely} return" )
    { return success(); };

    BENCHMARK( "{baseline}: {surely} return : try-catch" )
    { return success_try_catch(); };

    BENCHMARK( "error_t : {surely} return no error" )
    { return no_error(); };
  }

  SECTION( "fail" )
  {
    auto surely_throw =
      []{ try { throw std::exception{}; }
          catch(...) { return; } };
    auto error =
      []() noexcept -> mry::error_t
        { return mry::error_t{"e"}; };

    BENCHMARK( "{baseline}: try-catch : {surely} throw" )
    { return surely_throw(); };

    BENCHMARK( "error_t : return error" )
    { return error(); };
  }
}

TEST_CASE( "benchmark expect<T>", "[benchmark][expect<T>]" )
{
  using success_t =
    std::vector<int>;
  using variant_t =
    std::variant< success_t
                , mry::error_t >;
  using expect_t =
    mry::expect<success_t>;

  SECTION( "success" )
  {
    auto success =
      []{ return success_t{}; };
    auto nothrow_try_catch =
      [&]() -> success_t
         { try { return success(); }
           catch( ... ) { return success_t{}; } };
    auto variant_success =
      [&]() noexcept -> variant_t
         { return success_t{}; };
    auto expect_success =
      [&]() noexcept -> expect_t
         { return success_t{}; };

    BENCHMARK( "{baseline}: {surely} return success" )
    { return success(); };

    BENCHMARK( "{baseline}: try-catch : {surely} return success" )
    { return nothrow_try_catch(); };

    BENCHMARK( "{baseline}: std::variant : {surely} return success" )
    { return variant_success(); };

    BENCHMARK( "expect<T> : {surely} return success" )
    { return variant_success(); };
  }

  SECTION( "fail" )
  {
    auto surely_throw =
      []() -> success_t
        { try { throw std::exception{}; }
          catch( ... ) { return success_t{}; } };
    auto variant_fail =
      [&]() noexcept -> variant_t
         { return mry::error_t{"e"}; };
    auto expect_fail =
      [&]() noexcept -> expect_t
         { return mry::error_t{"e"}; };

    BENCHMARK( "{baseline}: try-catch : {surely} throw" )
    { return surely_throw(); };

    BENCHMARK( "{baseline}: variant : {surely} return fail" )
    { return variant_fail(); };

    BENCHMARK( "expect<T> : {surely} return fail" )
    { return expect_fail(); };
  }

  std::cout << "std::variant : " << sizeof(variant_t) << " [B]\n"
            << "expect<T>    : " << sizeof(expect_t)  << " [B]\n"
            << std::endl;
}

} //< namespace
