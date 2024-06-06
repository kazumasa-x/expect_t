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
 * @file variant.h defines low-level facilities on holding alternative types
 *       of variants and fundamental operations on such types
 */
#include "mry/meta.h"

#include <type_traits>
#include <algorithm>
#include <memory>

namespace mry::internal {

/**
 * @brief variant_storage defines the layout and alignment of
 *        storage capable of holding any of the types in its
 *        variadic `Types...` pack argument
 *
 * `variant_storage` also provde access to such {uninitialized}
 * storage to its subclasses
 */
template <typename ...Types>
  class variant_storage
{
    static auto constexpr storage_size =
      std::max({ sizeof(Types)... });
    static auto constexpr store_alignment =
      std::max({ alignof(Types)... });

    /**
     * @brief store defines the storage capable of holding
     *        any of the types in the variadic type
     *        argument pack to `variant_storage`
     */
    struct alignas(store_alignment) store final
    {
      char buffer[storage_size];
    };

  public :
    using pointer =
      char*;

    /**
     * @brief default constructs the uninitialized storage
     */
    constexpr variant_storage() noexcept
      =default;

    /**
     * @brief data returns a raw untyped pointer to the
     *        beginning of the storage region
     */
    inline auto data() noexcept
      -> pointer
    { return &store_.buffer[0]; }

  private :
    store store_;
};

/**
 * @brief variant_alternative describes the operations on a
 *        variant alternative
 */
template <typename T>
  struct variant_alternative
{
  using reference =
    std::add_lvalue_reference_t<T>;

  /**
   * @brief default constructs an uninitialized alternative
   */
  constexpr variant_alternative() noexcept
    =default;

  /**
   * @brief in-place move constructs an alternative on the storage
   *        denoted by `storage`
   */
  inline
    variant_alternative( char *storage, T &&o ) noexcept
  { std::construct_at( reinterpret_cast<T*>(storage), std::move(o) ); }

  /**
   * @brief destroys the alternative at its location
   */
  auto destroy( T &o ) noexcept
    -> void
  { std::destroy_at( &o ); }

  /**
   * @brief get returns the reference of the {expected}
   *        alternative type T at the designated `storage` location
   *
   * @note: it is the responsibility of the caller to ensure
   *        `storage` does refer to a storage holding an initialized
   *        instance of type T
   */
  inline
    auto get( char *storage, mry::meta::type_tag<T> ) noexcept
      -> reference
  { return *reinterpret_cast<T*>(storage); }
};

} // namespace mry::internal
