#include "SPIRIT/Transform/Transform.hpp"
#include "catch2/catch_test_macros.hpp"


TEST_CASE("Transformations")
{
    SECTION("Construction")
    {
        sp::Transform2D t{};
        REQUIRE(t.toMatrix() == sp::Mat3::Identity());

        sp::Transform2D rot = sp::Transform2D{}.rotate(sp::radians(45.f));
        t                   = rot;
        REQUIRE(t.toMatrix() == rot.toMatrix());
        REQUIRE(sp::Transform2D{rot}.toMatrix() == rot.toMatrix());

        sp::Mat2 rot2x2 = rot.linear();
        REQUIRE(sp::Transform2D{rot2x2}.toMatrix() == rot.toMatrix());

        t.translate({1, 2});
        t = rot2x2;
        REQUIRE(t.toMatrix() == rot.toMatrix()); // removes translation
    }

    SECTION("Order of operation")
    {
        sp::Transform2D t{};
        t.translate({1, 0}).rotate(sp::radians(90.f));

        sp::Vec2 p{0, 0};
        REQUIRE(t * p == sp::Vec2{0, 1});

        t = sp::Transform2D{}.rotate(sp::radians(90.f)).translate({1, 0});
        REQUIRE(t * p == sp::Vec2{1, 0});

        t = sp::Transform2D{}.translate({1, 0});
        t.transform(sp::Transform2D{}.rotate(sp::radians(90.f)));
        REQUIRE(t * p == sp::Vec2{0, 1});
    }

    SECTION("Inverse")
    {
        sp::Transform3D t{};
        t.translate({1, 2, 3})
            .rotate(sp::radians(30.f), sp::Vec3{1, 1, 1})
            .scale(5)
            .translate({3, 2, 1});

        sp::Vec3 p{5, 6, 4};
        REQUIRE(t * t.inversed() * p == p);
        REQUIRE(t.inversed() * t * p == p);

        // REQUIRE(t * t.inversed() == t.inversed() * t); // TODO: Fails..? (not even approx)

        sp::Transform3D inv = t.inversed().inversed();
        REQUIRE(t.isApprox(inv) == true);

        inv = t.inversed();
        t.inverse();
        REQUIRE(inv == t);
    }
}
