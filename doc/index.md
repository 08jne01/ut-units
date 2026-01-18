# ut-units

## Introduction

ut-units is a light weight single header only C++20 compile time unit and dimensional analysis system library. It is heavily inspire by the awesome [mp-units](https://github.com/mpusz/mp-units).

Like mp-units this library allows compile time validation and automatic conversions.

The primary motivation for this library to be lightweight: provide fast compile times and zero runtime overhead over using normal scalar types. ut-units also provide readible easy to understand compile time errors. The majority of unit related use cases are covered however if you have very specific requirements the [mp-units](https://github.com/mpusz/mp-units) library is probably more well suited.

For the MSVC compiler the requirements for code generation to pass structs by register are extremely strict: ut-units makes sure that even on MSVC unit quantities are passed by register ensure [little overhead](#overhead).

[See the repository here](https://github.com/08jne01/ut-units).

[See the documentation here](https://08jne01.github.io/ut-units/).

## Features

- Compile time checking
- Little to no runtime overhead
- Fast compile times
- Easily readable compile-time errors
- Easy to understand syntax
- Easily extendible with further units

## Examples

### Converting to and from unit

To convert to a unit from a floating point simply multiply it by the corresponding unit.

```cpp
ut::speed<double> speed = 10.0 * ut::meter_per_second;
```

To convert from a unit back to a floating point the `in` function can be used with a compatible unit.

```cpp
ut::length<double> length = 10.0 * ut::metre;
double length_in_ft_as_double = length.in(ut::foot);
```

This makes it easy to interface with existing code that does not use units. See the [quantity page](quantity.md) for more.

### Using Units

All the usual operators you would expect are overloaded, operators are appropriately constrained. For example you cannot add metre to a second.

```cpp
ut::time<double> dt; //assume this has a valid value
ut::angle<double> angle; // assume this is some variable we wish to change

ut::angular_rate<double> drift_rate = 10.0 * ut::degree / ut::hour;
angle += dt * drift_rate;

const double cosa = std::cos( angle ); // dimensionless are implicitly converted to corresponding floating point
```

### Gotchas

Care must be taken when creating some quantities as incorrect brackets can lead to unexpected results. This is because operator presedence still applies. Consider the example below

```cpp
ut::angular_rate<double> rate  = 1.0 * ut::degree / ( 2.0 * ut::minute );
ut::angular_rate<double> rate  = 1.0 * ut::degree / 2.0 * ut::minute; // this is the same as 1.0 * ( ut::degree / 2.0) * ut::minute and fails to compile
```

### Symbols

Short hand symbols are provided for convenient use in the sym namespace.

```cpp
ut::energy<double> energy = 10.0 * sym::kWH
```

See the [units page](units.md) for more.

### Defining New Units

Units can be can create by simply combining existing units. Units are defined using the `double` type so they can be easily converted to `float` with the most accurate precision.

```cpp
static constexpr ut::qty newton = ut::kilogram * ut::metre / ( ut::second * ut::second );
static constexpr ut::qty nautical_mile = 1852.0 * ut::metre;
static constexpr ut::qty knot = ut::nautical_mile / ut::hour;
```

See the [units page](units.md) for more.

### Defining New Unit Aliases

Unit aliases are names for categories of unit which share the same dimension

```cpp
template<std::floating_point T> using speed = ut::qty<T,decltype(ut::metre_per_second)::dimensions>;
```

See the [aliases page](aliases.md) for more.

### Converting from double to float

Units are defined with the `double` type but can be easily converted `float`.

```cpp
ut::force<float> force = 100.0f * ut::newton.f();
```

## Overhead

Since we want this to be as close to a "zero cost abstraction" as possible it is important we ensure that the generated code is the same or similar to the equivalent code written as doubles. This is mostly the case when it comes to gcc and clang compilers however the MSVC compiler struggles with this when it comes to passing these units by value.

It is expected a struct of 8 bytes be passed in a register and in the best case passed in the xmm registers so it can be used immediately. However the MSVC compiler does not pass any struct in registers if the data is not a [plain old data type](https://learn.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-170) **and** the struct does not inherit from anything. The second point is the exact reason that the fundemental type ut::qty does not inherit from anything.

Considering all this ut::qty is able to be passed by value in the registers on MSVC however it will use 1 additional instruction to move it from the integer register to an xmm floating pointer register. Moving from the integer register to floating point register is much prefer to behaviour of passing by reference and the cost that incurs with literal types. clang and gcc pass in xmm registers as they seem to understand that ut::qty is just a floating point number.

The original purpose for this library was to be built with MSVC so much care has been taken to ensure it is optimal as possible for this compiler.

## Contributions

Contributions are welcome. There are several ways to contribute:

- Adding new units and symbols
- Adding new aliases
- Making issues for any bugs discovered.
- Fixing bugs
- Adding tests

There are no specific requirements for contributing but for code contributions please match the general style and provide comments with sources for any significant assertions (definition of specific unit for example).
