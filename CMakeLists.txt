cmake_minimum_required(VERSION 2.8)
project( Kbot )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( main main.cpp )
target_link_libraries( main ${OpenCV_LIBS} )
SET(TARGET_H
    a_star.h    
   )

# Local source files here
SET(TARGET_SRC
    a_star.cpp
    )
