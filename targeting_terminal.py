import pygame

pygame.font.init()

# CONSTANTS
BLACK = 0, 0, 0
BLUE = 0, 0, 255
WHITE = 255, 255, 255
EVENT_KEY_a = 97
EVENT_KEY_z = 122
EVENT_KEY_ENTER = 13
EVENT_KEY_BACKSPACE = 8

class TargetingTerminal():
    
    def __init__(self, DRAWING_SCALE): 
        self.fontSize_ = 15
        self.terminalRectX_ = 10
        self.terminalRectY_ = 40
        self.terminalWidth_ = 400
        self.terminalHeight_ = 25
        self.maxTextWidth_ = 380
        self.borderRectWidth_ = 2
        self.textAlias_ = 1
        self.textX_ = 15
        self.textY_ = 15

        self.current_text_ = ""
        self.ui_font_ = pygame.font.SysFont("monospace", fontSize*DRAWING_SCALE)
        self.DRAWING_SCALE_ = DRAWING_SCALE
        self.terminal_rect_ = None

    def update(self , events):
        for event in events:
            if event.type == pygame.KEYDOWN:
                textWidth = self.ui_font_.size(self.current_text_)[0]
                if event.key == EVENT_KEY_ENTER:
                    self.current_text_ = ""
                if event.key == EVENT_KEY_BACKSPACE:
                    self.current_text_ = self.current_text_[:-1]
                elif event.key in range(EVENT_KEY_a, EVENT_KEY_z) and textWidth < maxTextWidth*self.DRAWING_SCALE_:
                    self.current_text_ += chr(event.key)

    def draw_terminal(self, screen):
        if not self.terminal_rect_:
            self.terminal_rect_ = pygame.Rect(terminalRectX*self.DRAWING_SCALE_, screen.get_height()-terminalRectY*self.DRAWING_SCALE_, 
                                              terminalWidth*self.DRAWING_SCALE_, terminalHeight*self.DRAWING_SCALE_)
        
        pygame.draw.rect(screen, BLUE,self.terminal_rect_)
        pygame.draw.rect(screen, WHITE, self.terminal_rect_, borderRectWidth*self.DRAWING_SCALE_)
    
        text_label = self.ui_font_.render(self.current_text_, textAntialias, WHITE)
        screen.blit(text_label, (textX * self.DRAWING_SCALE_, screen.get_height()-(textY * self.DRAWING_SCALE_)))
