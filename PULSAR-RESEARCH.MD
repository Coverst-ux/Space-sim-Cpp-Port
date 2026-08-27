# Pulsar Research

## 1. What is a pulsar?
- What physical object is it? rotating magnetized Neutron Star
- Why does it produce pulses? The rotating radiation that causes the beam to sweep through sapce
- What is actually rotating? The Neutron Star
- What makes the beam observable? The radiation emitted in beams associated with magnetic poles

## 2. Important geometry
- Rotation axis: The imaginary axis on which the star rotates. Represented by angular_speed, rotation_axis, and then multiplying them to get angular_velocity (direction and speed).

- Magnetic axis: The direction of the neutron star's magnetic dipole. Represented by magnetic_axis.

- Magnetic inclination/tilt: The angle between the rotation axis and the magnetic axis. Alpha = 0: aligned rotator. Alpha between 0 and 90: tilted rotator. Alpha = 90: perpendicular rotator

- Observer direction: A vector from the pulsar toward the observer's line of sight. Represented by Nobserver

- Why pulses appear: As the pulsar rotates, its radiation beams sweep through space, periodically crossing the observer's line of sight.

## 3. Parameters and units
- R: stellar radius, measured in meters, preset/advanced input only.
- P: rotation period, measured in seconds, normal user input.
- omega: angular speed, measured in radians per second, calculated from P.
- polar_field_strength(Bp): polar surface magnetic field strength, measured in tesla, User input/preset.
- c: speed of light, measured in meters per second. Fixed constant.
- RLC: light cylinder radius, measured in meters, calculated from c and omega.
- alpha: angle between rotation and magnetic axes, measured in radians, user input.
- magnetic_axis: unit vector giving the magnetic axis direction, calculated as the pulsar rotates.
- observer_direction: unit vector pointing from the pulsar to the observer, set from camera position.


## 4. Light cylinder
- Physical meaning: Boundary surrounding a pulsar where the rotational speed of the star's magnetic field lines equal the speed of light.
- Equation: c/omega
- Assumptions: Constant angular speed, the magnetosphere corotates with it strictly, speed of light in a vacuum.

## 5. Magnetic-field model
- What is a magnetic dipole? A simplified magnetic field source with two opposite magnetic poles. Its direction is represented by the magnetic axis unit vector.
- What inputs determine the field at a point? The position relative to the star's center, stellar radius, polar field strength, and magnetic axis direction.
- What does the calculation return? 3D vector "B", measured in tesla, provides the field's strength and direction
- How does field strength change with distance? Increasing the distance by a factor of x reduces the field strength to 1/x^3 of its original value. For example, doubling the distance reduces it to 1/8.
- Polar versus equatorial field convention: The magnetic field strength differs across the star's surface. For an ideal dipole, the field at the magnetic equator is half as strong as the field at the magnetic poles.
- Static dipole versus rotating dipole: Static dipole has a fixed magnetic axis direction, while the rotating dipole changes direction over time. The first implementation will use a static diple and the final visualization will convert to a rotating dipole by changing the axis direction over time.
- Model chosen for the first implementation: A static, centred vacuum dipole. It calculates the magnetic-field vector at one exterior position using the stellar radius, polar field strength, magnetic-axis direction and position relative to the star’s centre.
- Why this simplified model is sufficient: It is the bare minimum needed by later features/implementations. It lets me verify that the field has the correct direction, the polar surface strength equals polar field strength, the equatorial magnetic strength is half the polar stength, the field strength decreases by 1/r^3, and that the calculation works at 3D positions

## 6. Features intentionally postponed
- Spin-down
- Dipole radiation
- Particle acceleration
- Relativistic plasma/magnetosphere
- Radiation beams

## 7. Sources

- “Chapter 6: Pulsars” — National Radio Astronomy Observatory
  https://www.cv.nrao.edu/~sransom/web/Ch6.html
  Used for pulsar geometry, magnetic fields and the simplified dipole model.

- “Timing an X-ray Pulsar” — NASA HEASARC
  https://heasarc.gsfc.nasa.gov/docs/xte/learning_center/xray_pulsar.html
  Used for the basic explanation of neutron stars, pulsars and observable pulses.

- “Biot–Savart Law, Magnetic Dipoles, Vector Potential” — Michigan State University
  https://web.pa.msu.edu/people/duxbury/courses/phy481/Fall2009/Lecture18.pdf
  Used for the magnetic-dipole vector equation.

- “Pulsar Electrodynamics” — Goldreich and Julian
  https://adsabs.harvard.edu/full/1969ApJ...157..869G
  Used for the light cylinder, corotation and open field lines.