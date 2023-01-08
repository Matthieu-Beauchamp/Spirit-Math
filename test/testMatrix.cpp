#include "SPIRIT/Math.hpp"

#include "catch2/catch_test_macros.hpp"

TEST_CASE("Matrix")
{
    SECTION("Construction and conversions")
    {
        sp::RowVector<float, 2> row{1, 2};
        sp::Vec2 col{2, 1};

        float dot = (row * col)[0];
        REQUIRE(dot == col.dot(row));
        REQUIRE(dot == 4);

        col = row.transposed();
        REQUIRE(col == row.transposed());
        col = {1, 1};
        row = 2*col.transposed();

        sp::Mat2 twos = col * row;
        REQUIRE(twos == sp::Mat2{{2, 2}, {2, 2}});

        sp::Mat3 identity{
            {1, 0, 0},
            {0, 1, 0},
            {0, 0, 1}
        };

        REQUIRE(identity == sp::Mat3::Identity());

        sp::Mat2 coeff{
            {1, 0},
            {0, 1}
        };

        coeff(0, 1) = 2;
        coeff(1, 0) = 3;

        REQUIRE(coeff == sp::Mat2{{1, 2}, {3, 1}});

        REQUIRE(sp::Mat2::Identity() + sp::Mat2::Zero() == sp::Mat2::Identity());

        REQUIRE(sp::Vec2::UnitX() == sp::Vec2::Unit(0));
        REQUIRE(sp::Vec2::UnitY() == sp::Vec2::Unit(1));
        REQUIRE(sp::Vec3::UnitZ() == sp::Vec3::Unit(2));
        REQUIRE(sp::Vec4::UnitW() == sp::Vec4::Unit(3));
        REQUIRE(
            sp::Vec4::UnitX() + sp::Vec4::UnitY() + sp::Vec4::UnitZ()
                + sp::Vec4::UnitW()
            == sp::Vec4{1, 1, 1, 1}
        );
    }

    SECTION("Matrix operations"){
        // add, mult, div, cross, dot, solver, ...

    }
}