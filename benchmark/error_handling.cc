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
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <stdexcept>
#include <vector>

namespace {

TEST_CASE( "error reporting mechanisms"
         , "[benchmark][expect<T>][reporting]" )
{
  using success_t =
    std::vector<int>;

  SECTION( "success" )
  {
    auto success =
      []{ return success_t{}; };
    auto nothrow_try_catch =
      [&]{ try { return success(); }
           catch( ... ) {} };

    BENCHMARK( "{baseline}: try-catch : {surely} return success" )
    { return nothrow_try_catch(); };
  }

  SECTION( "fail" )
  {
    auto surely_throw =
      []{ try { throw std::exception{}; }
          catch( ... ) {} };

    BENCHMARK( "{baseline}: try-catch : {surely} throw" )
    { return surely_throw(); };
  }
}

} //< namespace
