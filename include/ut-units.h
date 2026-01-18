/*
MIT License

Copyright (c) 2026 Joshua Nelson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <concepts>
#include <numbers>
#include <cmath>
#include <utility>

#pragma once
#ifndef UT_UNITS_CRITICAL_INLINE
#   if defined(_MSC_VER)
#       define UT_UNITS_CRITICAL_INLINE __forceinline
#   elif defined(__GNUC__) && __GNUC__>3
#       define UT_UNITS_CRITICAL_INLINE inline __attribute__ ((always_inline))
#   else
#       define UT_UNITS_CRITICAL_INLINE inline
#   endif
#endif

namespace ut::detail
{
    template<typename T>
    concept qty_dimensions_type = requires() {
        { T::d_second } -> std::convertible_to<int>;
        { T::d_metre } -> std::convertible_to<int>;
        { T::d_kilogram } -> std::convertible_to<int>;
        { T::d_ampere } -> std::convertible_to<int>;
        { T::d_kelvin } -> std::convertible_to<int>;
        { T::d_mole } -> std::convertible_to<int>;
        { T::d_candela } -> std::convertible_to<int>;
    };

    template<typename T>
    concept qty_type = requires() {
        requires std::floating_point<typename T::type>;
        requires qty_dimensions_type<typename T::dimensions>;
    };

    template<qty_dimensions_type T1, qty_dimensions_type T2>
    struct same_dimensions
    {
        static_assert(T1::d_second   == T2::d_second,   "Time dimension does not match");
        static_assert(T1::d_metre    == T2::d_metre,    "Length dimension does not match");
        static_assert(T1::d_kilogram == T2::d_kilogram, "Mass dimension does not match");
        static_assert(T1::d_ampere   == T2::d_ampere,   "Current dimension does not match");
        static_assert(T1::d_kelvin   == T2::d_kelvin,   "Temperature dimension does not match");
        static_assert(T1::d_mole     == T2::d_mole,     "Amount dimension does not match");
        static_assert(T1::d_candela  == T2::d_candela,  "Luminosity dimension does not match");
        static constexpr bool value = 
            T1::d_second   == T2::d_second &&
            T1::d_metre    == T2::d_metre &&
            T1::d_kilogram == T2::d_kilogram &&
            T1::d_ampere   == T2::d_ampere &&
            T1::d_kelvin   == T2::d_kelvin &&
            T1::d_mole     == T2::d_mole &&
            T1::d_candela  == T2::d_candela;
    };

    template<qty_dimensions_type T>
    struct sqrtable
    {
        static_assert(T::d_second   % 2 == 0 , "Time dimension not sqrt-able");
        static_assert(T::d_metre    % 2 == 0 , "Length dimension not sqrt-able");
        static_assert(T::d_kilogram % 2 == 0 , "Mass dimension not sqrt-able");
        static_assert(T::d_ampere   % 2 == 0 , "Current dimension not sqrt-able");
        static_assert(T::d_kelvin   % 2 == 0 , "Temperature dimension not sqrt-able");
        static_assert(T::d_mole     % 2 == 0 , "Amount dimension not sqrt-able");
        static_assert(T::d_candela  % 2 == 0 , "Luminosity dimension not sqrt-able");
        static constexpr bool value = 
            T::d_second   % 2 == 0 &&
            T::d_metre    % 2 == 0 &&
            T::d_kilogram % 2 == 0 &&
            T::d_ampere   % 2 == 0 &&
            T::d_kelvin   % 2 == 0 &&
            T::d_mole     % 2 == 0 &&
            T::d_candela  % 2 == 0;
    };

    template<typename T1, typename T2>
    concept compatible_qty = requires() {
        requires qty_type<T1>;
        requires qty_type<T2>;
        requires std::same_as<typename T1::type, typename T2::type>;
        requires same_dimensions<typename T1::dimensions, typename T2::dimensions>::value;
    };
} // end namespace ut::detail

namespace ut
{
    template<
        int second = 0, 
        int metre = 0,
        int kilogram = 0,
        int ampere = 0,
        int kelvin = 0,
        int mole = 0,
        int candela = 0
    >
    struct qty_dimensions
    {
        static constexpr int d_second = second;
        static constexpr int d_metre = metre;
        static constexpr int d_kilogram = kilogram;
        static constexpr int d_ampere = ampere;
        static constexpr int d_kelvin = kelvin;
        static constexpr int d_mole = mole;
        static constexpr int d_candela = candela;
    };

    template<
        std::floating_point T,
        detail::qty_dimensions_type dimensions = qty_dimensions<>
    >
    struct qty_offset;

    template<
        std::floating_point T,
        detail::qty_dimensions_type TyDimensions = qty_dimensions<>
    >
    struct qty
    { // cannot inherit otherwise MSVC will not optimise this into registers - clang and gcc will
        using type = T;
        using dimensions = TyDimensions;

        // stored in base SI units
        T value;

        [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr T in( 
            const qty_offset<T,dimensions>& other
        ) const;

        // This is only for dimensionless quantities
        // we could do something like template specialisation or use requires
        // however these all end in horrible error messages, static_assert is
        // the most clear way to convey this failure.
        [[nodiscard]] explicit constexpr operator T() const 
        { 
            static_assert( std::same_as<dimensions,qty_dimensions<>>, "Implicit conversion to floating point (T) requires dimensionless (ratios,angles,etc). You can use .in( ut::<your unit of choice> ) to convert to a floating point value in a specific unit." );
            return value; 
        }

        // This invalid operator is provided to give readable warnings upon incorrect assignment.
        template<typename Ty, typename other_dims>
        requires ( ! std::same_as<qty<Ty,other_dims>,qty> )
        [[nodiscard]] constexpr operator qty<Ty,other_dims>() const 
        { 
            static_assert( detail::same_dimensions<other_dims, dimensions>::value, "dimensions do not match" );
            static_assert( std::same_as<Ty, T>, "scalar types do not match consider using .cast<Scalar>()" );
            static_assert( false, "assignment failed - this operator is never valid" );
        }

        template<detail::compatible_qty<qty> Ty>
        [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr T in( const Ty& other ) const { return value / other.value; }

        // Convert to Specific Scalar but Keep the Unit
        // returns qty<Ty,dimensions>
        template<std::constructible_from<T> TyScalar>
        [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr qty<TyScalar,dimensions> cast() const
        { 
            qty<TyScalar,dimensions> result;
            result.value = TyScalar( value );
            return result; 
        }

        [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr qty<float,dimensions> f() const
        {
            qty<float,dimensions> result;
            result.value = float( value );
            return result;
        }
    };

    template<std::floating_point T, detail::qty_dimensions_type TyDimensions>
    struct qty_offset
    {
        using type = T;
        using dimensions = TyDimensions;
        // stored in base SI units
        T value;
        T offset;
    };

    template<std::floating_point T, detail::qty_dimensions_type dimensions>
    UT_UNITS_CRITICAL_INLINE constexpr T qty<T,dimensions>::in( const qty_offset<T,dimensions>& other) const
    {
        return (value / other.value) - other.offset;
    }

} // end namespace ut

namespace ut::detail
{
    template<std::floating_point T, detail::qty_dimensions_type T1, detail::qty_dimensions_type T2>
    struct qty_multiply_s
    {
        using value = qty<
            T,
            qty_dimensions<
                T1::d_second    + T2::d_second,
                T1::d_metre     + T2::d_metre,
                T1::d_kilogram  + T2::d_kilogram,
                T1::d_ampere    + T2::d_ampere,
                T1::d_kelvin    + T2::d_kelvin,
                T1::d_mole      + T2::d_mole,
                T1::d_candela   + T2::d_candela
            >
        >;
    };

    template<std::floating_point T, detail::qty_dimensions_type T1, detail::qty_dimensions_type T2>
    using qty_multiply = qty_multiply_s<T,T1,T2>::value;

    template<std::floating_point T, detail::qty_dimensions_type T1, detail::qty_dimensions_type T2>
    struct qty_divide_s
    {
        using value = qty<
            T,
            qty_dimensions<
                T1::d_second    - T2::d_second,
                T1::d_metre     - T2::d_metre,
                T1::d_kilogram  - T2::d_kilogram,
                T1::d_ampere    - T2::d_ampere,
                T1::d_kelvin    - T2::d_kelvin,
                T1::d_mole      - T2::d_mole,
                T1::d_candela   - T2::d_candela
            >
        >;
    };

    template<std::floating_point T, detail::qty_dimensions_type T1, detail::qty_dimensions_type T2>
    using qty_divide = qty_divide_s<T,T1,T2>::value;

    template<std::floating_point T, detail::qty_dimensions_type dimensions>
    requires(detail::sqrtable<dimensions>::value)
    struct qty_sqrt_s
    {
        using value = qty<
            T,
            qty_dimensions<
                dimensions::d_second    / 2,
                dimensions::d_metre     / 2,
                dimensions::d_kilogram  / 2,
                dimensions::d_ampere    / 2,
                dimensions::d_kelvin    / 2,
                dimensions::d_mole      / 2,
                dimensions::d_candela   / 2
            >
        >;
    };

    template<std::floating_point T, detail::qty_dimensions_type dimensions>
    using qty_sqrt = qty_sqrt_s<T,dimensions>::value;

    template<int N, std::floating_point T, detail::qty_dimensions_type dimensions>
    struct qty_pow_s
    {
        using value = qty<
            T,
            qty_dimensions<
                dimensions::d_second    * N,
                dimensions::d_metre     * N,
                dimensions::d_kilogram  * N,
                dimensions::d_ampere    * N,
                dimensions::d_kelvin    * N,
                dimensions::d_mole      * N,
                dimensions::d_candela   * N
            >
        >;
    };

    template<int N, std::floating_point T, detail::qty_dimensions_type dimensions>
    using qty_pow = qty_pow_s<N,T,dimensions>::value;

    template<typename T>
    struct qty_offset_to_qty_s
    {
        using type = qty<
            typename T::type,
            typename T::dimensions
        >;
    };

    template<typename T>
    using qty_offset_to_qty = qty_offset_to_qty_s<T>::type;
} // end namespace detail

namespace ut // operators, unit definitions and aliases
{
    template<
        std::floating_point T,
        detail::qty_dimensions_type TyLeft,
        detail::qty_dimensions_type TyRight
    >
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr detail::qty_multiply<T, TyLeft, TyRight>
    operator*(qty<T,TyLeft> left, qty<T,TyRight> right)
    {
        detail::qty_multiply<T, TyLeft,TyRight> value;
        value.value = left.value * right.value;
        return value;
    }

    template<
        std::floating_point T,
        detail::qty_dimensions_type TyLeft,
        detail::qty_dimensions_type TyRight
    >
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr detail::qty_divide<T, TyLeft, TyRight>
    operator/( qty<T,TyLeft> left, qty<T,TyRight> right)
    {
        detail::qty_divide<T, TyLeft,TyRight> value;
        value.value = left.value / right.value;
        return value;
    }

    template<typename TyLeft, typename TyRight>
    requires( detail::compatible_qty<TyLeft,TyRight> )
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr TyLeft operator+(TyLeft left, TyRight right)
    {
        TyLeft value;
        value.value = left.value + right.value;
        return value;
    }

    template<typename TyLeft, typename TyRight>
    requires( detail::compatible_qty<TyLeft,TyRight> )
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr TyLeft operator-(TyLeft left, TyRight right)
    {
        TyLeft value;
        value.value = left.value - right.value;
        return value;
    }

    template<typename TyLeft, typename TyRight>
    requires( detail::compatible_qty<TyLeft,TyRight> )
    UT_UNITS_CRITICAL_INLINE constexpr void operator+=(TyLeft& left, TyRight right)
    {
        left.value += right.value;
    }

    template<typename TyLeft, typename TyRight>
    requires( detail::compatible_qty<TyLeft,TyRight> )
    UT_UNITS_CRITICAL_INLINE constexpr void operator-=(TyLeft& left, TyRight right)
    {
        left.value -= right.value;
    }

    template<typename TyLeft, typename TyRight>
    requires( detail::compatible_qty<TyLeft,TyRight> )
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr bool operator<(TyLeft left, TyRight right)
    {
        return left.value < right.value;
    }

    template<typename TyLeft, typename TyRight>
    requires( detail::compatible_qty<TyLeft,TyRight> )
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr bool operator<=(TyLeft left, TyRight right)
    {
        return left.value <= right.value;
    }

    template<typename TyLeft, typename TyRight>
    requires( detail::compatible_qty<TyLeft,TyRight> )
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr bool operator>(TyLeft left, TyRight right)
    {
        return left.value > right.value;
    }

    template<typename TyLeft, typename TyRight>
    requires( detail::compatible_qty<TyLeft,TyRight> )
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr bool operator>=(TyLeft left, TyRight right)
    {
        return left.value >= right.value;
    }

    template<typename TyLeft, typename TyRight>
    requires( detail::compatible_qty<TyLeft,TyRight> )
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr bool operator==(TyLeft left, TyRight right)
    {
        return left.value == right.value;
    }

    template<typename TyLeft, typename TyRight>
    requires( detail::compatible_qty<TyLeft,TyRight> )
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr bool operator!=(TyLeft left, TyRight right)
    {
        return left.value != right.value;
    }

    template<std::floating_point T, detail::qty_dimensions_type dimensions>
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr qty<T,dimensions> operator-(qty<T,dimensions> value)
    {
        value.value = -value.value;
        return value;
    }

    template<std::floating_point T, detail::qty_dimensions_type dimensions, std::convertible_to<T> Ty>
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr qty<T,dimensions> operator*(Ty left, qty<T,dimensions> right)
    {
        right.value *= left;
        return right;
    }

    template<std::floating_point T, detail::qty_dimensions_type dimensions>
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr qty<T,dimensions> operator*(qty<T,dimensions> left, T right)
    {
        left.value *= right;
        return left;
    }

    template<std::floating_point T, detail::qty_dimensions_type dimensions>
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr detail::qty_divide<T, qty_dimensions<>, dimensions> operator/(T left, qty<T,dimensions> right)
    {
        detail::qty_divide<T, qty_dimensions<>, dimensions> result;
        result.value = left / right.value;
        return result;
    }

    template<std::floating_point T, detail::qty_dimensions_type dimensions>
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr qty<T,dimensions> operator/(qty<T,dimensions> left, T right)
    {
        left.value /= right;
        return left;
    }

    template<std::floating_point T, detail::qty_dimensions_type dimensions>
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr qty<T,dimensions> operator*(T left, qty_offset<T,dimensions> right)
    {
        qty<T,dimensions> result;
        result.value = (left + right.offset) * right.value;
        return result;
    }

    template<std::floating_point T, detail::qty_dimensions_type dimensions>
    UT_UNITS_CRITICAL_INLINE constexpr void operator*=(qty<T,dimensions>& left, T right)
    {
        left.value *= right;
    }

    template<std::floating_point T, detail::qty_dimensions_type dimensions>
    UT_UNITS_CRITICAL_INLINE constexpr void operator/=(qty<T,dimensions>& left, T right)
    {
        left.value /= right;
    }

    // SI units
    static constexpr qty<double,qty_dimensions<1>>              second      { .value = 1.0 };
    static constexpr qty<double,qty_dimensions<0,1>>            metre       { .value = 1.0 };
    static constexpr qty<double,qty_dimensions<0,0,1>>          kilogram    { .value = 1.0 };
    static constexpr qty<double,qty_dimensions<0,0,0,1>>        ampere      { .value = 1.0 };
    static constexpr qty<double,qty_dimensions<0,0,0,0,1>>      kelvin      { .value = 1.0 };
    static constexpr qty<double,qty_dimensions<0,0,0,0,0,1>>    mole        { .value = 1.0 };
    static constexpr qty<double,qty_dimensions<0,0,0,0,0,0,1>>  candela     { .value = 1.0 };

    // unitless
    static constexpr qty<double>                one{.value = 1.0};
    static constexpr qty<double>                radian{.value = 1.0};
    static constexpr qty<double>                degree{.value = std::numbers::pi / 180.0};

    static constexpr qty_offset<double,qty_dimensions<0,0,0,0,1>>      celsius{.value = 1.0, .offset = 273.15};

    // derrived
    static constexpr qty minute                 = 60.0 * second;
    static constexpr qty hour                   = 60.0 * minute;
    static constexpr qty kilometre              = 1000.0 * metre;
    static constexpr qty milimetre              = 1.0e-3 * metre;
    static constexpr qty centimetre             = 1.0e-2 * metre;
    static constexpr qty decimetre              = 0.1 * metre;
    static constexpr qty newton                 = kilogram * metre / ( second * second );
    static constexpr qty newton_metre           = newton * metre;
    static constexpr qty pascal                 = newton / ( metre * metre ); // this (pascal) can conflict with a macro in Windows.h
    static constexpr qty metre_per_second       = metre / second;
    static constexpr qty kilometre_per_hour     = kilometre / hour;
    static constexpr qty metre_per_second2      = metre / (second * second);
    static constexpr qty joule                  = newton * metre;
    static constexpr qty watt                   = joule / second;
    static constexpr qty kilowatt               = 1000.0 * watt;
    static constexpr qty watt_hour              = watt * hour;
    static constexpr qty kilowatt_hour          = kilowatt * hour;
    static constexpr qty volt                   = watt / ampere;
    static constexpr qty hertz                  = 1.0 / second;
    static constexpr qty coulomb                = second * ampere;
    static constexpr qty farad                  = coulomb / volt;
    static constexpr qty ohm                    = volt / ampere;
    static constexpr qty gram                   = 1.0e-3 * kilogram;
    static constexpr qty kilogram_per_second    = kilogram / second;
    static constexpr qty kilogram_metre2        = kilogram * metre * metre;

    static constexpr qty litre                  = decimetre * decimetre * decimetre;
    static constexpr qty metre2                 = metre * metre;
    static constexpr qty centimetre2            = centimetre * centimetre;
    static constexpr qty metre3                 = metre * metre * metre;
    static constexpr qty radian_per_second      = radian / second;
    static constexpr qty radian_per_second2     = radian / (second*second);
    static constexpr qty degree_per_second      = degree / second;
    static constexpr qty kilogram_per_metre3    = kilogram / metre3;

    static constexpr qty revolution             = { .value = 2.0 * std::numbers::pi };
    static constexpr qty revolution_per_second  = revolution / second;
    static constexpr qty revolution_per_minute  = revolution / minute; 

    // International Units
    static constexpr qty foot                   = 0.3048 * metre;
    static constexpr qty inch                   = 25.4 * milimetre;
    static constexpr qty nautical_mile          = 1852.0 * metre;
    static constexpr qty yard                   = 3.0 * foot;
    static constexpr qty mile                   = 5280.0 * foot;

    static constexpr qty pound                  = 0.453592 * kilogram;
    static constexpr qty pound_force            = 4.448222 * newton;
    static constexpr qty rankine                = (5.0 / 9.0) * kelvin;
    static constexpr qty knot                   = nautical_mile / hour;
    static constexpr qty gallon                 = 3.785411784 * litre; // US gallon
    static constexpr qty imperial_gallon        = 4.54609 * litre;
    static constexpr qty quart                  = 0.946352946 * litre;

    static constexpr qty_offset<double,qty_dimensions<0,0,0,0,1>> fahrenheit{ .value = rankine.value, .offset = 459.67 };


    static constexpr qty pound_per_square_inch  = pound_force / ( inch * inch );
    static constexpr qty foot_pounds            = foot * pound_force;
    static constexpr qty foot_per_second        = foot / second;
    static constexpr qty pound_per_second       = pound / second;
    static constexpr qty pound_per_hour         = pound / hour;

    // Type Aliases for Quantities
    // Here we specify common quantity types to make it easy to declare these
    // in class/structs. In other places template deduction can be used.
    template<std::floating_point T> using angle         = qty<T,qty_dimensions<>>;
    template<std::floating_point T> using dimensionless = qty<T,qty_dimensions<>>;
    template<std::floating_point T> using time          = qty<T,qty_dimensions<1>>;
    template<std::floating_point T> using length        = qty<T,qty_dimensions<0,1>>;
    template<std::floating_point T> using mass          = qty<T,qty_dimensions<0,0,1>>;
    template<std::floating_point T> using current       = qty<T,qty_dimensions<0,0,0,1>>;
    template<std::floating_point T> using temperature   = qty<T,qty_dimensions<0,0,0,0,1>>;
    template<std::floating_point T> using amount        = qty<T,qty_dimensions<0,0,0,0,0,1>>;
    template<std::floating_point T> using luminosity    = qty<T,qty_dimensions<0,0,0,0,0,0,1>>;

    template<std::floating_point T> using speed                 = qty<T,decltype(metre_per_second)::dimensions>;
    template<std::floating_point T> using acceleration          = qty<T,decltype(metre_per_second2)::dimensions>;
    template<std::floating_point T> using force                 = qty<T,decltype(newton)::dimensions>;
    template<std::floating_point T> using energy                = qty<T,decltype(joule)::dimensions>;
    template<std::floating_point T> using power                 = qty<T,decltype(watt)::dimensions>;
    template<std::floating_point T> using volume                = qty<T,decltype(litre)::dimensions>;
    template<std::floating_point T> using pressure              = qty<T,decltype(pascal)::dimensions>;
    template<std::floating_point T> using torque                = qty<T,decltype(newton_metre)::dimensions>;
    template<std::floating_point T> using area                  = qty<T,decltype(metre2)::dimensions>;
    template<std::floating_point T> using angular_rate          = qty<T,decltype(radian_per_second)::dimensions>;
    template<std::floating_point T> using angular_acceleration  = qty<T,decltype(radian_per_second2)::dimensions>;
    template<std::floating_point T> using frequency             = qty<T,decltype(hertz)::dimensions>;
    template<std::floating_point T> using mass_flow_rate        = qty<T,decltype(kilogram_per_second)::dimensions>;
    template<std::floating_point T> using moment_of_inertia     = qty<T,decltype(kilogram_metre2)::dimensions>;
    template<std::floating_point T> using density               = qty<T,decltype(kilogram_per_metre3)::dimensions>;
}

namespace sym
{
    // SI Units
    // https://en.wikipedia.org/wiki/International_System_of_Units

    static constexpr auto s         = ut::second;
    static constexpr auto m         = ut::metre;
    static constexpr auto kg        = ut::kilogram;
    static constexpr auto A         = ut::ampere;
    static constexpr auto K         = ut::kelvin;
    static constexpr auto mol       = ut::mole;
    static constexpr auto cd        = ut::candela;
    
    static constexpr auto rad       = ut::radian;
    static constexpr auto Hz        = ut::hertz;
    static constexpr auto N         = ut::newton;
    static constexpr auto pa        = ut::pascal;
    static constexpr auto j         = ut::joule;
    static constexpr auto W         = ut::watt;
    static constexpr auto kW        = ut::kilowatt;
    static constexpr auto kWH       = ut::kilowatt_hour;

    static constexpr auto C         = ut::coulomb;
    static constexpr auto V         = ut::volt;
    static constexpr auto F         = ut::farad;

    static constexpr auto mm        = ut::milimetre;
    static constexpr auto km        = ut::kilogram;
    static constexpr auto degC      = ut::celsius;
    static constexpr auto kgps      = ut::kilogram_per_second;
    static constexpr auto kgpm3     = ut::kilogram_per_metre3;


    static constexpr auto m2        = ut::metre2;
    static constexpr auto L         = ut::litre;

    static constexpr auto deg       = ut::degree;
    static constexpr auto degps     = ut::degree_per_second;
    static constexpr auto radps     = ut::radian_per_second;
    static constexpr auto rps       = ut::revolution_per_second;
    static constexpr auto rpm       = ut::revolution_per_minute;

    static constexpr auto kgm2      = ut::kilogram_metre2;

    // International Units
    // https://en.wikipedia.org/wiki/United_States_customary_units
    static constexpr auto nmi       = ut::nautical_mile;
    static constexpr auto ft        = ut::foot;
    static constexpr auto mi        = ut::mile;
    static constexpr auto yd        = ut::yard;
    static constexpr auto lb        = ut::pound;
    static constexpr auto lbf       = ut::pound_force;
    static constexpr auto gal       = ut::gallon;
    static constexpr auto qt        = ut::quart;
    static constexpr auto kt        = ut::knot;
    static constexpr auto psi       = ut::pound_per_square_inch;
    static constexpr auto fps       = ut::foot_per_second;
    static constexpr auto degF      = ut::fahrenheit;
    static constexpr auto pph       = ut::pound_per_hour;
}

// functions
namespace ut
{
    // These functions below are generalised functions for raw floats and qty to 
    // use the other helper functions like abs,copysign sqrt easily.

    // Returns underling value
    template<std::floating_point T>
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr T scalar( T value ) { return value; }

     // Returns underling value
    template<std::floating_point T, detail::qty_dimensions_type dimensions>
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr T scalar( qty<T,dimensions> value ) { return value.value; }

    // Assigns value from floating point
    template<std::floating_point T>
    UT_UNITS_CRITICAL_INLINE constexpr void assign( T& value, T new_value ) { value = new_value; }

    // Assigns value from floating point
    template<std::floating_point T, detail::qty_dimensions_type dimensions>
    UT_UNITS_CRITICAL_INLINE constexpr void assign( qty<T,dimensions>& value, T new_value ) { value.value = new_value; }

    // template<std::floating_point T, typename dimensions>
    // UT_UNITS_CRITICAL_INLINE constexpr qty<T,dimensions> create( T value )

    template<std::floating_point T, detail::qty_dimensions_type dimensions>
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE detail::qty_sqrt<T,dimensions> sqrt( qty<T,dimensions> value )
    {
        detail::qty_sqrt<T,dimensions> result;
        result.value = std::sqrt(value.value);
        return result;
    }

    template<int N, std::floating_point T, detail::qty_dimensions_type dimensions>
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE constexpr detail::qty_pow<N,T,dimensions> pow( qty<T,dimensions> value )
    {
        if constexpr ( N >= 0 )
        {
            detail::qty_pow<N,T,dimensions> result;
            result.value = 1.0;
            for ( size_t i = 0; i < N; i++ )
            {
                result.value *= value.value;
            }
            return result;
        }
        else
        {
            detail::qty_pow<N,T,dimensions> result;
            result.value = 1.0;
            for ( size_t i = 0; i < std::abs(N); i++ )
            {
                result.value /= value.value;
            }
            return result;
        }
    }

    template<typename T>
    [[nodiscard]] UT_UNITS_CRITICAL_INLINE T abs( T value )
    {
        const auto new_value = std::abs(scalar(value));
        assign(value, new_value);
        return value;
    }
 
    template<typename TyNumber, typename TySign>
    UT_UNITS_CRITICAL_INLINE TyNumber copysign( TyNumber number, TySign sign )
    {
        const auto new_value = std::copysign(scalar(number), scalar(sign));
        assign(number, new_value);
        return number;
    }
}