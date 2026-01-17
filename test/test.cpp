#include <ut-units.h>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
//#include <GD/Math/Vector.h>


using Catch::Matchers::WithinRel;
using Catch::Matchers::WithinAbs;

consteval double pow( double x, int y )
{
    if ( y > 0 )
    {
        double result = 1.0;
        for ( int i = 0; i < y; i++ )
            result *= x;

        return result;
    }
    else if ( y < 0 )
    {
        double result = 1.0;
        for ( int i = 0; i < -y; i++ )
            result /= x;

        return result;
    }

    return 1.0;
}

// test units
static constexpr ut::qty<double,ut::qty_dimensions<1>>              t_time       { .value = 0.1 };
static constexpr ut::qty<double,ut::qty_dimensions<0,1>>            t_length     { .value = 0.2 };
static constexpr ut::qty<double,ut::qty_dimensions<0,0,1>>          t_mass       { .value = 0.3 };
static constexpr ut::qty<double,ut::qty_dimensions<0,0,0,1>>        t_current    { .value = 0.4 };
static constexpr ut::qty<double,ut::qty_dimensions<0,0,0,0,1>>      t_temp       { .value = 0.5 };
static constexpr ut::qty<double,ut::qty_dimensions<0,0,0,0,0,1>>    t_amount     { .value = 0.6 };
static constexpr ut::qty<double,ut::qty_dimensions<0,0,0,0,0,0,1>>  t_lum        { .value = 0.7 };

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

TEST_CASE("Unit-Float Operations", "[Units][Operators][Float]")
{

} 

TEST_CASE("Unit Operations", "[Units][Operators]")
{
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