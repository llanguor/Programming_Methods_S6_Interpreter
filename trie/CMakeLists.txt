file(GLOB_RECURSE TRIE_SOURCES CONFIGURE_DEPENDS
        src/*.cpp
)

file(GLOB_RECURSE TRIE_HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
)

add_library(trie ${TRIE_SOURCES} ${TRIE_HEADERS})

target_include_directories(trie PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)