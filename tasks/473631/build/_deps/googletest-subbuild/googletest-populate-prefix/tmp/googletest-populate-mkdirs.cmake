# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/473631/build/_deps/googletest-src")
  file(MAKE_DIRECTORY "/Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/473631/build/_deps/googletest-src")
endif()
file(MAKE_DIRECTORY
  "/Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/473631/build/_deps/googletest-build"
  "/Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/473631/build/_deps/googletest-subbuild/googletest-populate-prefix"
  "/Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/473631/build/_deps/googletest-subbuild/googletest-populate-prefix/tmp"
  "/Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/473631/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
  "/Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/473631/build/_deps/googletest-subbuild/googletest-populate-prefix/src"
  "/Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/473631/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/473631/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/ittehad/Documents/Coding/Python/Turing/Tester_cpp_codebench/tasks/473631/build/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
