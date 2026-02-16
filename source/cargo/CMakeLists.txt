#############################
# THE CARGO-LOADING PROBLEM #
#############################

cmake_minimum_required(VERSION 3.29)
project(CargoLoading VERSION 1.0 LANGUAGES C)

# Build options
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_COMPILER clang)

# add_compile_options(-g) # debug mode
# add_compile_options(-O0)  # turn off optimization

if(CMAKE_SYSTEM_NAME MATCHES "Darwin") # mac
  message("Building on macOS")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
  message("Building on Linux")
else()
  message(FATAL_ERROR "Building on ${CMAKE_SYSTEM_NAME} is not supported")
endif()


###############
# Directories #
###############

set(projectDirectory "${CargoLoading_SOURCE_DIR}")
cmake_path(GET projectDirectory PARENT_PATH sourceDirectory)
message("Source directory: ${sourceDirectory}")

set(commonDirectory "${sourceDirectory}/common")
set(arrayDirectory "${sourceDirectory}/array")
set(matrixDirectory "${sourceDirectory}/matrix")
set(stringsDirectory "${sourceDirectory}/strings")


#############
# Libraries #
#############

add_library(tools
  "${commonDirectory}/tools.c"
)
target_include_directories(tools
  PRIVATE ${commonDirectory}
)

add_library(strings
  "${stringsDirectory}/strings.c"
)
target_include_directories(strings
  PRIVATE ${commonDirectory}
  PRIVATE ${stringsDirectory}
)

add_library(array
  "${arrayDirectory}/array.c"
)
target_include_directories(array
  PRIVATE ${commonDirectory}
  PRIVATE ${arrayDirectory}
  PRIVATE ${stringsDirectory}
)
target_link_libraries(array
  tools
  strings
)

add_library(matrix
  "${matrixDirectory}/matrix.c"
)
target_include_directories(matrix
  PRIVATE ${commonDirectory}
  PRIVATE ${matrixDirectory}
)
target_link_libraries(matrix
  tools
)

add_library(search
  search.c
)
target_include_directories(search
  PRIVATE ${commonDirectory}
  PRIVATE ${projectDirectory}
  PRIVATE ${arrayDirectory}
  PRIVATE ${stringsDirectory}
)
target_link_libraries(search
  tools
  array
  strings
)

add_library(recurrence
  recurrence.c
)
target_include_directories(recurrence
  PRIVATE ${commonDirectory}
  PRIVATE ${projectDirectory}
  PRIVATE ${arrayDirectory}
  PRIVATE ${matrixDirectory}
  PRIVATE ${stringsDirectory}
)
target_link_libraries(recurrence
  tools
  array
  matrix
  search
  strings
)


##############
# Executable #
##############

add_executable(cargo.out
  cargo.c
)
target_include_directories(cargo.out
  PRIVATE ${commonDirectory}
  PRIVATE ${projectDirectory}
  PRIVATE ${arrayDirectory}
  PRIVATE ${matrixDirectory}
  PRIVATE ${stringsDirectory}
)
target_link_libraries(cargo.out
  tools
  array
  matrix
  search
  recurrence
  strings
)
