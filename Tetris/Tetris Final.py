import pygame
import random
import sys
from pygame import *
from pygame.locals import *

#GridObjects that store color information of block.
class GridObject():
    def __init__(self, colordata):
        self.color = colordata
        self.x = None
        self.y = None
        self.identifier = 0.0

def BUGFIX(shape):
    outer_nest = []
    middle_nest = []
    inner_nest = []
    for i in range(len(shape)):
        middle_nest = []
        for j in range(len(shape[i])):
            inner_nest = []
            for k in range(len(shape[i][j])):
                inner_nest.append(shape[i][j][k])
            middle_nest.append(inner_nest)
        outer_nest.append(middle_nest)
    return outer_nest
            
    
# Block Object
class block():
    def __init__(self, shape, randomizer):
        self.shape = BUGFIX(shape)
        self.orientation = self.shape[0]
        color = random.choice(colorlist[:])
        self.color = color
        for i in range(len(self.shape)):
            for j in range(len(self.shape[i])):
                for k in range(len(self.shape[i][j])):
                    if self.shape[i][j][k] == 1:
                        self.shape[i][j][k] = GridObject(color)
                    elif self.shape[i][j][k] == 0:
                        self.shape[i][j][k] = GridObject(None)
                    self.shape[i][j][k].identifier = randomizer
                    self.shape[i][j][k].x = k + 4
                    self.shape[i][j][k].y = j
        if len(self.shape[1][0]) == len(straight_shape[1][0]):
            for m in range(len(self.shape[1][0])):
                self.shape[1][0][m].y += 3
                    


    def rotate_CW(self, game):
        index = self.shape.index(self.orientation)
        firstcheck = True
        for i in range(len(self.shape)):
            for j in range(len(self.shape[i])):
                if any(t.x == 9 for t in self.shape[i][j]) and any(t.x == 0 for t in self.shape[i][j]):
                    firstcheck = False
                    
        if firstcheck and rotationcheck1(game) and rotationcheck2(game):
            movementdeletion(game)
            if index == len(self.shape) - 1:
                self.orientation = self.shape[0]
            else:
                self.orientation = self.shape[index + 1]
            boardoverwrite(game)

    def rotate_CCW(self, game):
        index = self.shape.index(self.orientation)
        firstcheck = True
        for i in range(len(self.shape)):
            for j in range(len(self.shape[i])):
                if any(t.x == 9 for t in self.shape[i][j]) and any(t.x == 0 for t in self.shape[i][j]):
                    firstcheck = False
        if firstcheck and rotationcheck1(game) and rotationcheck2(game):
            
            movementdeletion(game)
            if index == 0:
                self.orientation = self.shape[len(self.shape)-1]
            else:
                self.orientation = self.shape[index - 1]
            boardoverwrite(game)

##SHAPE TEMPLATES AND MASTER VARIABLES FOR DRAWING/OTHER THINGS
Box_size = 40
colorlist = [
    (255, 0, 0),
    (255, 128, 0),
    (255, 255, 0),
    (0, 255, 0),
    (0, 255, 255),
    (0, 0, 255),
    (255, 0, 255),
    (255, 0 , 127)]



square = [[[1, 1],
           [1, 1]],
          
          [[1, 1],
           [1, 1]]]

straight_shape = [[[1],
                   [1],
                   [1],
                   [1]],

                  [[1, 1, 1, 1]]]

s_shape = [[[1, 0],
            [1, 1],
            [0, 1]],
           
           [[0, 1, 1],
            [1, 1, 0]],]


z_shape = [ [[0, 1],
             [1, 1],
             [1, 0]],

          [[1, 1, 0],
           [0, 1, 1]]]

            

L_shape = [[[1, 0],
           [1, 0],
           [1, 1]],

         [[1, 1, 1],
          [1, 0, 0]],

         [[1, 1],
          [0, 1],
          [0, 1]],

         [[0, 0, 1],
          [1, 1, 1]]]

J_shape = [[[0, 1],
           [0, 1],
           [1, 1]],

         [[1, 0, 0],
          [1, 1, 1]],

         [[1, 1],
          [1, 0],
          [1, 0]],

         [[1, 1, 1],
          [0, 0, 1]]]

t_shape = [[[0, 1],
            [1, 1],
            [0, 1]],
        
           [[0, 1, 0],
           [1, 1, 1]],

           [[1, 0],
            [1, 1],
            [1, 0]],

         [[1, 1, 1],
          [0, 1, 0]]]


shape_list = [square, straight_shape,\
              z_shape, s_shape, L_shape, J_shape, t_shape]

##Tetris Object
##Contains game variables

o = 0

class TetrisOBJ:
    def __init__(self, width, height, FPS):
        window = width, height
        self.size = window
        self.frames = FPS
        self.score = 0
        self.tetriscount = 0
        self.clock = pygame.time.Clock()
        self.activepiece = None
        self.CurrentTime = 0
        self.LastTime = 0
        self.cooldown = False
        self.gameover = False
        self.board = [[o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o],
                      [o, o, o, o, o, o, o, o, o, o]]
        for i in range(len(self.board)):
            for j in range(len(self.board[i])):
                self.board[i][j] = GridObject(None)
                      

    def __repr__(self):
        return "You window size is " + str(self.size) +\
               " and your maximum FPS is " + str(self.frames) + "."
    

def drawing(board, screen):
    screen.fill((0, 0, 0))
    for i in range(len(board)):
        for j in range(len(board[i])):
            if board[i][j].color != None:
                pygame.draw.rect(screen, board[i][j].color, ((j * Box_size, i * Box_size),(Box_size, Box_size)), 0)
                pygame.draw.rect(screen, (255, 255, 255) , ((j * Box_size, i * Box_size),(Box_size, Box_size)), 1)
    pygame.display.update()

def piecemaker():
    randomizer = random.uniform(0, 1)
    shape = random.choice(shape_list)[:]
    return block(shape, randomizer)

def rotationcheck1(game):
    active = game.activepiece.shape
    for i in range(len(active)):
        for j in range(len(active[i])):
            if any(t.y > 21 for t in active[i][j]):
                return False            
    return True

def rotationcheck2(game):
    active = game.activepiece.shape
    for i in range(len(active)):
        for j in range(len(active[i])):
            for k in range(len(active[i][j])):
                if game.board[active[i][j][k].y][active[i][j][k].x].color != None and \
                   game.board[active[i][j][k].y][active[i][j][k].x].identifier != active[i][j][k].identifier and \
                   active[i][j][k].color != None:
                    return False
                if game.board[active[i][j][k].y][active[i][j][k].x].color != None and \
                   game.board[active[i][j][k].y][active[i][j][k].x].identifier != active[i][j][k].identifier and \
                   active[i][j][k].color != None:
                    return False
    return True
 
def lockpiece(game):
    active = game.activepiece.orientation
    cooldown = 500
    if not game.cooldown:
        for i in range(len(game.activepiece.orientation)):
            for j in range(len(game.activepiece.orientation[i])):
                if game.activepiece.orientation[i][j].color != None:
                    if  game.activepiece.orientation[i][j].y == 21:
                        game.cooldown = True
                        game.LastTime = pygame.time.get_ticks()
                    elif game.board[active[i][j].y + 1][active[i][j].x].identifier != active[i][j].identifier and \
                         game.board[active[i][j].y + 1][active[i][j].x].color != None:
                        game.cooldown = True
                        game.LastTime = pygame.time.get_ticks()
    if game.cooldown:
        now = pygame.time.get_ticks()
        if now - game.LastTime >= cooldown:
            for i in range(len(game.activepiece.orientation)):
                for j in range(len(game.activepiece.orientation[i])):
                    if game.activepiece.orientation[i][j].color != None:
                        if  game.activepiece.orientation[i][j].y == 21:
                            game.cooldown = False
                            return True
                        elif game.board[active[i][j].y + 1][active[i][j].x].identifier != active[i][j].identifier and \
                             game.board[active[i][j].y + 1][active[i][j].x].color != None:
                            game.cooldown = False
                            return True
                        else:
                            return newpieceinputcheck(game)
                            

def getnewpiece(game):
    if game.activepiece == None or (game.activepiece != None and lockpiece(game) == True):
        lineremoval(game)
        game.activepiece = piecemaker()
        boardoverwrite(game)
        gameovercheck(game)
        if game.gameover:
            pygame.quit()
            sys.exit()

def altnewpiece(game):
    game.activepiece = piecemaker()
    lineremoval(game) 
    boardoverwrite(game)
    game.cooldown = False
    gameovercheck(game)
    if game.gameover:
            pygame.quit()
            sys.exit()

def gameovercheck(game):
    active = game.activepiece.orientation
    for i in range(len(game.activepiece.orientation)):
        for j in range(len(game.activepiece.orientation[i])):
            if game.board[active[i][j].y + 1][active[i][j].x].identifier != active[i][j].identifier and \
            game.board[active[i][j].y + 1][active[i][j].x].color != None:
                game.gameover = True
                print "Game Over!"

def boardoverwrite(game):
    for i in range(len(game.activepiece.orientation)):
        for j in range(len(game.activepiece.orientation[i])):
            if game.activepiece.orientation[i][j].color != None:
                game.board[game.activepiece.orientation[i][j].y][game.activepiece.orientation[i][j].x] = game.activepiece.orientation[i][j]

def movementdeletion(game):
    for i in range(len(game.activepiece.orientation)):
        for j in range(len(game.activepiece.orientation[i])):
            game.board[game.activepiece.orientation[i][j].y][game.activepiece.orientation[i][j].x] = GridObject(None)
            
def moveLeft(game):
    active = game.activepiece.orientation
    movementdeletion(game)
    move = True
    for m in range(len(game.activepiece.orientation)):
        if any(t.x == 0 for t in game.activepiece.orientation[m]):
            move = False
        else:
            for n in range(len(active[m])):
                if game.board[active[m][n].y ][active[m][n].x - 1].identifier != active[m][n].identifier and \
                     game.board[active[m][n].y][active[m][n].x - 1].color != None:
                    move = False
    if move:
        for i in range(len(game.activepiece.shape)):
            for j in range(len(game.activepiece.shape[i])):
                for k in range(len(game.activepiece.shape[i][j])):
                    if game.activepiece.shape[i][j][k].color != None:
                        game.activepiece.shape[i][j][k].x = (game.activepiece.shape[i][j][k].x - 1) % 10
    boardoverwrite(game)

def lineremoval(game):
    count = 0
    lines_removed = 0
    endloop = False
    x = GridObject(None)
    while not endloop:
        lines_removed = 0
        for i in range(len(game.board)):
            count = 0
            mylist = []
            for j in range(len(game.board[i])):
                mylist.append(x)
                if game.board[i][j].color != None:
                    count += 1
            if count == len(game.board[i]):
                game.board.pop(i)
                game.board.insert(0, mylist)
                lines_removed += 1
        if lines_removed == 0:
            endloop = True
        if lines_removed == 4:
            print "WOW"
                        


def moveRight(game):
    active = game.activepiece.orientation
    movementdeletion(game)
    move = True
    for m in range(len(game.activepiece.orientation)):
        if any(t.x == 9 for t in game.activepiece.orientation[m]):
            move = False
        else:
            for n in range(len(active[m])):
                if game.board[active[m][n].y ][active[m][n].x + 1].identifier != active[m][n].identifier and \
                     game.board[active[m][n].y][active[m][n].x + 1].color != None:
                    move = False
    if move:
        for i in range(len(game.activepiece.shape)):
            for j in range(len(game.activepiece.shape[i])):
                for k in range(len(game.activepiece.shape[i][j])):
                    if game.activepiece.shape[i][j][k].color != None:
                        game.activepiece.shape[i][j][k].x = (game.activepiece.shape[i][j][k].x + 1) % 10
    boardoverwrite(game)

def moveDown(game):
    active = game.activepiece.orientation
    check = False
    if not any(t.y == 21 for t in active[len(active) - 1]) and not game.cooldown:
        movementdeletion(game)
        for i in range(len(game.activepiece.shape)):
            for j in range(len(game.activepiece.shape[i])):
                for k in range(len(game.activepiece.shape[i][j])):
                    if game.activepiece.shape[i][j][k].color != None:
                        game.activepiece.shape[i][j][k].y = (game.activepiece.shape[i][j][k].y + 1)
        boardoverwrite(game)

def forced_move(game):
    if game.CurrentTime > 1000:
        game.CurrentTime = 0
        moveDown(game)

def newpieceinputcheck(controller):
    active = controller.activepiece.orientation 
    for i in range(len(controller.activepiece.orientation)):
        for j in range(len(controller.activepiece.orientation[i])):
            if  controller.activepiece.orientation[i][j].y == 21:
                return True                         
            elif controller.board[active[i][j].y + 1][active[i][j].x].identifier != active[i][j].identifier and \
            controller.board[active[i][j].y + 1][active[i][j].x].color != None:
                return True
    controller.cooldown = False
    moveDown(controller)
    return False

def music_setup():
    pygame.mixer.pre_init(44100, -16, 2, 2048)
    pygame.mixer.init
    pygame.mixer.music.load("Tetris.ogg")
    pygame.mixer.music.play(-1)
    

def main():
    black = (0, 0, 0)
    controller = TetrisOBJ(400, 880, 30)
    pygame.init()
    pygame.event.get()
    pygame.mixer.init
    screen = pygame.display.set_mode(controller.size)
    music_setup()
    #gameloop
    while not controller.gameover:

        getnewpiece(controller)
        controller.CurrentTime += controller.clock.tick()
        drawing(controller.board, screen)
        forced_move(controller)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == K_ESCAPE:
                    pygame.quit()
                    sys.exit()
                if event.key == K_LEFT:
                    moveLeft(controller)
                if event.key == K_RIGHT:
                    moveRight(controller)
                if event.key == K_DOWN:
                    if not controller.cooldown:
                        moveDown(controller)
                    elif newpieceinputcheck(controller):
                        altnewpiece(controller)
                if event.key == K_z:
                    controller.activepiece.rotate_CCW(controller)
                if event.key == K_x:
                    controller.activepiece.rotate_CW(controller)
                    
 

main()

