add_unit_test(arkanoid_sim_test
    SOURCES
        SimTest.cpp
    DEPENDENCIES
        arkanoid_sim
)

add_unit_test(arkanoid_physics_test
    SOURCES
        PhysicsTest.cpp
    DEPENDENCIES
        arkanoid_sim
)

add_unit_test(arkanoid_sim_property_test
    SOURCES
        SimPropertyTest.cpp
    DEPENDENCIES
        arkanoid_sim
)

add_unit_test(arkanoid_powerup_test
    SOURCES
        PowerUpTest.cpp
    DEPENDENCIES
        arkanoid_sim
)
