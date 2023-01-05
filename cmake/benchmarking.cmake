macro(spirit_benchmark spiritLib targetName ...)
    add_executable(${targetName} ${...})
    target_link_libraries(${targetName} celero)

    set(benchSrcsVar ${spiritLib}-benchSrcs)
    set(${benchSrcsVar} ${${benchSrcsVar}} ${targetName})
endmacro()

macro(spirit_analyse_benchmarks spiritLib outputDir)
    set(benchSrcsVar ${spiritLib}-benchSrcs)
    # call script on all

    foreach(benchmark ${benchSrcsVar})
        set(csv ${benchmark}.csv)

        add_custom_command(
            OUTPUT ${csv}
            COMMAND ${benchmark} -t ${csv}
            WORKING_DIRECTORY ${outputDir}
        )

        set(html ${benchmark}.html)
        set(htmls ${htmls} ${html})


        set(plotScript ${CMAKE_CURRENT_SOURCE_DIR}/plot.py)
        find_package(Python3 INTERPRETER)
        if (NOT Python3_FOUND)
            message(FATAL_ERROR "Could not find a python3 interpreter")
        
        add_custom_command(
            OUTPUT ${html}
            COMMAND ${Python3_EXECUTABLE} ${plotScript} ${csv} ${html}
            WORKING_DIRECTORY ${outputDir}
        )

    endforeach()
          
    add_custom_target(
        ${spiritLib}-benchmarks ALL
        DEPENDS ${htmls}
    )


    add_custom_target(COMMAND

        COMMAND_EXPAND_LISTS
    )
endmacro()