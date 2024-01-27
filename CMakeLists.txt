cmake_minimum_required(VERSION 3.27.1)
project ("Schizophasia")
add_executable (${PROJECT_NAME} "main.c" "main.h" "miscpayloads.c" "miscpayloads.h" "graphicalpayloads.c" "graphicalpayloads.h" "audiopayloads.c" "audiopayloads.h" "arrays.c" "arrays.h")
target_link_libraries(${PROJECT_NAME} winmm psapi shlwapi gdi32)
set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
target_compile_options(${PROJECT_NAME} PUBLIC -municode -std=c99 -O2 -mfpmath=sse)

# i don't have vs installed, will probably add stuff for msvc later (cba to install it)
# rn just compile with mingw under some enviroment such as msys2, cygwin(if you're on windows) or under linux lol
