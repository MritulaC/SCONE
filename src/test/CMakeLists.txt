set(FILES
    ExampleObjective.cpp
    ExampleObjective.h
    main.cpp
    Test.cpp
    Test.h
    )

add_executable(scone-test ${FILES})
set_target_properties(scone-test PROPERTIES
    PROJECT_LABEL "test"
    OUTPUT_NAME test)

target_link_libraries(scone-test sconelib)
