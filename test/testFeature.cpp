#include "SPIRIT/Base.hpp"
#include "catch2/catch_test_macros.hpp"


TEST_CASE("Test module setup")
{
    auto ss = std::make_shared<sp::AnsiStreamSink_mt<std::stringstream>>(false);
    sp::spiritLogger()->sinks().emplace_back(ss);
    ss->set_pattern("%v"); // message only

    std::string msg{"This module seems to be up and running"};
    sp::spiritLog() << sp::Info{"{}{}{}{}", sp::green, sp::bold, msg, sp::reset};

    sp::ansiOut << sp::spiritPattern();

    sp::ansiOut.stream().flush();
    REQUIRE(ss->stream().str() == msg+"\n"); // (loggers add newlines)
}

