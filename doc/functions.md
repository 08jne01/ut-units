# Functions

There are lots of operators and some free functions which are compatible with `ut::qty`.

There are several helper structs which provide information for return types

| type | description
|------|-------------
| `qty_multiply<scalar_t, dim_left, dim_right>` | qty result multiply of `dim_left` and `dim_right`
| `qty_divide<scalar_t, dim_left, dim_right>` | qty result divide of `dim_left` by `dim_right`
| `qty_sqrt<scalar_t, dim>` | qty result sqrt of `dim`
| `qty_pow<scalar_t, dim>` | qty result pow of `dim`
| `qty_offset_to_qty<qty_offset>` | qty result conversion from `qty_offset`

Simplified syntax (pseudocode) is used for function descriptions to aid in reading.

## Operators

Operators which multiply or divide with a quantity in both operands produce a corresponding quantity given by the helper struct, for example operator* uses `qty_multiply`. Operators which have a quantity for both operands and do not divide require that `compatible_qty<left,right>` is true. This concept is defined as follows:

```cpp
template<typename T1, typename T2>
concept compatible_qty = requires() {
    requires qty_type<T1>;
    requires qty_type<T2>;
    requires std::same_as<typename T1::type, typename T2::type>;
    requires same_dimensions<typename T1::dimensions, typename T2::dimensions>::value;
};
```

## Free Functions

### sqrt

```cpp
qty_sqrt<scalar_t, qty_type::dimensions> sqrt( qty_type quantity );
```

returns ```sqrt(quantity)``` if it is square-rootable otherwise compile error

### pow

```cpp
template<int N>
qty_pow<scalar_t, qty_type::dimensions> pow( qty_type quantity );
```

returns ```quantity^N```

### abs

```cpp
type abs( type quantity );
```

returns absolute value of `type`, `type` can be either scalar or quantity

### copysign

```cpp
type_number copysign(type_number number, type_sign sign);
```

returns copies sign of `sign` onto `number`, `type_number` and `type_sign` can one of scalar or quantity
