import pygame

class MainCharacter():

    def __init__(self, starting_position):
        self.colour_ = 0, 255, 0
        self.position_ = starting_position
        self.size_ = 50
    def draw(self, screen):
        visual_rect = pygame.Rect(self.position_[0]-self.size_/2, self.position_[1]-self.size_/2, 
                                       self.size_, self.size_)
        pygame.draw.rect(screen, self.colour_, visual_rect)

    def update(self, position=None):
        if position:
           self.position_ = position  