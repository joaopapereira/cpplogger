CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(example_src exampleProgram.cpp) 
SET(example1_src exampleProgram1.cpp)
SET(example2_src exampleProgram2.cpp) 
ADD_EXECUTABLE( exampleProgram  ${example_src})
ADD_EXECUTABLE( exampleProgram1  ${example1_src})
ADD_EXECUTABLE( exampleProgram2  ${example2_src})

TARGET_LINK_LIBRARIES(exampleProgram  JPLoggerStatic )
TARGET_LINK_LIBRARIES(exampleProgram1 pthread JPLoggerStatic )
TARGET_LINK_LIBRARIES(exampleProgram2 pthread JPLoggerStatic )

