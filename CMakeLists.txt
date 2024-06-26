# copyright (c) 2024 imre szekeres <iszekeres.x@gmail.com>
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
cmake_minimum_required( VERSION 3.22 )

project(      expect<T>
  VERSION     2024.0
  LANGUAGES   CXX
  DESCRIPTION "defines strongly typed alternatives to standardish error handling mechanisms AND comparisions to such alternatives -- benchmarks and examples included" )

set( CMAKE_CXX_STANDARD 20 )

enable_testing()

include( cmake/options.cmake )
include( cmake/sanitize.cmake )

add_subdirectory( lib )
add_subdirectory( test )
add_subdirectory( benchmark )
