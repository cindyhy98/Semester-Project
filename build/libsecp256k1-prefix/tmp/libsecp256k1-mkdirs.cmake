# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/secp256k1"
  "/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/libsecp256k1-prefix/src/libsecp256k1-build"
  "/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/libsecp256k1-prefix"
  "/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/libsecp256k1-prefix/tmp"
  "/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/libsecp256k1-prefix/src/libsecp256k1-stamp"
  "/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/libsecp256k1-prefix/src"
  "/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/libsecp256k1-prefix/src/libsecp256k1-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/yangyuhsin/Documents/22_Fall/Semester_Project/Accountable-Confirmer/build/libsecp256k1-prefix/src/libsecp256k1-stamp/${subDir}")
endforeach()
