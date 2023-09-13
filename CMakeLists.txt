cmake_minimum_required(VERSION 3.10)
project(Recorder)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(
  record_reader
  main.cpp
  recorder.cpp
  FileRecorder.cpp
  FileReader.cpp
  reader.cpp
)