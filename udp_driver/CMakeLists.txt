# Copyright 2018 Apex.AI, Inc.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Co-developed by Tier IV, Inc. and Apex.AI, Inc.
# Maintained by LeoDrive, 2021

cmake_minimum_required(VERSION 3.5)

project(udp_driver)

# Consider passing `-DCMAKE_BUILD_TYPE=Debug` option to cmake to export Debug symbols.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

## dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_auto REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)
find_package(io_context REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif ()

include_directories(include)

ament_export_include_directories(include)
ament_auto_find_build_dependencies()
ament_auto_add_library(${PROJECT_NAME} SHARED
        src/udp_socket.cpp
        src/udp_driver.cpp
        examples/udp_driver_node.cpp)
ament_target_dependencies(${PROJECT_NAME} "rclcpp" "io_context" "std_msgs")

target_compile_options(${PROJECT_NAME} PUBLIC "-O0" PRIVATE -Wall)

if (Boost_FOUND)
  ament_target_dependencies(${PROJECT_NAME} "Boost")
  target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif ()

install(
        DIRECTORY include
        DESTINATION include
)

if (BUILD_TESTING)
    find_package(ament_cmake_gtest REQUIRED)
    find_package(ament_lint_auto REQUIRED)

    ament_lint_auto_find_test_dependencies()

    set(TEST_FILES
            test_udp_socket.cpp
            test_udp_data.cpp
            test_udp_driver.cpp
            test_udp_driver_node.cpp)

    foreach(file ${TEST_FILES})
        get_filename_component(name "${file}" NAME_WE)
        set(TEST_UDP_DRIVER_EXE ${name})

        ament_add_gtest(${TEST_UDP_DRIVER_EXE}
                test/gtest_main.cpp
                test/${name}.cpp)
        ament_target_dependencies(${TEST_UDP_DRIVER_EXE} rclcpp std_msgs Boost)

        target_include_directories(${TEST_UDP_DRIVER_EXE} PRIVATE include)
        target_link_libraries(${TEST_UDP_DRIVER_EXE})
    endforeach()
    find_package(ament_lint_auto REQUIRED)
    ament_lint_auto_find_test_dependencies()
endif()
# TODO(flynneva): add some IF statement here to only build this if needed
# IF(blah)
find_package(udp_msgs REQUIRED)

add_library(udp_component SHARED
  src/udp_component.cpp
)

target_compile_definitions(udp_component
  PRIVATE "COMPOSITION_BUILDING_DLL")

ament_target_dependencies(udp_component
  "rclcpp"
  "rclcpp_components"
  "udp_msgs"
  "io_context"
  "Boost"
)

target_include_directories(udp_component PRIVATE include)
target_link_libraries(udp_component ${Boost_LIBRARIES})

rclcpp_components_register_node(udp_component
  PLUGIN "udp_component::UdpComponent"
  EXECUTABLE "udp_node"
)

install(TARGETS
  udp_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  DIRECTORY config/
  DESTINATION config
)

ament_export_include_directories(include)

ament_package()
