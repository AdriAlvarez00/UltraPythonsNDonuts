# -*- coding: utf-8 -*-
from serializable import Serializable
import pygame
import sys
import random
import snake_pb2 as serialMsg
from pygame.math import Vector2
import gameSocket

# el tamaño de la ventana y el gridsize tienen que ser divisible, y de resultado un n par, si no se mama (hay que añadir excepciones y tal)
screen_width = 480
screen_height = 480


#TODO tener un tam de arena independiente para tener UI por fuera o algo asi
GRID_SIZE = 20
TILE_SIZE = 24

GAME_SPEED = 5  # esto determina la velocidad del juego (mayor -> mas rapido)

COLOR_SNAKE = (50, 100, 20)
SIZE_SNAKE = 7  # tam inicial

TILE_BG = True  # patron de ajedrez
COLOR_BG_1 = (150, 150, 170)
COLOR_BG_2 = (180, 180, 200)

COLOR_FOOD = (223, 50, 90)
SIZE_FOOD = 7
SCORE_PER_FOOD = 1
LENGTH__PER_FOOD = 2

COLOR_SCORE = (0, 0, 0)
SIZE_SCORE = 100



# direcciones usadas para el movimiento de la serpiente
up =  Vector2(0,-1)
down = Vector2(0,1)
left = Vector2(-1,0)
right = Vector2(1,0)

class Snake(Serializable):
    def __init__(self,id):
        self.length = SIZE_SNAKE
        # vector de las pociciones del cuerpo -> indice 0 es la cabeza
        self.positions = [Vector2(GRID_SIZE/2,GRID_SIZE/2)]
        # random.choice([up, down, left, right])  #La dir inicial es aleatoria
        self.direction = right
        self.color = COLOR_SNAKE
        self.score = 0
        # este bool evita que la serpiente se pise a si misma al hacer buffering de input en el mismo frame 
        self.turned = False     # es dependiente de la implementación del juego, no se si será<necesario serializarlo 
        self.id = id;
        self._msg = serialMsg.Snake()

    def get_message(self):
        msg = serialMsg.Snake()
        msg.playerID = self.id
        msg.direction.x = int(self.direction.x)
        msg.direction.y = int(self.direction.y)
        i = 0
        for b in self.positions:
            bodyPos = msg.body.add()
            bodyPos.x = int(b.x)
            bodyPos.y = int(b.y)
            i = i+15
        return msg
    def from_message(self,msg):
        print(f'loaded id {msg.playerID}')
        self.id = msg.playerID
        self.direction = Vector2(msg.direction.x,msg.direction.y)
        self.positions.clear()
        for p in msg.body:
            self.positions.append(Vector2(p.x,p.y))
        self.length = len(msg.body)

    def get_head_position(self):
        return self.positions[0]

    def turn(self, dirMov):
        # si la serpiente mide mas de 1 cubito, no puede girar hacia atras
        if self.turned or (self.length > 1 and (dirMov.x * -1, dirMov.y * -1) == self.direction):
            return  # tampoco puede girar si no ha caminado desde la ultima vez que giro
        else:  # si podemos, giramos
            self.direction = dirMov
            self.turned = True

    def move(self):
        head = self.get_head_position()  # pos actual cabeza
        dir = self.direction  # direction es una tupla x, y

        new = Vector2((head.x+dir.x)%GRID_SIZE,(head.y+dir.y)%GRID_SIZE)

        # si la posicion nueva esta ya en el cuerpo (nos hemos chocado), reiniciamos el juego
        if new in self.positions[3:]:
            self.reset()
        else:  # si no nos movemos y crecemos si la variable de longitud es mayor que la longitud real
            self.positions.insert(0, new)
            self.turned = False
            # si no tenemos que crecer, borramos la antigua cola
            if len(self.positions) > self.length:
                self.positions.pop()

    def reset(self):  # reseteamos la serpiente
        self.length = SIZE_SNAKE
        self.positions = [Vector2(GRID_SIZE/2,GRID_SIZE/2)]
        self.direction = right  # random.choice([up, down, left, right])
        self.score = 0

    def draw(self, surface):
        i = 1
        for p in self.positions:
            # rectangulo que vamos a pintar (pos_x,pos_y, tam_x,tam_y)
            r = pygame.Rect((p.x*TILE_SIZE, p.y*TILE_SIZE), (TILE_SIZE, TILE_SIZE))
            pygame.draw.rect(surface, self.color, r)  # lo pintamos
            # efecto de hacer chikita la colae
            pygame.draw.rect(surface, COLOR_BG_1, r, 
                            int(8 * i/len(self.positions) + 1))
            i += 1

    def handle_keys(self):  # no hay cebolla de input (┬┬﹏┬┬)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_UP:
                    self.turn(up)
                elif event.key == pygame.K_DOWN:
                    self.turn(down)
                elif event.key == pygame.K_LEFT:
                    self.turn(left)
                elif event.key == pygame.K_RIGHT:
                    self.turn(right)
                elif event.key == pygame.K_l:
                    print('loading snake')
                    file = open("pysnake.data","rb")
                    self.from_bin(file.read())
                elif event.key == pygame.K_s:
                    print('saving snake')
                    self.to_bin()
                    file = open("pysnake.data","wb")
                    file.write(self._data)
                    file.close()
                elif event.key == pygame.K_u:
                    sock = gameSocket.GameSocket();
                    sock.connect('127.0.0.1',22222)
                    sock.send(self,serialMsg.MessageID.GAMEUPDATE)
                elif event.key == pygame.K_r:
                    sock = gameSocket.GameSocket();
                    sock.sock.bind(('127.0.0.1',22222))
                    head, objStart = sock.recvHeader()
                    print(head._msgid)
                    sock.loadObject(self,objStart)
                    



class Food():           # Donuts (*/ω＼*)
    def __init__(self):
        self.position = Vector2(0, 0)
        self.color = COLOR_FOOD

    # ponemos el donut en un lugar aleatorio no ocupado por la serpiente
    def randomize_position(self, snake):
        self.position = Vector2(random.randint(0, GRID_SIZE-1),
                         random.randint(0, GRID_SIZE-1))

        # lo unico mas dulce que un donut es la recursion (●'◡'●)
        if self.position in snake.positions:
            self.randomize_position(snake)

    def draw(self, surface):
        # TODO recuperar los efectos visuales chulos del donut
        r = pygame.Rect((self.position.x*TILE_SIZE, self.position.y*TILE_SIZE) # magia para que parezca un donut de verdad
                         , (int(TILE_SIZE*0.8), int(TILE_SIZE*0.8)))
        pygame.draw.rect(surface, self.color, r, SIZE_FOOD)


def drawGrid(surface):  # dibujamos el fondo
    for y in range(0, GRID_SIZE):
        for x in range(0, GRID_SIZE):
            r = pygame.Rect((x*TILE_SIZE, y*TILE_SIZE), (TILE_SIZE, TILE_SIZE))

            if TILE_BG and (x+y) % 2 == 0:  # hacemos el patron de ajedrez
                pygame.draw.rect(surface, COLOR_BG_1, r)
            else:
                pygame.draw.rect(surface, COLOR_BG_2, r)


class GameState():
    def __init__(self):
        self.snakes = [Snake(14)]
        self.food = Food()
        self.food.randomize_position(self.snakes[0])
    def update(self):
        for snake in self.snakes:
            snake.handle_keys()
            snake.move()
            
            if snake.get_head_position() == self.food.position:
                snake.length += LENGTH__PER_FOOD
                snake.score += SCORE_PER_FOOD
                self.food.randomize_position(self.snakes[0])


    def draw(self,surface):
        drawGrid(surface)
        for snake in self.snakes:
            snake.draw(surface)
        self.food.draw(surface)

        


def main():
    pygame.init()  # inicializamos movidas de pygame

    clock = pygame.time.Clock()
    screen = pygame.display.set_mode((screen_width, screen_height), 0, 32)

    surface = pygame.Surface(screen.get_size())
    surface = surface.convert()

    snake = Snake(14)  # movidas del juego
    food = Food()
    food.randomize_position(snake)

    myfont = pygame.font.SysFont("monospace", 16)  # IU de los donuts ingeridos

    gs = GameState()

    while (True):
        clock.tick(GAME_SPEED)

        gs.update()
        gs.draw(surface)
        # esto manda la surface a la ventana para pintarla
        screen.blit(surface, (0, 0))

        text = myfont.render("Donuts {0}".format(snake.score), 1, COLOR_SCORE)
        screen.blit(text, (5, 10))  # lo mismo de arriba pero con el texto

        pygame.display.update()  # esto updatea la ventana en si


main()  # 1 linea de juego am I right boys (⌐■_■)
