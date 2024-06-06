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
#include "mry/error_t.h"

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <stdexcept>
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
catch( ... ) {}


TEST_CASE( "error reporting mechanisms"
         , "[benchmark][expect<T>][reporting]" )
{
  using success_t =
    std::vector<int>;

  SECTION( "success : return <void>" )
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

  SECTION( "fail : return <void>" )
  {
    auto surely_throw =
      []{ try { throw std::exception{}; }
          catch(...) {} };
    auto error =
      []() noexcept -> mry::error_t
        { return mry::error_t{"e"}; };

    BENCHMARK( "{baseline}: try-catch : {surely} throw" )
    { return surely_throw(); };

    BENCHMARK( "error_t : return error" )
    { return error(); };
  }
}

} //< namespace
