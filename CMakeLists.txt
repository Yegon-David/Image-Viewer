cmake_minimum_required(VERSION 3.30.0)

project(viewer VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(wxWidgets REQUIRED COMPONENTS core base)



include(${wxWidgets_USE_FILE})

set(wxWidgets_LIBRARIES_DEBUG "")
set(wxWidgets_LIBRARIES_RELEASE "")

foreach(lib ${wxWidgets_LIBRARIES})
    if(${lib} STREQUAL "debug")
        set(current_mode "DEBUG")
    elseif(${lib} STREQUAL "optimized")
        set(current_mode "RELEASE")
    elseif(current_mode STREQUAL "DEBUG")
        list(APPEND wxWidgets_LIBRARIES_DEBUG ${lib})
    elseif(current_mode STREQUAL "RELEASE")
        list(APPEND wxWidgets_LIBRARIES_RELEASE ${lib})
    endif()
endforeach()


add_subdirectory(src)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Configuring for debug mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    add_definitions(-DDEBUG)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")    
    message(STATUS "Configuring for release mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -02")
    add_definitions(-DDEBUG)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
endif()

set(LIBD ${wxWidgets_LIBRARIES_DEBUG} gallery)
set(LIBN ${wxWidgets_LIBRARIES_RELEASE} gallery)


target_link_libraries(${PROJECT_NAME} 
                    debug ${LIBD}
                    optimized ${LIBN}
                    )
target_link_libraries(gallery 
                    debug ${wxWidgets_LIBRARIES_DEBUG}
                    optimized ${wxWidgets_LIBRARIES_RELEASE}
                    )
