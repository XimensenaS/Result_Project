# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/User/Desktop/result_project/build/_deps/bs_thread_pool-src"
  "C:/Users/User/Desktop/result_project/build/_deps/bs_thread_pool-build"
  "C:/Users/User/Desktop/result_project/build/_deps/bs_thread_pool-subbuild/bs_thread_pool-populate-prefix"
  "C:/Users/User/Desktop/result_project/build/_deps/bs_thread_pool-subbuild/bs_thread_pool-populate-prefix/tmp"
  "C:/Users/User/Desktop/result_project/build/_deps/bs_thread_pool-subbuild/bs_thread_pool-populate-prefix/src/bs_thread_pool-populate-stamp"
  "C:/Users/User/Desktop/result_project/build/_deps/bs_thread_pool-subbuild/bs_thread_pool-populate-prefix/src"
  "C:/Users/User/Desktop/result_project/build/_deps/bs_thread_pool-subbuild/bs_thread_pool-populate-prefix/src/bs_thread_pool-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/User/Desktop/result_project/build/_deps/bs_thread_pool-subbuild/bs_thread_pool-populate-prefix/src/bs_thread_pool-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/User/Desktop/result_project/build/_deps/bs_thread_pool-subbuild/bs_thread_pool-populate-prefix/src/bs_thread_pool-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
