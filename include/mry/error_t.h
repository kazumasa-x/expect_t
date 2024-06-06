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
 * @file error_t.h defines {strongly typed} facilities for signaling error
 *       conditions
 *       
 * @see: `mry::error_t`
 */
#include <optional>
#include <string>

namespace mry {

/**
 * @brief error_t describes a {strongly typed} error condition that
 *        may occure during runtime
 *
 * @note: error_t is a {strongly typed} alternative to raising exceptions
 *        in a context when an operation would only have side-effects
 *        and not yield a result otherwise
 */
class error_t final
{
  public :
    /**
     * @brief constructs "empty" state denoting no error
     *        condition met during runtime -- "holds" no errors
     */
    constexpr error_t() noexcept
      =default;

    /**
     * @brief constructs an instance denoting an error condition
     *        met along with the description of such condition
     *
     */
    explicit
      error_t( std::string e ) noexcept
        : err_{ std::move(e) }
    {}

    /**
     * @brief holds_error predicate tests whether the
     *        instance denotes an error condition met
     *        and holds a description of such condition
     */
    inline
      auto holds_error() const noexcept
        -> bool
    { return err_.has_value(); }

    /**
     * @brief {explicit} operator bool is a convenience layer
     *        over `holds_error`
     *
     * @see: `holds_error()`
     */
    inline explicit
      operator bool() const noexcept
    { return holds_error(); }

    /**
     * @brief get returns the {weakly typed} description of the error condition
     *
     * @note: it is the responsibility of the caller to
     *        ensure the `error_t` instance calling `get()`
     *        on does hold an error
     *
     * @see: `operator bool()`
     * @see: `holds_error()`
     */
    inline auto get() noexcept
      -> decltype(auto)
    { return *err_; }

  private :
    std::optional<std::string> err_ ={};
};

} // namespace mry
