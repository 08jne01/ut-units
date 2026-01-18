# Quantity

## Introduction

`ut::qty` is the core type of the ut-units library. It describes a value and its dimensions (stored as the type). Internally `ut::qty` stores its value in base SI units. This means conversions happen at compile time or when the [in](#in) function is explicitly used.

## Unit definitions

Units are defined as a `constexpr ut::qty` this means that new units can easily be defined by combining other units. Since the units stored value is ratio of this unit to that of its equivalent unit in base SI units.

This is covered more in the [units section](./units.md).

## qty

Below is the definition of `ut::qty`

```cpp
template<
    std::floating_point T,
    typename TyDimensions = qty_dimensions<>
>
struct qty;
```

`T` is the scalar and this must be a floating point type.

`TyDimensions` is an instance of `ut::qty_dimensions` with the dimensions specialised. This encodes the dimensions into the type as a set of integers representing the powers of each type.

Floating point types cannot be implicitly converted to `ut::qty` **even** if the quantity is dimensionless. This is by design. However `ut::qty` can be implicitly converted back to a floating point if the quantity is dimensionless, see [operator T](#operator-t).

Below are a list of all the members of qty.

## qty_dimensions

`qty_dimensions` specifies the dimensions for this type. Below is the definition.

```cpp
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
```

each of the 7 fundemental dimensions are specified by an integer indicating the power. For example the dimensions LT^-2 is indicated by `qty_dimensions<-2,1>`.

## Usage

Below is some example usage of the various functions and operations. We define some quantities with their respective units.

```cpp
ut::length<double> distance     = 5000.0 * ut::foot;            // 5000 ft
ut::mass<double>    mass        = 2.0 * ut::kilogram;           // 2 kg
ut::speed<double>   velocity    = 3.0 * ut::metre_per_second;   // 3 m/s
ut::temperature<double> temp    = 25.0 * ut::celsius;           // 25 °C
```

Calculating kinetic energy.

```cpp
ut::energy<double> kinetic_energy = 0.5 * mass * ut::pow<2>(velocity);
double kinetic_energy_joules = kinetic_energy.in(ut::joule);
std::cout << std::format("Kinetic energy {} J\n", kinetic_energy_joules);
```

Converting distance.

```cpp
std::cout << std::format("Distance {} ft, {} km\n", distance.in(ut::foot), distance.in(ut::kilometre));
```

Converting temperature.

```cpp
std::cout << std::format("Temperature {} °C, {} K\n", temp.in(ut::celsius), temp.in(ut::kelvin));
```

Function arguments. Pass by value is same as double/float ([with negligable exception on MSVC](index.md#overhead)).

```cpp
auto calculate_potential_energy = []( ut::mass<double> mass, ut::length<double> height ) -> ut::energy<double> {   
    // gravitational acceleration g = 9.81 m/s^2
    constexpr ut::acceleration<double> g = 9.81 * ut::metre_per_second2;
    return mass * g * height; // kg * (m/s^2) * m -> J
};

ut::length<double> height = 2.5 * ut::foot;
ut::energy<double> potential_energy = calculate_potential_energy(mass, height);
ut::time<double> time = 1.0 * ut::minute;
ut::power<double> power = potential_energy / time;
std::cout << std::format("Energy {} J, Power {} kW\n", potential_energy.in(ut::joule), power.in(ut::kilowatt));
```

## Members

### in

```cpp
scalar_t qty<scalar_t,dimensions>::in(qty<scalar_t,dimensions> other) const;
```

returns quantity as `scalar_t` (double or float) converted to the corresponding unit `other`

```cpp
scalar_t qty<scalar_t,dimensions>::in(qty_offset<scalar_t,dimensions> other) const;
```

see [Quantity Offset](#quantity-offset) section

### cast

```cpp
qty<new_scalar_t, dimensions> qty<scalar_t,dimensions>::cast() const; 
```

returns quantity with new scalar type `new_scalar_t` by using static_cast on the underlying `scalar_t`

### f

```cpp
qty<float,dimensions> qty<scalar_t, dimensions>::f() const;
```

returns quantity with new scalar type `float` by using static_cast on the underlying `scalar_t`, equivalent to `qty::cast<float>()`

### value

```cpp
qty<scalar_t,dimensions>::value
```

this is the internal `scalar_t` value store by `qty`. It is exposed for use with interfaces requiring a `scalar_t&`, otherwise it is advisable to use the [in](#in) function instead as this provides unit safety whereas direct value access does not.

### operator T

```cpp
qty<scalar_t,dimensions>::operator scalar_t() const;
```

returns internal SI value as `scalar_t` if implicit conversion is satisfied

> requires dimensions are dimensionless

### operator qty

```cpp
qty<scalar_t,dimensions>::operator qty<new_scalar_t,new_dimensions>() const;
```

this operator is always invalid and is provided to allow readable compile errors when assiging mismatched dimensions

requires `qty<scalar,dimensions> != qty<new_scalar_t, new_dimensions>`, this enables normal assignment when they are the same type

## Quantity Offset

These are a special case for offset quantities such as temperature. ut-units generally only handles ratios however the `qty_offset` type is provided for special cases. If you need better handling of such units the [mp-units](https://github.com/mpusz/mp-units) library may be better suited.

Only one operator is provided for `qty_offset`

```cpp
template<std::floating_point T, typename TyDimension>
constexpr qty<T,TyDimension> operator*(T left, qty_offset<T,TyDimension> right)
```

this allows conversion from a scalar type to a regular quantity using a `qty_offset`. Regular quantities can also be converted to an offset unit as a scalar using a `qty_offset`.

```cpp
// operator* converts from celsius to kelvin SI unit
ut::temperature<double> T = 10.0 * ut::celsius;

// ut::fahrenheit is a qty_offset
double T_in_fahrenheit = T.in( ut::fahrenheit );
```
