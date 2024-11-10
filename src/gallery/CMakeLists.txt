
add_library(gallery STATIC gallery.cpp animation.cpp)


target_include_directories(gallery PRIVATE 
                    ${wxWidgets_INCLUDE_DIRS} 
                    ${CMAKE_SOURCE_DIR}/src/common 
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    )



