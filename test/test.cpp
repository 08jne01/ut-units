#include "test.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <random>
#include <catch2/catch_get_random_seed.hpp>

using Catch::Matchers::WithinRel;
using Catch::Matchers::WithinAbs;

template<
    int second = 0, 
    int metre = 0,
    int kilogram = 0,
    int ampere = 0,
    int kelvin = 0,
    int mole = 0,
    int candela = 0
>
using qt = ut::qty<double,ut::qty_dimensions<second,metre,kilogram,ampere,kelvin,mole,candela>>;

TEST_CASE("Unit Operations", "[Units][Operators]")
{
    // just check that our custom pow works
    static_assert( pow(2.0, 0) == 1.0 );
    static_assert( pow(2.0, 1) == 2.0 );
    static_assert( pow(2.0, 2) == 2.0 * 2.0 );
    static_assert( pow(2.0, 3) == 2.0 * 2.0 * 2.0 );
    static_assert( pow(2.0, 4) == 2.0 * 2.0 * 2.0 * 2.0 );
    static_assert( pow(2.0, 5) == 2.0 * 2.0 * 2.0 * 2.0 * 2.0 );
    static_assert( pow(2.0, 6) == 2.0 * 2.0 * 2.0 * 2.0 * 2.0 * 2.0 );

    static_assert( pow(2.0, -1) == 1.0 / 2.0 );
    static_assert( pow(2.0, -2) == 1.0 / 2.0 / 2.0 );
    static_assert( pow(2.0, -3) == 1.0 / 2.0 / 2.0 / 2.0 );
    static_assert( pow(2.0, -4) == 1.0 / 2.0 / 2.0 / 2.0 / 2.0 );
    static_assert( pow(2.0, -5) == 1.0 / 2.0 / 2.0 / 2.0 / 2.0 / 2.0 );
    static_assert( pow(2.0, -6) == 1.0 / 2.0 / 2.0 / 2.0 / 2.0 / 2.0 / 2.0 );

    static_assert( requires(qt<5> q) {
        { q + qt<5>{} } -> std::same_as<qt<5>>;
        { q - qt<5>{} } -> std::same_as<qt<5>>;
        { q += qt<5>{} } -> std::same_as<void>;
        { q -= qt<5>{} } -> std::same_as<void>;
    });

    static_assert( requires(qt<5> q) {
        { q *= double{} };
        { q /= double{} };
        { q * double{} } -> std::same_as<qt<5>>;
        { q / double{} } -> std::same_as<qt<5>>;
        { q * qt<3>{} } -> std::same_as<qt<8>>;
        { q / qt<3>{} } -> std::same_as<qt<2>>;
    });

    // All the invalid stuff
    // static_assert( ! requires(qt<5> q) {
    //     { q + qt<4>{} } -> std::same_as<qt<5>>;
    //     { q - qt<4>{} } -> std::same_as<qt<5>>;
    //     { q += qt<4>{} } -> std::same_as<void>;
    //     { q -= qt<4>{} } -> std::same_as<void>;
    //     { q *= qt<5>{} };
    //     { q /= qt<5>{} };
    // });

    SECTION("Multiply/Divide")
    {
        static constexpr qt<1> t1 = t_time;
        static constexpr qt<2> t2 = t_time * t_time;
        static constexpr qt<3> t3 = t_time * t_time * t_time;
        static constexpr qt<4> t4 = t_time * t_time * t_time * t_time;
        static constexpr qt<5> t5 = t_time * t_time * t_time * t_time * t_time;
        static constexpr qt<6> t6 = t_time * t_time * t_time * t_time * t_time * t_time;

        REQUIRE( pow(t_time.value, 1) == t1.value );
        REQUIRE( pow(t_time.value, 2) == t2.value );
        REQUIRE( pow(t_time.value, 3) == t3.value );
        REQUIRE( pow(t_time.value, 4) == t4.value );
        REQUIRE( pow(t_time.value, 5) == t5.value );
        REQUIRE( pow(t_time.value, 6) == t6.value );

        static constexpr qt<-1> it1 =      1.0 / (t_time);
        static constexpr qt<-2> it2 = (    1.0 / t_time) / t_time ;
        static constexpr qt<-3> it3 = ((   1.0 / t_time) / t_time ) / t_time;
        static constexpr qt<-4> it4 = (((  1.0 / t_time) / t_time ) / t_time) / t_time;
        static constexpr qt<-5> it5 = (((( 1.0 / t_time) / t_time ) / t_time) / t_time) / t_time;
        static constexpr qt<-6> it6 = (((((1.0 / t_time) / t_time ) / t_time) / t_time) / t_time) / t_time;

        REQUIRE( pow(t_time.value, -1) == it1.value );
        REQUIRE( pow(t_time.value, -2) == it2.value );
        REQUIRE( pow(t_time.value, -3) == it3.value );
        REQUIRE( pow(t_time.value, -4) == it4.value );
        REQUIRE( pow(t_time.value, -5) == it5.value );
        REQUIRE( pow(t_time.value, -6) == it6.value );
    }

    SECTION("Add")
    {
        static constexpr qt<0,1> t_1 = { .value = 0.1 };
        static constexpr qt<0,1> t_2 = { .value = 0.2 };

        const qt<0,1> t_result = t_1 + t_2;
        REQUIRE( t_result.value == ( t_1.value + t_2.value ) );
    }

    SECTION("Subtract")
    {
        static constexpr qt<0,1> t_1 = { .value = 0.1 };
        static constexpr qt<0,1> t_2 = { .value = 0.2 };

        const qt<0,1> t_result = t_1 - t_2;
        REQUIRE( t_result.value == ( t_1.value - t_2.value ) );
    }

    SECTION("Negation")
    {
        static constexpr qt<0,1> t_1 = { .value = 0.1 };
        static constexpr qt<0,1> t_2 = { .value = 0.2 };

        static constexpr qt<0,1> nt_1 = -t_1;
        static constexpr qt<0,1> nt_2 = -t_2;

        REQUIRE( nt_1.value == -t_1.value );
        REQUIRE( nt_2.value == -t_2.value );
    }

    SECTION("Add Equals")
    {
        static constexpr qt<0,1> t_1 = { .value = 0.1 };
        static constexpr qt<0,1> t_2 = { .value = 0.2 };

        qt<0,1> t = 1.0 * t_1;
        t += 4.0 * t_2;

        REQUIRE( t.value == ( t_1.value + 4.0 * t_2.value ) );
    }

    SECTION("Subtract Equals")
    {
        static constexpr qt<0,1> t_1 = { .value = 0.1 };
        static constexpr qt<0,1> t_2 = { .value = 0.2 };

        qt<0,1> t = 1.0 * t_1;
        t -= 4.0 * t_2;

        REQUIRE( t.value == ( t_1.value - 4.0 * t_2.value ) );
    }

    SECTION("Comparison Equals")
    {
        static constexpr qt<0,1> t_1 = { .value = 0.1 };
        static constexpr qt<0,1> t_2 = { .value = 0.2 };

        { const bool x = t_1 < t_2; REQUIRE( x == true ); }
        { const bool x = t_1 > t_2; REQUIRE( x == false ); }

        { const bool x = t_1 <= t_2; REQUIRE(x == true); };
        { const bool x = t_1 >= t_2; REQUIRE(x == false); };

        { const bool x = t_1 != t_2; REQUIRE( x == true ); };
        { const bool x = t_1 == t_2; REQUIRE( x == false ); };
    }
}

TEST_CASE("Operations", "[Functions]")
{
    SECTION("pow")
    {
        static constexpr ut::length<double> length = 5.0 * ut::foot;
        static constexpr ut::area<double> area = ut::pow<2>( length );
        static constexpr ut::volume<double> volume = ut::pow<3>( length );

        REQUIRE( area.value == length.value * length.value );
        REQUIRE( volume.value == length.value * length.value * length.value );
    }

    SECTION("sqrt")
    {
        static constexpr ut::length<double> length = 5.0 * ut::foot;
        static constexpr ut::area<double> area = ut::pow<2>( length );
        const ut::length<double> length_again = ut::sqrt( area );
        REQUIRE( length.value == length_again.value );
    }
}

TEST_CASE("qty_offset", "[Units][Offset]")
{
    ut::temperature<double> t_c = 25.0 * ut::celsius;
    ut::temperature<double> t_f = 32.0 * ut::fahrenheit;

    REQUIRE_THAT( t_c.in(ut::kelvin), Catch::Matchers::WithinULP(298.15, 1) );
    REQUIRE_THAT( t_f.in(ut::kelvin), Catch::Matchers::WithinULP(273.15, 1) );

    REQUIRE_THAT( t_c.in(ut::celsius), Catch::Matchers::WithinULP(25.0, 1) );
    REQUIRE_THAT( t_f.in(ut::fahrenheit), Catch::Matchers::WithinULP(32.0, 1) );

}

TEST_CASE("Unit Operations Randomised", "[Units][Operators][Random]")
{
    std::mt19937 generator(Catch::getSeed());
    std::uniform_real_distribution<> distribution{-1.0e6, 1.0e6};

    for ( size_t i = 0; i < 10000; i++ )
    {
        const double unit_1_scale = distribution(generator);
        const double unit_2_scale = distribution(generator);
        const double unit_scalar = distribution(generator);
        CAPTURE(i,unit_1_scale, unit_2_scale, unit_scalar);
        // Actual dimensions are not used at run time at all, so it makes sense to only test a few types.
        Test<ut::qty_dimensions<-1,-4,2,5,-1,1,0>, 	ut::qty_dimensions<1,-4,0,1,3,-4,2>>(unit_1_scale, unit_2_scale, unit_scalar);
        Test<ut::qty_dimensions<-1,-4,2,5,-1,1,0>, 	ut::qty_dimensions<1,-4,0,1,3,-4,2>>(unit_1_scale, unit_2_scale, unit_scalar);
        Test<ut::qty_dimensions<-2,-2,2,2,4,1,-3>, 	ut::qty_dimensions<1,-4,0,0,-2,-3,-4>>(unit_1_scale, unit_2_scale, unit_scalar);
        Test<ut::qty_dimensions<-4,2,-5,1,0,2,-1>, 	ut::qty_dimensions<3,5,5,2,0,0,4>>(unit_1_scale, unit_2_scale, unit_scalar);

    }

    auto test_unit = TestUnit<ut::qty_dimensions<1,-5,6,7,12>>();
}