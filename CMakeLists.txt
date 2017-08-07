project(FragmentDisplay)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR})
find_package(Maya 2017 REQUIRED)

set(PLUGIN_NAME FragmentDisplay)
add_library(${PLUGIN_NAME} SHARED main.cpp)
set_target_properties(${PLUGIN_NAME} PROPERTIES PREFIX "")
if (WIN32)
    set_target_properties(${PLUGIN_NAME} PROPERTIES SUFFIX ".mll")
endif ()
target_link_libraries(${PLUGIN_NAME} Maya::Maya)

install(TARGETS ${PLUGIN_NAME} DESTINATION plug-ins)
install(FILES test_fragment.xml DESTINATION plug-ins)
