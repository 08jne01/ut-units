# Units

Units are defined by a `static constexpr ut::qty`. Units can be easily created by combining units mathetmatically the same way they are used at run time.

Units store the ratio between their unit and the equivalent SI base unit. For example

```cpp
// ratio := 0.3048
static constexpr double ratio = ut::foot_per_second.value;
```

This means all units store their value in SI base units under the hood this prevents uncessary conversions between different unit types.

## Defining a Unit

Defining a unit is as simple as mathematically combining existing units

```cpp
// here ut namespace is used for clarity
static constexpr qty minute                 = 60.0 * second;
static constexpr qty hour                   = 60.0 * minute;
static constexpr qty kilometre              = 1000.0 * metre;
static constexpr qty milimetre              = 1.0e-3 * metre;
static constexpr qty centimetre             = 1.0e-2 * metre;
static constexpr qty decimetre              = 0.1 * metre;
static constexpr qty newton                 = kilogram * metre / ( second * second );
```

for non-SI units they can be defined by their official ratio to the SI units

```cpp
// here ut namespace is used for clarity
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
```

If you think there should be any units added please feel free to [contribute](index.md#contributions).

## Symbols

Symbols are just aliases (constexpr copies) of their verbose unit counter parts. Below is a list of _some_ of the units defined. For a complete list it is better to check the [source file](https://github.com/08jne01/ut-units/blob/main/include/ut-units.h#L562) for the sym namespace directly. If you think there should be any symbols added please feel free to [contribute](index.md#contributions).

```cpp
namespace sym
{
    // SI Units
    // <https://en.wikipedia.org/wiki/International_System_of_Units>

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
```