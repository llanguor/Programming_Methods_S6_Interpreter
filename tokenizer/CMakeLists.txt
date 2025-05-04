file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        src/*.cpp
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
        include/*.hpp
        include/*.h
        include/*.tpp
)

if(SOURCES)
    add_library(tokenizer ${SOURCES} ${HEADERS})
else()
    add_library(tokenizer ${HEADERS})
    set_target_properties(tokenizer PROPERTIES LINKER_LANGUAGE CXX)
endif()


target_include_directories(tokenizer PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)