enable_testing()

macro(spirit_math_add_test testName ...)
    spirit_add_test(spirit-math ${testName} ${...})
endmacro()

spirit_math_add_test(Transform-test testTransform.cpp)

# adds spirit-base-test
spirit_test_all(spirit-math)

