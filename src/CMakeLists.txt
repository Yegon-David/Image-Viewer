
add_executable(${PROJECT_NAME} WIN32 runner.cpp frame.cpp)

add_subdirectory(gallery)

target_include_directories(${PROJECT_NAME} PRIVATE 
                        ${wxWidgets_INCLUDE_DIRS} 
                        ${CMAKE_CURRENT_SOURCE_DIR}/common 
                        ${CMAKE_CURRENT_SOURCE_DIR}/gallery
                        )


target_precompile_headers(gallery PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/common/pch.hpp)

target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/common/pch.hpp)

