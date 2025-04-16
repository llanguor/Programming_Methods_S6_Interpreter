file(GLOB_RECURSE INTERPRETER_SOURCES CONFIGURE_DEPENDS
        src/*.cpp
)

file(GLOB_RECURSE INTERPRETER_HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
)

add_library(interpreter ${INTERPRETER_SOURCES} ${INTERPRETER_HEADERS})

target_include_directories(interpreter PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)