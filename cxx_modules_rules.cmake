set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set the experimental UUID
#   Look up uuid at https://github.com/Kitware/CMake/blob/v3.27.0/Help/dev/experimental.rst
#   using the version tag in the top left corner.
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.28")
	set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "bf70d4b0-9fb7-465c-9803-34014e70d112")
elseif(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.27")
	set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
elseif(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.26")
	set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
elseif(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.25")
	set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "3c375311-a3c9-4396-a187-3227ef642046")
else()
	message(FATAL_ERROR "Unsupported CMake version '${CMAKE_VERSION}'. Please upgrade to version 3.28+")
	#set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "9629ab6c-6c0e-423f-bb9d-cc5ac4a22041") # ?
	#set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "17be90bd-a850-44e0-be50-448de847d652") # ?
endif()

# Set up GCC specific stuff
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	string(CONCAT CMAKE_EXPERIMENTAL_CXX_SCANDEP_SOURCE
      "<CMAKE_CXX_COMPILER> <DEFINES> <INCLUDES> <FLAGS> -E -x c++ <SOURCE>"
      " -MT <DYNDEP_FILE> -MD -MF <DEP_FILE>"
      " -fmodules-ts -fdeps-file=<DYNDEP_FILE> -fdeps-target=<OBJECT> -fdeps-format=p1689r5"
      " -o <PREPROCESSED_SOURCE>")
	set(CMAKE_EXPERIMENTAL_CXX_MODULE_MAP_FORMAT "gcc")
	set(CMAKE_EXPERIMENTAL_CXX_MODULE_MAP_FLAG "-fmodules-ts -fmodule-mapper=<MODULE_MAP_FILE> -fdeps-format=p1689r5 -x c++")
endif ()
