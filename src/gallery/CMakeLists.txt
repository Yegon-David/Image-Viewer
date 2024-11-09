
add_library(gallery STATIC gallery.cpp animation.cpp)

target_link_libraries(gallery PRIVATE ${wxWidgets_LIBRARIES})

target_include_directories(gallery PRIVATE 
                    ${wxWidgets_INCLUDE_DIRS} 
                    ${CMAKE_SOURCE_DIR}/src/common 
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    )



