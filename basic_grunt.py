import math

import pygame

from gameobject import GameObject


class BasicGruntProjectile(GameObject):

    def __init__(self, initial_position, target_position, universe):
        self.universe_ = universe

        self.damage_ = 5
        self.id_ = self.create_ID()
        self.listeners_ = []
        self.position_ = initial_position
        self.size_ = 5
        self.speed_ = 7.5
        self.velocity_ = self.calculate_trajectory(initial_position,
                                                   target_position)

    def calculate_trajectory(self, initial_position, target_position):
        x_distance = target_position[0] - initial_position[0]
        y_distance = target_position[1] - initial_position[1]
        distance = math.sqrt(x_distance**2 + y_distance**2)

        x_velocity = (x_distance * self.speed_) / distance
        y_velocity = (y_distance * self.speed_) / distance

        return x_velocity, y_velocity

    def check_collisions(self):
        collisions = self.universe_.get_collisions(self)
        if collisions:
            collisions[0].take_damage(self.damage_)
            self.report_destroyed()

    def update(self, events):
        self.check_collisions()
        self.position_ = (self.position_[0] + self.velocity_[0],
                          self.position_[1] + self.velocity_[1])

    def collision_box(self):
        return pygame.Rect(self.position_[0]-self.size_/2,
                           self.position_[1]-self.size_/2,
                           self.size_,
                           self.size_)

    """
    Listener Functions
    """

    def report_destroyed(self):
        for listener in self.listeners_:
            listener.reported_destroyed(self)

    def register(self, listeners):
        self.listeners_.append(listeners)


class BasicGrunt(GameObject):

    def __init__(self, starting_position, universe):
        self.position_ = starting_position
        self.universe_ = universe

        self.closest_range_ = 100
        self.detection_range_ = 400
        self.ID_ = self.create_ID()
        self.health_ = 30
        self.enemy_type_ = "BasicGrunt"
        self.listeners_ = []
        self.size_ = 15
        self.speed_ = 0.15
        self.weapon_delay_ = 200
        self.weapon_delay_timer_ = self.weapon_delay_
        self.id_ = self.create_ID()

    """
    Update Functions
    """

    def update(self, events):
        main_character_position = self.universe_.main_character().position_
        x_distance = main_character_position[0] - self.position_[0]
        y_distance = main_character_position[1] - self.position_[1]
        distance = (x_distance**2 + y_distance**2)**(1/2)

        if distance > self.detection_range_:
            self.position_[1] += self.speed_

        elif distance >= self.closest_range_:
            x_velocity = (x_distance * self.speed_) / distance
            y_velocity = (y_distance * self.speed_) / distance
            self.position_ = (self.position_[0]+x_velocity, 
                              self.position_[1]+y_velocity)

            if self.weapon_delay_timer_ >= self.weapon_delay_:
                if self.universe_.main_character():
                    new_projectile = BasicGruntProjectile(self.position_,
                                                          self.universe_.main_character().position_,self.universe_)
                    self.universe_.create_enemy_projectile(new_projectile)
                    self.weapon_delay_timer_ = 0
            else:
                self.weapon_delay_timer_ += 1
        else:
            self.position_ = (self.position_[0], self.position_[1])

            if self.weapon_delay_timer_ >= self.weapon_delay_:
                if self.universe_.main_character():
                    new_projectile = BasicGruntProjectile(self.position_,
                                                          self.universe_.main_character().position_,self.universe_)
                    self.universe_.create_enemy_projectile(new_projectile)
                    self.weapon_delay_timer_ = 0
            else:
                self.weapon_delay_timer_ += 2

    """
    Access Functions
    """

    def collision_box(self):
        return pygame.Rect(self.position_[0]-self.size_/2,
                           self.position_[1]-self.size_/2,
                           self.size_,
                           self.size_)

    def take_damage(self, damage):
        if damage <= 0:
            print("WARNING BasicGrunt " + str(self.id_) +
                  " taking " + str(damage) + " damage")
            print("Disregarding non positive damage")
        elif damage >= self.health_:
            self.report_destroyed()
        else:
            self.health_ -= damage

    def get_type(self):
        return self.enemy_type_

    """
    Listener Functions
    """

    def report_destroyed(self):
        for listener in self.listeners_:
            listener.reported_destroyed(self)

    def register(self, listeners):
        self.listeners_.append(listeners)
        pass
