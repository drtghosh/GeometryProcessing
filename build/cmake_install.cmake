# Install script for directory: C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/GeometryProcessingFramework")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/extern/clean-core/cmake_install.cmake")
  include("C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/extern/typed-geometry/cmake_install.cmake")
  include("C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/extern/polymesh/cmake_install.cmake")
  include("C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/extern/glfw/cmake_install.cmake")
  include("C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/extern/glow/cmake_install.cmake")
  include("C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/extern/imgui/cmake_install.cmake")
  include("C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/extern/glow-extras/cmake_install.cmake")
  include("C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/extern/eigen-lean/cmake_install.cmake")
  include("C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/src/assignment00/cmake_install.cmake")
  include("C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/src/assignment01/cmake_install.cmake")
  include("C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/src/assignment02/cmake_install.cmake")
  include("C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/src/assignment03/cmake_install.cmake")
  include("C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/src/assignment04/cmake_install.cmake")
  include("C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/src/assignment05/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/drtghosh/Documents/All_of_RWTH/Semester2/GeometryProcessingCourse/Exercises/GeometryProcessingFramework/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
