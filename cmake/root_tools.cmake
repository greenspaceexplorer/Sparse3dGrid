# Tool to encapsulate some root dictionary building - making linkdefs, etc.
#
## Accepts the following:
#   * TARGET_NAME => will be the generated source file name
#   * CLASS_LIST => a cmake list of all classes needing dictionaries
#   * HEADER_LIST => a cmake list of all the headers that these classes are in.
#                    this allows you to have multiple dictionary classes in some headers
#   * DEPS        => dependencies to add to
#
#  fixme: it doesn't know how to inherit include_directories from libraries that the dictionaries depend on.
#
# https://sft.its.cern.ch/jira/browse/ROOT-8575
#
#
MESSAGE("-- Including ROOT Dictionary Builder")
function(root_dictionary_builder)
    cmake_parse_arguments(
            INT                            # prefix that gets added to variables
            ""                             # options
            "TARGET_NAME"                  # keywords that accept only 1 value
            "CLASS_LIST;HEADER_LIST;DEPS"  # keywords that accept multiple values.
            ${ARGN})

    set(INT_LINKDEF_NAME  ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${INT_TARGET_NAME}_LinkDef.h)
    set(INT_LINKDEF_MAKER ${CMAKE_CURRENT_SOURCE_DIR}/utils/LinkDefMakerMulti.py)

    # build space-delimited version
    list(JOIN INT_CLASS_LIST " " INT_CLASS_LIST_SP)

    #message(STATUS "LinkDef Maker : ${INT_LINKDEF_MAKER}")
    #message(STATUS "LinkDef Name  : ${INT_LINKDEF_NAME}")
    #message(STATUS "Class List    : ${INT_CLASS_LIST}")
    #message(STATUS "Class List SP : ${INT_CLASS_LIST_SP}")
    #message(STATUS "Headers       : ${INT_HEADER_LIST}")
    #message(STATUS "Target Name   : ${INT_TARGET_NAME}")
    #message(STATUS "Dependencies  : ${INT_DEPS}")

    message("${BYellow}-- Building ROOT Dictionaries --${ColReset}")
    message("   => Output file location will be: ${BCyan}${INT_TARGET_NAME}.cxx${ColReset}")
    #message("   => ROOT Dictionaries needed for: ${BRed}${INT_CLASS_LIST_SP}${ColReset}")

    # Pretty version - Report the dictionaries, splitting up list
    set(index 0)
    #list(SORT INT_CLASS_LIST COMPARE STRING)
    list(LENGTH INT_CLASS_LIST count)
    set(outline "   => ROOT Dictionaries needed for: ${BRed}")
    while(index LESS count)
        list(GET INT_CLASS_LIST ${index} item)
        string(APPEND outline "${item} ")    #<==== items added to output here
        math(EXPR ready "(${index}+1) % 4")
        if(ready EQUAL 0)
            message("${outline}${ColReset}")
            set(outline "                                    ${BRed}")
        endif(ready EQUAL 0)
        math(EXPR index "${index}+1")
    endwhile(index LESS count)
    message("${outline}${ColReset}")
    message("")

    # Runs the custom LinkDefMaker - this is added as a dependency of ROOT_GENERATE_DICTIONARY
    # INT_CLASS_LIST_SP should be a SPACE-DELIMITED list of class names
    # using bash -c here because COMMAND is so picky... can never get it to behave
    add_custom_command(
            OUTPUT ${INT_LINKDEF_NAME}
            COMMAND bash -c "${INT_LINKDEF_MAKER} ${INT_LINKDEF_NAME} ${INT_CLASS_LIST_SP}"
            COMMENT "Building LinkDef for ${INT_CLASS_LIST_SP}")

    # Actually do the generation - a single dictionary for all the appropriate classes
    root_generate_dictionary(${INT_TARGET_NAME} ${INT_HEADER_LIST} LINKDEF ${INT_LINKDEF_NAME} DEPENDENCIES ${INT_LINKDEF_NAME} ${INT_DEPS})

endfunction(root_dictionary_builder)
