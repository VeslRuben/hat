SET(ModuleName Filter)
SET(ExeName ${ModuleName}-Example)

set(LOCAL_FILES
    include/Filter.h
    include/MultiChannelFilter.h
    src/Filter.cpp
    src/MultiChannelFilter.cpp
)

set(LOCAL_FILES_LONG
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Filter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Filter.cpp
)


list(APPEND INSTALL_TARGETS ${ModuleName})
list(APPEND INSTALL_FILES ${LOCAL_FILES_LONG})

SET(INSTALL_TARGETS ${INSTALL_TARGETS} PARENT_SCOPE)
SET(INSTALL_FILES ${INSTALL_FILES} PARENT_SCOPE)

add_library(${ModuleName} ${LOCAL_FILES})
target_include_directories(
    ${ModuleName} PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_options(${ModuleName} PRIVATE -Werror -Wall -Wextra)

add_executable(${ExeName} examples/Example.cpp)
target_link_libraries(${ExeName} ${ModuleName})
target_compile_options(${ExeName} PRIVATE -Werror -Wall -Wextra)