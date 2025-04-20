file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        src/*.cpp
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

#If .cpp exists then add with library. Else add only headers. Also set linker language
if(SOURCES)
    add_library(trie ${SOURCES} ${HEADERS})
else()
    add_library(trie ${HEADERS})
    set_target_properties(trie PROPERTIES LINKER_LANGUAGE CXX)
endif()


target_include_directories(trie PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)