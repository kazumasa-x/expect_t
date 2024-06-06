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
#pragma once
/**
 * @file expect.h defines {strongly typed} facilities for describing
 *       expected results or any error conditions occurring during the
 *       computation of such results
 *
 * #see: `mry::expect<T>`
 */
#include "mry/expect/variant.h"
#include "mry/error_t.h"

namespace mry {

/**
 * @brief expect<T> is a {strongly typed} variant of the expected result
 *        type T and an unexpected potential error condition met
 *        during the computation of such result type T
 *
 * @note: expect<T> is a {strongly typed} alternative to raising exceptions
 *        in a context when an operation would have a valid result otherwise
 */
template <typename T>
  class expect final
    : public internal::variant_storage< T, error_t >
    , public internal::variant_alternative<T>
    , public internal::variant_alternative<error_t>
{
    using storage_alternative_type =
      typename expect::variant_storage;
    using success_alternative_type =
      internal::variant_alternative<T>;
    using fail_alternative_type =
      internal::variant_alternative<error_t>;

    using storage_alternative_type::data;
    using success_alternative_type::get;
    using success_alternative_type::destroy;
    using fail_alternative_type::get;
    using fail_alternative_type::destroy;

  public :
    using success_type =
      T;
    using fail_type =
      error_t;

    /**
     * @brief constructs success case
     */
    expect( success_type s ) noexcept
      : success_alternative_type{ data(), std::move(s) }
    {}

    /**
     * @brief constructs fail case
     */
    expect( fail_type f ) noexcept
      : fail_alternative_type{ data(), std::move(f) }
      , holds_error_{ true }
    {}

    /**
     * @brief success returns the expected result type T
     *
     * @note: it is the responsibility of the caller to ensure
     *        the `expect<T>` instance holds the expected
     *        type T
     */
    inline
      auto success() noexcept
        -> decltype(auto)
    { return get( data(), mry::meta::type<success_type> ); }

    /**
     * @brief fail returns the description of the error condition
     *        occurred
     *
     * @note: it is the responsibility of the caller to ensure
     *        the `expect<T>` instance holds the description of
     *        the error description
     */
    inline
      auto fail() noexcept
        -> decltype(auto)
    { return get( data(), mry::meta::type<fail_type> ); }

    /**
     * @brief holds_error predicate tests whether the
     *        instance denotes an error condition met
     *        and holds a description of such condition
     *
     * @see: `mry::error_t::holds_error()`
     */
    constexpr
      auto holds_error() const noexcept
        -> bool
    { return holds_error_; }

    /**
     * @brief {explicit} bool conversion operator is a
     *        convenience layer over `holds_error()`
     *
     * @see: `holds_error()`
     */
    constexpr
      explicit operator bool() const noexcept
    { return !holds_error(); }

    /**
     * @brief destroys the appropriate alternative type being
     *        held
     *
     * @see: `destroy()`
     */
    ~expect() noexcept
    { destroy(); }

  private :
    /**
     * @brief destroy invokes the destructor the appropriate alternative
     *        type being held
     */
    auto destroy() noexcept
      -> void
    {
      if (! holds_error_)
        destroy( success() );
      else
        destroy( fail() );
    }
    bool holds_error_ =false;
};

} // namespace mry
