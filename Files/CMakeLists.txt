cmake_minimum_required(VERSION 3.24)
project(Text_Summarizer)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories (optional if needed for other header files)
include_directories(Files)

# Add executable and its source files
add_executable(Text_Summarizer
    Constants.h
    DocumentParser.cpp
    DocumentParser.h
    main.cpp
    TfIdf.cpp
    TfIdf.h
    TextRank.h
    TextRank.cpp
    SparseVector.h
    SparseVector.cpp
)
