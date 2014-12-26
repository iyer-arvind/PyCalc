BISON_TARGET(LanguageParser Language.y ${CMAKE_CURRENT_BINARY_DIR}/LanguageParser.cpp)

FLEX_TARGET(LanguageScanner Language.lex  ${CMAKE_CURRENT_BINARY_DIR}/LanguageLexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(LanguageScanner LanguageParser)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
ADD_LIBRARY(Calc SHARED
  ${BISON_LanguageParser_OUTPUTS}
  ${FLEX_LanguageScanner_OUTPUTS}
  LanguageStructures.cpp
  Dimension.cpp
)
ADD_EXECUTABLE(CalcTest CalcTest.cpp)
TARGET_LINK_LIBRARIES(CalcTest Calc)

