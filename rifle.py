import pygame
from wordgenerator import WordGenerator

pygame.font.init()

BLACK = 0, 0, 0
RED = 255, 0, 0
GREEN = 0, 255, 0
BLUE = 0, 0, 255
WHITE = 255, 255, 255
EVENT_KEY_a = 97
EVENT_KEY_z = 122
EVENT_KEY_ENTER = 13
EVENT_KEY_BACKSPACE = 8


class RifleTargetingSystem():

    def __init__(self, universe, DRAWING_SCALE):
        self.word_generator_ = WordGenerator()
        self.universe_ = universe
        self.DRAWING_SCALE_ = DRAWING_SCALE
        self.ui_font_ = pygame.font.SysFont("monospace", 10*DRAWING_SCALE)
        self.enemy_color_ = RED
        self.main_character_color_ = GREEN
        self.target_tags_ = dict()
        self.current_text_ = ""

    def update(self, events):
        for enemy in self.universe_.enemies():
            if enemy.ID_ not in self.target_tags_:
                self.target_tags_[enemy.ID_] = self.new_word()

        for event in events:
            if event.type == pygame.KEYDOWN:
                if event.key == EVENT_KEY_ENTER:
                    self.current_text_ = ""
                if event.key == EVENT_KEY_BACKSPACE:
                    self.current_text_ = self.current_text_[:-1]
                elif event.key in range(EVENT_KEY_a, EVENT_KEY_z+1):
                    self.current_text_ += event.unicode.lower()

    def draw(self, screen):
        self.draw_background(screen)
        self.draw_entities(screen)
        self.draw_target_tags(screen)
        self.draw_terminal(screen)

    def draw_background(self, screen):
        pygame.draw.rect(screen, BLACK, pygame.Rect((0,0), screen.get_size()))

    def draw_entities(self, screen):
        for enemy in self.universe_.enemies():
            enemy_rect = pygame.Rect((enemy.position_[0]-enemy.size_/2) * self.DRAWING_SCALE_ ,
                                     (enemy.position_[1]-enemy.size_/2) * self.DRAWING_SCALE_ ,
                                     enemy.size_ * self.DRAWING_SCALE_,
                                     enemy.size_ * self.DRAWING_SCALE_)
            pygame.draw.rect(screen, self.enemy_color_, enemy_rect)

        main_character = self.universe_.main_character_
        main_character_rect = pygame.Rect((main_character.position_[0]-main_character.size_/2) * self.DRAWING_SCALE_,
                                          (main_character.position_[1]-main_character.size_/2) * self.DRAWING_SCALE_,
                                          main_character.size_ * self.DRAWING_SCALE_,
                                          main_character.size_ * self.DRAWING_SCALE_)
        pygame.draw.rect(
            screen, self.main_character_color_, main_character_rect)

    def draw_target_tags(self, screen):
        for enemy in self.universe_.enemies():
            target_tag_word = self.target_tags_[enemy.ID_]
            target_tag_label = self.ui_font_.render(
                target_tag_word, 1, self.enemy_color_)
            width = self.ui_font_.size(target_tag_word)[0]
            screen.blit(target_tag_label, 
                        (enemy.position_[0] * self.DRAWING_SCALE_ - (width/2),
                        (enemy.position_[1] * self.DRAWING_SCALE_ + enemy.size_/2) + (5 * self.DRAWING_SCALE_)))

    def draw_terminal(self, screen):
        terminal_rect = pygame.Rect(10 * self.DRAWING_SCALE_, screen.get_height()-(40* self.DRAWING_SCALE_),
                                    400 * self.DRAWING_SCALE_, 25 * self.DRAWING_SCALE_)
        pygame.draw.rect(screen, BLUE, terminal_rect)
        pygame.draw.rect(screen, WHITE, terminal_rect, 2 * self.DRAWING_SCALE_)

        text_label = self.ui_font_.render(self.current_text_, 1, WHITE)
        screen.blit(text_label, (15 * self.DRAWING_SCALE_, screen.get_height()-(30 * self.DRAWING_SCALE_)))

    def new_word(self):
        return self.word_generator_.request_word(3,3)


class Rifle():

    def __init__(self, universe, DRAWING_SCALE):
        self.universe_ = universe
        self.DRAWING_SCALE_ = DRAWING_SCALE
        self.NAME_ = "Rifle"
        self.targeting_system = RifleTargetingSystem(universe, DRAWING_SCALE)

    def update(self, events):
        self.targeting_system.update(events)

    def draw(self, screen):
        self.targeting_system.draw(screen)
