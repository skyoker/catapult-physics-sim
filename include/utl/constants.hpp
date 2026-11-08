#pragma once

// --- Gravitational Constants ---
#define G_AU 39.47841760435743f  // 4π² in AU³/(M☉·yr²)
#define G_SI 6.67430e-11f        // m³/(kg·s²)

// --- Unit Conversions ---
#define AU_TO_M 1.496e11f

// --- Coulomb Constants ---
#define K_COULOMB 8.9875517923e9
#define CHARGE_COLOMB 1.602e-19f
#define AMU_TO_KG 1.66053906660e-27
#define COULOMB_SCALE 1e-5f

// --- Masses (AU System) in solar masses (M☉ = 1.989e30 kg) ---
#define SUN_MASS_AU 1.0f
#define MERCURY_MASS_AU 3.285e23f / 1.989e30f  // 1.651e-7
#define VENUS_MASS_AU 4.867e24f / 1.989e30f     // 2.447e-6
#define EARTH_MASS_AU 5.972e24f / 1.989e30f     // 3.003e-6
#define MOON_MASS_AU 7.342e22f / 1.989e30f      // 3.692e-8
#define MARS_MASS_AU 6.417e23f / 1.989e30f      // 3.227e-7
#define JUPITER_MASS_AU 1.898e27f / 1.989e30f   // 9.544e-4
#define SATURN_MASS_AU 5.683e26f / 1.989e30f    // 2.858e-4
#define URANUS_MASS_AU 8.681e25f / 1.989e30f    // 4.366e-5
#define NEPTUNE_MASS_AU 5.151e-5f
#define PLUTO_MASS_AU 6.582e-9f

// --- Masses (SI) in kg ---
#define SUN_MASS_KG 1.989e30f
#define MERCURY_MASS_KG 3.285e23f
#define VENUS_MASS_KG 4.867e24f
#define EARTH_MASS_KG 5.972e24f
#define MOON_MASS_KG 7.342e22f
#define MARS_MASS_KG 6.417e23f
#define JUPITER_MASS_KG 1.898e27f
#define SATURN_MASS_KG 5.683e26f
#define URANUS_MASS_KG 8.681e25f
#define NEPTUNE_MASS_KG 1.024e26f
#define PLUTO_MASS_KG 1.309e22f

// --- Radii (AU System) ---
#define SUN_RADIUS_AU 0.004654f
#define MERCURY_RADIUS_AU 1.631e-5f
#define VENUS_RADIUS_AU 4.045e-5f
#define EARTH_RADIUS_AU 4.259e-5f
#define MOON_RADIUS_AU 1.161e-5f
#define MARS_RADIUS_AU 2.266e-5f
#define JUPITER_RADIUS_AU 4.673e-4f
#define SATURN_RADIUS_AU 3.892e-4f
#define URANUS_RADIUS_AU 1.696e-4f
#define NEPTUNE_RADIUS_AU 1.646e-4f
#define PLUTO_RADIUS_AU 7.946e-6f

#define MOON_ORBIT_RADIUS_AU 0.002569f

// --- Radii (SI) in meters ---
#define SUN_RADIUS_M 6.9634e8f
#define MERCURY_RADIUS_M 2.4397e6f
#define VENUS_RADIUS_M 6.0518e6f
#define EARTH_RADIUS_M 6.371e6f
#define MOON_RADIUS_M 1.7374e6f
#define MARS_RADIUS_M 3.3895e6f
#define JUPITER_RADIUS_M 6.9911e7f
#define SATURN_RADIUS_M 5.8232e7f
#define URANUS_RADIUS_M 2.5362e7f
#define NEPTUNE_RADIUS_M 2.4622e7f
#define PLUTO_RADIUS_M 1.1883e6f

#define MOON_ORBIT_RADIUS_M 3.844e8f

// --- Earth Surface Gravity ---
#define EARTH_GRAVITY 9.81f
