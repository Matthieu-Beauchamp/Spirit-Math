enable_testing()

macro(spirit_module_add_test testName ...)
    spirit_add_test(spirit-module ${testName} ${...})
endmacro()

spirit_module_add_test(feature-test testFeature.cpp)

# adds spirit-base-test
spirit_test_all(spirit-module)

