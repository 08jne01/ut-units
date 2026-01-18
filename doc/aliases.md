# Aliases

While `ut::qty` is the main type aliases are used to provide a readable interface for specific sets of dimensions. Below are aliases the 7 base dimensions + dimensionless

```cpp
template<std::floating_point T> using dimensionless = qty<T,qty_dimensions<>>;
template<std::floating_point T> using time          = qty<T,qty_dimensions<1>>;
template<std::floating_point T> using length        = qty<T,qty_dimensions<0,1>>;
template<std::floating_point T> using mass          = qty<T,qty_dimensions<0,0,1>>;
template<std::floating_point T> using current       = qty<T,qty_dimensions<0,0,0,1>>;
template<std::floating_point T> using temperature   = qty<T,qty_dimensions<0,0,0,0,1>>;
template<std::floating_point T> using amount        = qty<T,qty_dimensions<0,0,0,0,0,1>>;
template<std::floating_point T> using luminosity    = qty<T,qty_dimensions<0,0,0,0,0,0,1>>;
```

for an understanding of the qty and qty_dimensions see their [relevant sections](quantity.md#qty)

## Using Aliases

Here we will calculate force require for a given acceleration from velocity change and time as an example.

```cpp
ut::speed<double> v0 = 1.0 * ut::metre_per_second;
ut::speed<double> v1 = 10.0 * ut::foot_per_second;

ut::time<double> t = 0.1 * ut::minutes;

ut::speed<double> dV = v1 - v0; // 2.048 m/s
ut::acceleration<double> a = dV / t; // 0.341 m/s^2

ut::mass<double> mass = 24.0 * ut::pound; // 

ut::force<double> force = mass * a;

double force_in_N = a.in(ut::newton); // 3.71
double force_in_lbf = a.in(ut::pound_force); // 0.83

```

Aliases are preferable to using `ut::qty` as with `ut::qty` you either have to specify dimensions manually or use template type deduction which can result in an unintended unit type. Using an alias constrains the unit the intended dimensions and indicates to the reader what the author intended.

## Adding Aliases

Adding aliases is very simple, the only requirement is an existing unit for the desired alias. Below are some of the aliases for the non-fundamental types

```cpp
// in ut namespace for clarity
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
```

If you think there should be an alias added please feel free to [contribute](index.md#contributions).
