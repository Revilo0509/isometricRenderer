# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/olive/Documents/GitHub/isometricRenderer/build/_deps/raygui-src")
  file(MAKE_DIRECTORY "C:/Users/olive/Documents/GitHub/isometricRenderer/build/_deps/raygui-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/olive/Documents/GitHub/isometricRenderer/build/_deps/raygui-build"
  "C:/Users/olive/Documents/GitHub/isometricRenderer/build/_deps/raygui-subbuild/raygui-populate-prefix"
  "C:/Users/olive/Documents/GitHub/isometricRenderer/build/_deps/raygui-subbuild/raygui-populate-prefix/tmp"
  "C:/Users/olive/Documents/GitHub/isometricRenderer/build/_deps/raygui-subbuild/raygui-populate-prefix/src/raygui-populate-stamp"
  "C:/Users/olive/Documents/GitHub/isometricRenderer/build/_deps/raygui-subbuild/raygui-populate-prefix/src"
  "C:/Users/olive/Documents/GitHub/isometricRenderer/build/_deps/raygui-subbuild/raygui-populate-prefix/src/raygui-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/olive/Documents/GitHub/isometricRenderer/build/_deps/raygui-subbuild/raygui-populate-prefix/src/raygui-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/olive/Documents/GitHub/isometricRenderer/build/_deps/raygui-subbuild/raygui-populate-prefix/src/raygui-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
