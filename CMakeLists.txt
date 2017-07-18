cmake_minimum_required(VERSION 2.8)
project(FSM)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Build/)
set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/Sources/fsm.cpp ${PROJECT_SOURCE_DIR}/Sources/main.cpp)
set(EXECUTABLE FSM)

find_package(OpenCV REQUIRED)
set(PROJECT_INCLUDE_DIRS ${OpenCV_INCLUDE_DIRS})
set(PROJECT_LIBRARIES ${OpenCV_LIBS})

include_directories(${PROJECT_INCLUDE_DIRS})
add_executable(${EXECUTABLE} ${SOURCE_FILES})
target_link_libraries(${EXECUTABLE} ${PROJECT_LIBRARIES})

