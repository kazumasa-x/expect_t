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

#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <string>

namespace {

TEST_CASE( "error_t semantics", "[expect<T>][error_t]" )
{
  using std::string_literals::operator"" s;

  SECTION( "empty : no error condition met" )
  {
    auto error =
      mry::error_t{};

    REQUIRE( !error );
    REQUIRE( !error.holds_error() );
  }

  SECTION( "error condition met" )
  {
    auto expect =
      "e"s;
    auto error =
      mry::error_t{ expect };

    REQUIRE( error );
    REQUIRE( error.holds_error() );
    REQUIRE( expect == error.get() );
  }

  SECTION( "return" )
  {
    auto success =
      []() noexcept -> mry::error_t
        { return {}; };
    auto expect =
      "e"s;
    auto fail =
      [expect]() noexcept -> mry::error_t 
        { return mry::error_t{expect}; };

    REQUIRE( !success().holds_error() );
    REQUIRE( !success() );

    REQUIRE( fail().holds_error() );
    REQUIRE( fail() );
    REQUIRE( expect == fail().get() );
  }
}

TEST_CASE( "expect<T> semantics", "[expect<T>][error_t]" )
{
  using success_t =
    std::int32_t;
  using expect_t =
    mry::expect< success_t >;
  using fail_t =
    typename expect_t::fail_type;

  SECTION( "success" )
  {
    auto expect =
      success_t{7};
    auto success =
      expect_t{ expect };

    REQUIRE( success );
    REQUIRE( !success.holds_error() );
    REQUIRE( expect == success.success() );
  }

  SECTION( "fail" )
  {
    auto expect =
      fail_t{"error"};
    auto fail =
      expect_t{ expect };

    REQUIRE( !fail );
    REQUIRE( fail.holds_error() );
    REQUIRE( expect.get()
         == fail.fail().get() );
  }
}

} // namespace
