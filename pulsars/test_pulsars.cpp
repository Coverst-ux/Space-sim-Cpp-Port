#include "pulsars.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

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

    std::cout << "All pulsar configuration tests passed.\n";
    return 0;
}