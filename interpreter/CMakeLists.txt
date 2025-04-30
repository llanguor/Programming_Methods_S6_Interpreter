file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        src/*.cpp
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

if(SOURCES)
    add_library(interpreter ${SOURCES} ${HEADERS})
else()
    add_library(interpreter ${HEADERS})
    set_target_properties(interpreter PROPERTIES LINKER_LANGUAGE CXX)
endif()

target_include_directories(interpreter PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(interpreter PUBLIC trie)
target_link_libraries(interpreter PUBLIC lexer)