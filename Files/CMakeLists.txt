cmake_minimum_required(VERSION 3.24)
project(Text_Summarizer)

include_directories(Files)

add_executable(Text_Summarizer
        Constants.h
        DocumentParser.cpp
        DocumentParser.h
        main.cpp
        TfIdf.cpp
        TfIdf.h TextRank.h TextRank.cpp SparseVector.h SparseVector.cpp)


# Optional: If you're using specific compiler flags, keep this
target_compile_options(Text_Summarizer PRIVATE -mavx -mfma)
