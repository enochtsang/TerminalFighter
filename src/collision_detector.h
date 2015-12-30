#pragma once 
#include <vector>

class CollisionDetector : public I_CollisionDetector {
public:
    CollisionDetector();

    void check_friendly_sensor_collisions();
    void check_enemy_sensor_collisions();
    void check_friendly_projectile_collisions();
    void check_enemy_projectile_collisions();
    void check_friendly_ship_collisions();
    void check_enemy_ship_collisions();

private:
    check_one_way_collisions(std::vector<Collidable*> setOne, 
                             std::vector<Collidable*> setTwo);

    check_two_way_collisions(std::vector<Collidable*> setOne, 
                             std::vector<Collidable*> setTwo);

    std::vector<Sensor*> friendly_sensors_;
    std::vector<Sensor*> enemy_sensors_;
    std::vector<Projectile*> friendly_projectiles_;
    std::vector<Projectile*> enemy_projectiles_;
    std::vector<Ship*> friendly_ships_;
    std::vector<Ship*> enemy_ships_;
}