cmake_minimum_required (VERSION 3.20)

project(ut-units DESCRIPTION "Compile Time Units Library")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE source_list "${CMAKE_CURRENT_SOURCE_DIR}/test/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp" )
file(GLOB_RECURSE headers_list "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp" )

add_library( ut-units INTERFACE )
target_include_directories( ut-units INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include" )

set(UT_UNITS_TEST OFF CACHE BOOL "Build Unit Tests")

if ( UT_UNITS_TEST )
    MESSAGE(INFO " Building Unit Tests")
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        88abf9bf325c798c33f54f6b9220ef885b267f4f # v3.12.0
    )

    FetchContent_MakeAvailable(Catch2)

    #add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/ext/Catch2")
    add_executable(ut-units-test ${source_list})
    set_property(TARGET ut-units-test PROPERTY CXX_STANDARD 23)
    target_link_libraries(ut-units-test PRIVATE Catch2::Catch2WithMain ut-units)
endif()

set(UT_UNITS_DOCS OFF CACHE BOOL "Build Docs")

if ( UT_UNITS_DOCS )
    file(GLOB_RECURSE site_sources "${CMAKE_CURRENT_SOURCE_DIR}/doc/*.md")
    LIST(APPEND site_sources "${CMAKE_CURRENT_SOURCE_DIR}/mkdocs.yml")

    find_program(MKDOCS mkdocs DOC "manual generator")

    set(site_output
        "${CMAKE_CURRENT_SOURCE_DIR}/mods/manual/index.html"
    )

    find_program(MKDOCS_COMMAND mkdocs)
    add_custom_command(
        OUTPUT ${site_output}
        COMMAND ${MKDOCS_COMMAND} build
            --clean
            --site-dir "${CMAKE_CURRENT_SOURCE_DIR}/site"
        DEPENDS ${site_sources}
        COMMENT "Building mkdocs..."
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    add_custom_target(build_docs DEPENDS ${site_output})
endif()


