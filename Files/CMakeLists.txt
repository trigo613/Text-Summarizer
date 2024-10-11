cmake_minimum_required(VERSION 3.24)
project(Text_Summarizer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx -mfma")

include_directories(.)

add_executable(Text_Summarizer
        Constants.h
        DocumentParser.cpp
        DocumentParser.h
        main.cpp
        TfIdf.cpp
        TfIdf.h TextRank.h TextRank.cpp SparseVector.h SparseVector.cpp)

target_compile_options(Text_Summarizer PRIVATE -mavx -mfma)

