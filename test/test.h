#pragma once 
#include <ut-units.h>
#include <catch2/catch_test_macros.hpp>

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

// Forms a test unit from set of dimensions using the units above.
template<ut::detail::qty_dimensions_type dimensions>
ut::qty<double,dimensions> TestUnit()
{
    const ut::qty<double,dimensions> unit = 
        ut::pow<dimensions::d_second>(t_time) *
        ut::pow<dimensions::d_metre>(t_length) *
        ut::pow<dimensions::d_kilogram>(t_mass) *
        ut::pow<dimensions::d_ampere>(t_current) *
        ut::pow<dimensions::d_kelvin>(t_temp) *
        ut::pow<dimensions::d_mole>(t_amount) *
        ut::pow<dimensions::d_candela>(t_lum);

    const double si_value = 
        pow(t_time.value, dimensions::d_second) *
        pow(t_length.value, dimensions::d_metre) *
        pow(t_mass.value, dimensions::d_kilogram) *
        pow(t_current.value, dimensions::d_ampere) *
        pow(t_temp.value, dimensions::d_kelvin) *
        pow(t_amount.value, dimensions::d_mole) *
        pow(t_lum.value, dimensions::d_candela);

    REQUIRE(si_value == unit.value);
    return unit;
}

template<typename dimensions1, typename dimensions2>
void TestMultiply(double unit_1_scale, double unit_2_scale, double scalar_value)
{
    ut::qty<double, dimensions1> unit_1 = TestUnit<dimensions1>();
    ut::qty<double, dimensions2> unit_2 = TestUnit<dimensions2>();

    const double base_unit_1_value = unit_1.value;
    const double base_unit_2_value = unit_2.value;

    const double unit_1_multiply_scalar = (unit_1_scale * unit_1.value) * scalar_value;
    const double unit_2_multiply_scalar = (unit_2_scale * unit_2.value) * scalar_value;

    unit_1 *= unit_1_scale;
    unit_2 *= unit_2_scale;

    const auto multiply = unit_1 * unit_2;
    REQUIRE( multiply.value == 
        (base_unit_1_value * unit_1_scale) * (base_unit_2_value * unit_2_scale) 
    );

    decltype(unit_1) multiply_scalar1 = unit_1 * scalar_value;
    REQUIRE( multiply_scalar1.value == unit_1_multiply_scalar );
    
    multiply_scalar1 = scalar_value * unit_1;
    REQUIRE( multiply_scalar1.value == unit_1_multiply_scalar );

    decltype(unit_2) multiply_scalar2 = unit_2 * scalar_value;
    REQUIRE( multiply_scalar2.value == unit_2_multiply_scalar );

    multiply_scalar2 = scalar_value * unit_2;
    REQUIRE( multiply_scalar2.value == unit_2_multiply_scalar );

    unit_1 *= scalar_value;
    unit_2 *= scalar_value;

    REQUIRE(unit_1.value == unit_1_multiply_scalar);
    REQUIRE(unit_2.value == unit_2_multiply_scalar);

}

template<typename dimensions1, typename dimensions2>
void TestDivide(double unit_1_scale, double unit_2_scale, double scalar_value)
{
    ut::qty<double, dimensions1> unit_1 = TestUnit<dimensions1>();
    ut::qty<double, dimensions2> unit_2 = TestUnit<dimensions2>();

    const double base_unit_1_value = unit_1.value;
    const double base_unit_2_value = unit_2.value;

    const double unit_1_divide_scalar = (unit_1_scale * unit_1.value) / scalar_value;
    const double unit_2_divide_scalar = (unit_2_scale * unit_2.value) / scalar_value;

    unit_1 *= unit_1_scale;
    unit_2 *= unit_2_scale;


    const auto divide = unit_1 / unit_2;
    REQUIRE( divide.value == 
        (base_unit_1_value * unit_1_scale) / (base_unit_2_value * unit_2_scale) 
    );

    decltype(unit_1) divide_scalar_1 = unit_1 / scalar_value;
    REQUIRE( divide_scalar_1.value == unit_1_divide_scalar );

    decltype(unit_2) divide_scalar_scalar2 = unit_2 / scalar_value;
    REQUIRE( divide_scalar_scalar2.value == unit_2_divide_scalar );

    unit_1 /= scalar_value;
    unit_2 /= scalar_value;

    REQUIRE(unit_1.value == unit_1_divide_scalar);
    REQUIRE(unit_2.value == unit_2_divide_scalar);
}

template<typename dimensions>
void TestAddSubtract(double unit_1_scale, double unit_2_scale, double scalar_value)
{
    ut::qty<double, dimensions> unit_1 = TestUnit<dimensions>();
    ut::qty<double, dimensions> unit_2 = TestUnit<dimensions>();

    const double base_unit_1_value = unit_1.value;
    const double base_unit_2_value = unit_2.value;

    unit_1 *= unit_1_scale;
    unit_2 *= unit_2_scale;

    ut::qty<double,dimensions> result_add = unit_1 + unit_2;
    REQUIRE( result_add.value == (base_unit_1_value * unit_1_scale) + (base_unit_2_value * unit_2_scale) );
    
    ut::qty<double,dimensions> result_subtract = unit_1 - unit_2;
    REQUIRE( result_subtract.value == (base_unit_1_value * unit_1_scale) - (base_unit_2_value * unit_2_scale) );
    
    auto unit_1_copy = unit_1;
    unit_1_copy += unit_2;
    REQUIRE( unit_1_copy.value == (base_unit_1_value * unit_1_scale) + (base_unit_2_value * unit_2_scale) );

    unit_1_copy = unit_1;
    unit_1_copy -= unit_2;
    REQUIRE( unit_1_copy.value == (base_unit_1_value * unit_1_scale) - (base_unit_2_value * unit_2_scale) );

}

template<typename dimensions1, typename dimensions2>
void Test(double unit_1_scale, double unit_2_scale, double scalar_value)
{
    TestMultiply<dimensions1,dimensions2>(unit_1_scale, unit_2_scale, scalar_value);
    TestDivide<dimensions1,dimensions2>(unit_1_scale, unit_2_scale, scalar_value);
    TestAddSubtract<dimensions1>(unit_1_scale, unit_2_scale, scalar_value);
    TestAddSubtract<dimensions2>(unit_1_scale, unit_2_scale, scalar_value);
}