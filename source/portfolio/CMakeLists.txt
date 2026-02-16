###############################################
# OPTIMAL TRADING OF GEOMETRIC MEAN-REVERSION #
###############################################

cmake_minimum_required(VERSION 3.29)
project(Portfolio VERSION 1.0 LANGUAGES C)

# Build settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED On)
set(CMAKE_C_COMPILER clang)

# add_compile_options(-g) # debug mode
# add_compile_options(-O0)  # turn off optimization

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
  add_link_options(-lm)  # standard math library
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Darwin") # mac
  message("Building on macOS")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
  message("Building on Linux")
else()
  message("Error: Building on ${CMAKE_SYSTEM_NAME} is not supported")
endif()


###############
# Directories #
###############

set(projectDirectory "${Portfolio_SOURCE_DIR}")
cmake_path(GET projectDirectory PARENT_PATH sourceDirectory)
message("Source directory: ${sourceDirectory}")

cmake_path(GET sourceDirectory PARENT_PATH aboveSourceDirectory)
file(WRITE basePath.h "#ifndef BASE_PATH_H\n")
file(APPEND basePath.h "#define BASE_PATH_H\n\n")
file(APPEND basePath.h "char* basePathLiteral = \"${aboveSourceDirectory}/files/examples/\";\n\n")
file(APPEND basePath.h "#endif /* BASE_PATH_H */")

set(commonDirectory "${sourceDirectory}/common")
set(arrayDirectory "${sourceDirectory}/array")
set(stringsDirectory "${sourceDirectory}/strings")
set(matrixDirectory "${sourceDirectory}/matrix")
set(volumeDirectory "${sourceDirectory}/volume")
set(csvDirectory "${sourceDirectory}/csv")
set(utilityFunctionsDirectory "${sourceDirectory}/utilityFunctions")
set(differentiationDirectory "${sourceDirectory}/differentiation")


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

add_library(strings
  "${stringsDirectory}/strings.c"
)
target_include_directories(strings
  PRIVATE ${stringsDirectory}
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

add_library(volume
  "${volumeDirectory}/volume.c"
)
target_include_directories(volume
  PRIVATE ${commonDirectory}
  PRIVATE ${volumeDirectory}
)
target_link_libraries(volume
  tools
)

add_library(csv
  "${csvDirectory}/csv.c"
)
target_include_directories(csv
  PRIVATE ${commonDirectory}
  PRIVATE ${arrayDirectory}
  PRIVATE ${stringsDirectory}
)
  
add_library(utilityFunctions
  "${utilityFunctionsDirectory}/utilityFunctions.c"
)
target_include_directories(utilityFunctions
  PRIVATE ${commonDirectory}
  PRIVATE ${utilityFunctionsDirectory}
)

add_library(differentiation
  "${differentiationDirectory}/differentiation.c"
)
target_include_directories(differentiation
  PRIVATE ${commonDirectory}
  PRIVATE ${differentiationDirectory}
  PRIVATE ${arrayDirectory}
  PRIVATE ${stringsDirectory}
  PRIVATE ${matrixDirectory}
)

add_library(kolmogorov
  "kolmogorov.c"
)
target_include_directories(kolmogorov
  PRIVATE ${commonDirectory}
  PRIVATE ${arrayDirectory}
  PRIVATE ${matrixDirectory}
  PRIVATE ${stringsDirectory}
  PRIVATE ${utilityFunctionsDirectory}
  PRIVATE ${differentiationDirectory}
)
target_link_libraries(kolmogorov
  differentiation
)

add_library(recurrence
  "recurrence.c"
)
target_include_directories(recurrence
  PRIVATE ${commonDirectory}
  PRIVATE ${arrayDirectory}
  PRIVATE ${stringsDirectory}
  PRIVATE ${matrixDirectory}
  PRIVATE ${volumeDirectory}
  PRIVATE ${utilityFunctionsDirectory}
  PRIVATE ${differentiationDirectory}
)
target_link_libraries(recurrence
  kolmogorov
)

add_library(output
  "output.c"
)
target_include_directories(output
  PRIVATE ${commonDirectory}
  PRIVATE ${arrayDirectory}
  PRIVATE ${stringsDirectory}
  PRIVATE ${matrixDirectory}
  PRIVATE ${volumeDirectory}
  PRIVATE ${utilityFunctionsDirectory}
)


##############
# Executable #
##############

add_executable(portfolio.out
  portfolio.c
)
target_include_directories(portfolio.out
  PRIVATE ${commonDirectory}
  PRIVATE ${arrayDirectory}
  PRIVATE ${matrixDirectory}
  PRIVATE ${volumeDirectory}
  PRIVATE ${stringsDirectory}
  PRIVATE ${csvDirectory}
  PRIVATE ${utilityFunctionsDirectory}
  PRIVATE ${differentiationDirectory}
)
target_link_libraries(portfolio.out
  array
  matrix
  volume
  csv
  utilityFunctions
  recurrence
  differentiation
  output
)
