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

#include <catch2/catch_test_macros.hpp>
#include <string_view>
#include <string>
#include <cctype>

namespace {

/**
 * @brief my_atoi examplar implementation of `std::atoi`
 *        demonstrating the use of `mry::expect<T>`
 */
auto my_atoi( std::string_view s ) noexcept
  -> mry::expect<int>
{
  using std::string_literals::operator"" s;

  auto parsed =
    0;

  auto constexpr numeric =
    []( char code ) noexcept
      { return (code - '0'); };

  for (auto c : s)
    if (! std::isdigit(c))
      return mry::error_t{"non digit char "s + c};

    else
      parsed =
        ( parsed * 10 ) + numeric(c);

  return parsed;
}

TEST_CASE( "expect<T> examples compile", "[expect<T>][examples]" )
{
  using fail_t =
    typename mry::expect<int>::fail_type;
  auto expect_success =
    []( auto expect, auto what )
      {
        REQUIRE( what );
        REQUIRE( expect == what.success() );
      };
  auto expect_fail =
    []( std::string_view message, auto what )
      {
        REQUIRE( what.holds_error() );
        REQUIRE( message == what.fail().get() );
      };

  expect_success( 10, my_atoi("10") );

  expect_fail( "non digit char d"
             , my_atoi( "9d" ) );
}

} //< namespace
