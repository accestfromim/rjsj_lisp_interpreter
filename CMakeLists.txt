cmake_minimum_required(VERSION 3.21)
project(lisp)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(lisp src/error.cpp src/main.cpp src/parser.cpp src/builtins.cpp src/eval_env.cpp src/files.cpp src/forms.cpp src/token.cpp src/tokenizer.cpp src/value.cpp)
set_target_properties(lisp 
PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
