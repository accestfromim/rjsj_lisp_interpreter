cmake_minimum_required(VERSION 3.21)
project(lisp)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(lisp ./error.cpp ./main.cpp ./parser.cpp ./builtins.cpp ./eval_env.cpp ./files.cpp ./forms.cpp ./token.cpp ./tokenizer.cpp ./value.cpp)
set_target_properties(lisp 
PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
