
macro(spirit_math_benchmark targetName ...)
    spirit_benchmark(spirit-math ${targetName} ${...})
endmacro()

spirit_math_benchmark(matrixMul-benchmark matrixMul.cpp)
spirit_analyse_benchmarks(spirit-math ${CMAKE_CURRENT_SOURCE_DIR}/out)