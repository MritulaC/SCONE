add_executable(sconecmd sconecmd.cpp)

# Require C++17 standard
set_target_properties(sconecmd PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_include_directories(sconecmd PRIVATE ${CMAKE_SOURCE_DIR}/contrib/tclap-1.2.1/include)

target_link_libraries(sconecmd sconelib)

if (SCONE_OPENSIM_3)
	target_link_libraries(sconecmd sconeopensim3)
	target_compile_definitions(sconecmd PRIVATE SCONE_OPENSIM_3)
endif()

if (SCONE_OPENSIM_4)
	target_link_libraries(sconecmd sconeopensim4)
	target_compile_definitions(sconecmd PRIVATE SCONE_OPENSIM_4)
endif()

if (SCONE_HYFYDY)
	target_link_libraries(sconecmd sconehfd)
	target_compile_definitions(sconecmd PRIVATE SCONE_HYFYDY)
endif()

if (SCONE_LUA)
	target_link_libraries(sconecmd sconelua)
	target_compile_definitions(sconecmd PRIVATE SCONE_LUA)
endif()
