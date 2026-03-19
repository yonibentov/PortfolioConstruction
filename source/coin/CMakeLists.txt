#########################
# BETTING ON COIN FLIPS #
#########################

cmake_minimum_required(VERSION 3.29)
project(Coin VERSION 1.1 LANGUAGES C)

# Build settings
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

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  add_link_options(-lm)  # standard math library
endif()


###############
# Directories #
###############

set(projectDirectory "${Coin_SOURCE_DIR}")
cmake_path(GET projectDirectory PARENT_PATH sourceDirectory)
message("Source directory: ${sourceDirectory}")

cmake_path(GET sourceDirectory PARENT_PATH aboveSourceDirectory)
file(WRITE basePath.h "#ifndef BASE_PATH_H\n")
file(APPEND basePath.h "#define BASE_PATH_H\n\n")
file(APPEND basePath.h "char* basePathLiteral = \"${aboveSourceDirectory}/files/coin/\";\n\n")
file(APPEND basePath.h "#endif /* BASE_PATH_H */")

set(commonDirectory "${sourceDirectory}/common")
set(arrayDirectory "${sourceDirectory}/array")
set(matrixDirectory "${sourceDirectory}/matrix")
set(stringsDirectory "${sourceDirectory}/strings")
set(csvDirectory "${sourceDirectory}/csv")


#############
# Libraries #
#############

add_library(tools
  "${commonDirectory}/tools.c"
)
target_include_directories(tools
  PRIVATE ${commonDirectory}
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
)
target_link_libraries(matrix
  tools
)

add_library(strings
  "${stringsDirectory}/strings.c"
)
target_include_directories(strings
  PRIVATE ${commonDirectory}
  PRIVATE ${stringsDirectory}
)

add_library(csv
  "${csvDirectory}/csv.c"
)
target_include_directories(csv
  PRIVATE ${commonDirectory}
  PRIVATE ${csvDirectory}
  PRIVATE ${arrayDirectory}
  PRIVATE ${stringsDirectory}
  PRIVATE ${matrixDirectory}
)

add_library(recurrence
  "recurrence.c"
)
target_include_directories(recurrence
  PRIVATE ${commonDirectory}
  PRIVATE ${arrayDirectory}
  PRIVATE ${stringsDirectory}
  PRIVATE ${matrixDirectory}
)
target_link_libraries(recurrence
  tools
)


##############
# Executable #
##############

add_executable(coin.out
  coin.c
)
target_include_directories(coin.out
  PRIVATE ${commonDirectory}
  PRIVATE ${arrayDirectory}
  PRIVATE ${matrixDirectory}
  PRIVATE ${stringsDirectory}
  PRIVATE ${csvDirectory}
)
target_link_libraries(coin.out
  array
  matrix
  strings
  csv
  recurrence
)
