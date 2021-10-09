#include <boost/test/unit_test.hpp>
#define EXPECT_EQ BOOST_CHECK_EQUAL

#include <cucumber-cpp/autodetect.hpp>

using cucumber::ScenarioScope;

struct tuple {
    double x, y, z, w;
};

struct tuple_ctx {
    std::vector<tuple> tuples;
    std::vector<std::string> names;
};

GIVEN("^a ← tuple\\(4.3, -4.2, 3.1, 1.0\\)$") {
    ScenarioScope<tuple_ctx> ctx;
    ctx->tuples.push_back({4.3, -4.2, 3.1, 1.0});
    ctx->names.push_back({"a"});
}

WHEN("test") {

}

THEN("^a.x = 4.3$") {
    ScenarioScope<tuple_ctx> ctx;
    EXPECT_EQ(ctx->tuples[0].x, 4.3);
}