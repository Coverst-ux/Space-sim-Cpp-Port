#include "pulsars.h"

#include <iomanip>
#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

PulsarConfig config{
    1.0,
    1.0,
    1.0,
    0.1,
    0.0,
    vector3d{0.0, 0.0, 1.0},
    1.4 * 1.9885e30,
    0.0
};



constexpr double PI = 3.14159265358979;

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


    PulsarConfig realistic_config = config;

    realistic_config.stellar_radius = 12000.0;
    realistic_config.speed_of_light = 299792458.0;
    realistic_config.polar_field_strength = 1e8;
    realistic_config.omega = 2.0 * PI;
    realistic_config.alpha = PI / 4.0;
    realistic_config.mass = 1.4 * 1.9885e30;
    realistic_config.phase = 0.0;


    {
        double rate = spin_down_rate(realistic_config);
        double power = dipole_radiation_power(realistic_config);

        std::cout << "Realistic pulsar spin-down: "
                << rate << " rad/s^2\n";

        std::cout << "Realistic pulsar radiation power: "
                << power << " W\n";

        assert(rate < 0);
        assert(power > 0);
    }
    {
        PulsarConfig test_config = realistic_config;

        double omega_before = test_config.omega;

        update_pulsar(test_config, 86400.0 * 365.25);

        std::cout << std::setprecision(17);

        std::cout << "Omega before: " << omega_before << "\n";
        std::cout << "Omega after:  " << test_config.omega << "\n";
        std::cout << "Change:       "
                << test_config.omega - omega_before << "\n";

        assert(test_config.omega < omega_before);
    }
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

// Magnetic axis tilted 90 degrees should start along +x
{
    vector3d actual = rotating_magnetic_axis(PI / 2.0, 0.0);

    assert(
        approximately_equal(actual.x, 1.0) &&
        approximately_equal(actual.y, 0.0) &&
        approximately_equal(actual.z, 0.0)
    );
}

// After rotating pi/2 radians, the axis should point along +y
{
    vector3d actual = rotating_magnetic_axis(PI / 2.0, PI / 2.0);

    assert(
        approximately_equal(actual.x, 0.0) &&
        approximately_equal(actual.y, 1.0) &&
        approximately_equal(actual.z, 0.0)
    );
}



{
    PulsarConfig test_config = config;
    test_config.alpha = PI / 4.0;

    double rate = spin_down_rate(test_config);
    assert(rate < 0);
}

    {
        PulsarConfig test_config = config;
        test_config.mass = 0;

        bool threw = false;

        try {
            spin_down_rate(test_config);
        }
        catch (const std::invalid_argument&) {
            threw = true;
    }

    assert(threw);
    }


    {
        PulsarConfig test_config = config;
        test_config.omega = 0;

        assert(spin_down_rate(test_config) == 0);

    }

    
    {
        PulsarConfig test_config = config;
        test_config.stellar_radius = 0;

        bool threw = false;

        try {
            spin_down_rate(test_config);
        }
        catch (const std::invalid_argument&) {
            threw = true;
    }

    assert(threw);
    }


    {
        PulsarConfig test_config = config;
        test_config.speed_of_light = 0;

        bool threw = false;

        try {
            spin_down_rate(test_config);
        }
        catch (const std::invalid_argument&) {
            threw = true;
    }

    assert(threw);
    }


    {
        PulsarConfig test_config = config;
        test_config.omega = -1;

        bool threw = false;

        try {
            spin_down_rate(test_config);
        }
        catch (const std::invalid_argument&) {
            threw = true;
    }

    assert(threw);
    }

// Radiation / spin-down tests need a non-zero magnetic tilt
PulsarConfig radiation_config = config;
radiation_config.alpha = PI / 4.0;


// Energy-loss consistency: P = -I * omega * domega/dt
{
    double loss = dipole_radiation_power(radiation_config);

    double moment_of_inertia =
        0.35 * radiation_config.mass *
        pow(radiation_config.stellar_radius, 2);

    assert(approximately_equal(
        loss,
        -moment_of_inertia
        * radiation_config.omega
        * spin_down_rate(radiation_config)
    ));
}


// A tilted, rotating pulsar should radiate energy
{
    double loss = dipole_radiation_power(radiation_config);
    assert(loss > 0);
}


// No rotation -> no radiation
{
    PulsarConfig test_config = radiation_config;
    test_config.omega = 0;

    assert(dipole_radiation_power(test_config) == 0);
}


// No magnetic tilt -> no radiation
{
    PulsarConfig test_config = radiation_config;
    test_config.alpha = 0;

    assert(dipole_radiation_power(test_config) == 0);
}


// P scales with omega^4
{
    PulsarConfig test_config = radiation_config;

    double loss_undoubled =
        dipole_radiation_power(test_config);

    test_config.omega *= 2;

    double loss_doubled =
        dipole_radiation_power(test_config);

    assert(approximately_equal(
        loss_doubled / loss_undoubled,
        16.0
    ));
}


// P scales with B^2
{
    PulsarConfig test_config = radiation_config;

    double loss_undoubled =
        dipole_radiation_power(test_config);

    test_config.polar_field_strength *= 2;

    double loss_doubled =
        dipole_radiation_power(test_config);

    assert(approximately_equal(
        loss_doubled / loss_undoubled,
        4.0
    ));
}
    {
        PulsarConfig test_config = config;
        test_config.phase = 0.0;
        test_config.omega = 2.0;

        update_phase(test_config, 0.5);
        assert(approximately_equal(test_config.phase, 1.0));
    }

    {
        PulsarConfig test_config = config;
        test_config.alpha = PI / 4.0;
        test_config.phase = 0.0;
        test_config.mass = 1e12;

        double omega_before = test_config.omega;

        update_pulsar(test_config, 0.5);

        assert(test_config.phase > 0.0);
        assert(test_config.omega < omega_before);
    }

    std::cout << "All pulsar tests passed.\n";
    return 0;

}