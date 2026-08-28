#include "pulsars.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

PulsarConfig config{
    1.0,
    1.0,
    1.0,
    0.1,
    vector3d{0.0, 0.0, 1.0}
};

bool approximately_equal(double actual, double expected){
    constexpr double tolerance = 1e-10;
    return std::abs(actual-expected) < tolerance; 
}

template <typename Function>
void expect_invalid_argument(Function function){
    bool exception_thrown = false;

    try{
        function();
    }
    catch (const std::invalid_argument&) {
        exception_thrown = true;
    }

    assert(exception_thrown);
}

int main() {
    // known normalized configuration

    assert(approximately_equal(rotation_period(0.1), 62.83185307179865));

    assert(approximately_equal(light_cylinder_radius(1.0, 0.1), 10));

    assert(approximately_equal(rotation_period(2.0), std::acos(-1.0)));

    assert(approximately_equal(light_cylinder_radius(1.0, 2.0), 0.5));


    expect_invalid_argument([] {
        rotation_period(0.0);
    });

    expect_invalid_argument([] {
        rotation_period(-0.1);
    });

    expect_invalid_argument([] {
        light_cylinder_radius(1.0, 0.0);
    });

    expect_invalid_argument([]{
        light_cylinder_radius(1.0, -0.1);
    });

    
// North magnetic pole
{
    vector3d position{0.0, 0.0, config.stellar_radius};
    vector3d actual = get_magnetic_field(config, position);

    assert(
        approximately_equal(actual.x, 0.0) &&
        approximately_equal(actual.y, 0.0) &&
        approximately_equal(actual.z, config.polar_field_strength)
    );
}

// Magnetic equator
{
    vector3d position{config.stellar_radius, 0.0, 0.0};
    vector3d actual = get_magnetic_field(config, position);

    assert(
        approximately_equal(actual.x, 0.0) &&
        approximately_equal(actual.y, 0.0) &&
        approximately_equal(
            actual.z,
            -config.polar_field_strength / 2.0
        )
    );
}

// Twice the polar distance, field should be 1/8 as strong
{
    vector3d position{
        0.0,
        0.0,
        config.stellar_radius * 2.0
    };

    vector3d actual = get_magnetic_field(config, position);

    assert(
        approximately_equal(actual.x, 0.0) &&
        approximately_equal(actual.y, 0.0) &&
        approximately_equal(
            actual.z,
            config.polar_field_strength / 8.0
        )
    );
}

// The dipole field is undefined at the pulsar's center
expect_invalid_argument([&] {
    vector3d position{0.0, 0.0, 0.0};
    get_magnetic_field(config, position);
});

// This implementation models only the exterior field
expect_invalid_argument([&] {
    vector3d position{
        0.0,
        0.0,
        config.stellar_radius * 0.5
    };

    get_magnetic_field(config, position);
});

// A zero vector cannot define a magnetic-axis direction
expect_invalid_argument([&] {
    PulsarConfig invalid_config = config;
    invalid_config.magnetic_axis = vector3d{0.0, 0.0, 0.0};

    vector3d position{
        0.0,
        0.0,
        invalid_config.stellar_radius
    };

    get_magnetic_field(invalid_config, position);
});

    std::cout << "All pulsar tests passed.\n";
    return 0;


}