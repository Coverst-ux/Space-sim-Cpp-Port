# Stardance extension

**The original C++ physics core existed before this Stardance project. Work on the
pulsar extension began on August 26, 2026.**

## Demo Website

**Live demo:** [Try Space Sim](https://coverst-ux.github.io/space-sim-site/)

**Demo website source code:** [View the website source](https://github.com/Coverst-ux/space-sim-site)

**The source code for the demo website is under demo_website in this repository**
## New work 

## Pulsar rotation-period calculations

A rotation period is the amount of time it takes for the pulsar to complete one full rotation. I added a rotation period calculation to convert the pulsar's angular velocity into the time required for one full rotation. The formula for this is 
    
$P = \frac{2\pi}{\Omega}$ 
    
I also added a negative value/zero guard since those values would make the calculation completely invalid.
    
## Light-cylinder calculations
The light cylinder is the distance from the pulsar's rotation axis where something rotating with the star would have to move at the speed of light

$R_{LC} = \frac{c}{\Omega}$

- $R_{LC}$ → light-cylinder radius
- $c$ → speed of light
- $\Omega$ → angular velocity

The formula divides the speed of light by the pulsar's angular velocity. A faster spinning pulsar has a smaller light cylinder while a slower pulsar has a bigger light cylinder

## Static magnetic dipole field model
 The magnetic dipole field approximates the pulsar's magnetic field around a neutron star. It uses the star's radius, polar magnetic field strength, magnetic axis direction, and the position that is currently being given to calculate the magnetic field at that point

 $$\mathbf{B}(\mathbf{p}) =B_p \left(\frac{R}{d}\right)^3\left(3(\hat{\mathbf{m}}\cdot\hat{\mathbf{p}})\hat{\mathbf{p}}-\hat{\mathbf{m}}\right)$$

 That's the formula to calculate the magnetic dipole. Looks scary, I know. In reality, it's pretty simple

- **$B_p$** → polar magnetic field strength
- **$R$** → radius of the star
- **$d$** → distance from the star's center
- **$\hat{\mathbf{p}}$** → normalized position vector
- **$\hat{\mathbf{m}}$** → normalized magnetic-axis vector


## Pulsar tests and research notes
Before connecting everything to the visualization, I wrote tests for the main pulsar calculations. I tested the rotation period, light-cylinder radius, magnetic field direction at known positions, and guards for invalid values.

A lot of the work also came from researching how pulsars actually work, especially the difference between the rotation axis and magnetic axis, magnetic tilt, dipole fields, and the light cylinder. I kept the model intentionally simplified and left out things like plasma physics, spin-down, and a full relativistic magnetosphere.

## Python pulsar visualization
The visualization is written in Python with Pygame, while the C++ module handles the pulsar calculations. I used the magnetic field calculations to trace field lines from seed points around the star, then added a rotating and tilted magnetic axis with beams drawn in both directions

I also added camera rotation, zoom, adjustable rotation speed and magnetic tilt, field-line depth fading, and occlusion so the visualization reads better in 3D. The rendering ended up taking much longer than I expected because getting the field lines and beams to look right without external assets was surprisingly harder than implementing the basic physics
