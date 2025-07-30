cmake_minimum_required(VERSION 3.5)

project(model_tests)

add_compile_options( -Wall -Werror -Wextra -pedantic -g -O0 --coverage )
add_link_options( --coverage )

###########################################################
#                         tests                           #
###########################################################

enable_testing()

set( BIN_NAME modelTests )
add_executable( ${BIN_NAME} tests/tests.cc )

set( GTEST_LIBRARIES gtest gtest_main gmock gmock_main )
target_link_libraries( ${BIN_NAME} ${GTEST_LIBRARIES} )

set( LIB_NAME _testing_model )
add_library( ${LIB_NAME} STATIC model.cc model.h lib/functions.h )
target_link_libraries( ${BIN_NAME} ${LIB_NAME} )

ADD_CUSTOM_TARGET(tests_${BIN_NAME}

    COMMAND ${BIN_NAME}
    COMMAND rm ./CMakeFiles/${LIB_NAME}.dir/*.gcda
    # COMMAND rm ./CMakeFiles/modelTests.dir/modelTests_autogen/*.gcda
    COMMAND rm ./CMakeFiles/modelTests.dir/tests/*.gcda

)

###########################################################
#                        coverage                         #
###########################################################

# Get the OS name using uname
execute_process(
    COMMAND uname -s
    OUTPUT_VARIABLE UNAME_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(IGNORE_ERROR "")
set(INCONSISTENT "")
set(CATEGOTY "")

if(UNAME_OUTPUT STREQUAL "Darwin")
    set(IGNORE_ERROR "--ignore-errors")
    set(INCONSISTENT "inconsistent")
    set(CATEGOTY "category")
endif()

FIND_PROGRAM(LCOV_PATH lcov)
FIND_PROGRAM(GENHTML_PATH genhtml)

set(_outputname coverage_${BIN_NAME}_dir)

ADD_CUSTOM_TARGET(coverage_${BIN_NAME}

    COMMAND ${BIN_NAME}
    COMMAND ${LCOV_PATH} --gcov-tool llvm-cov --gcov-tool gcov --directory ./CMakeFiles/${LIB_NAME}.dir --capture --output-file ${_outputname}.info --rc branch_coverage=1 ${IGNORE_ERROR} ${INCONSISTENT}
    COMMAND ${LCOV_PATH} --extract ${_outputname}.info '*model*' --output-file ${_outputname}.info.cleaned
    COMMAND ${GENHTML_PATH} -o ${_outputname} ${_outputname}.info.cleaned ${IGNORE_ERROR} ${CATEGOTY}
    COMMAND rm ./CMakeFiles/${LIB_NAME}.dir/*.gcda
    # COMMAND rm ./CMakeFiles/modelTests.dir/modelTests_autogen/*.gcda
    COMMAND rm ./CMakeFiles/modelTests.dir/tests/*.gcda

)