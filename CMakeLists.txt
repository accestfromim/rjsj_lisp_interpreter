cmake_minimum_required(VERSION 3.21)
project(lisp)

add_executable(lisp ./main.cpp ./parser.cpp ./builtins.cpp ./eval_env.cpp ./files.cpp ./forms.cpp ./token.cpp ./tokenizer.cpp ./value.cpp)
set_target_properties(lisp 
PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#target_include_directories(hw7 PRIVATE includes)
#target_link_directories(hw7 PRIVATE lib)
#target_link_libraries(hw7 PRIVATE rjsjai)
#target_link_libraries(hw7 PRIVATE pthread)

if(MINGW)
#target_link_libraries(hw7 PRIVATE )
#target_link_libraries(hw7 PRIVATE ws2_32)
endif()