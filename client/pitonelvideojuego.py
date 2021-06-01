import pygame
import sys
import random

GAME_SPEED = 10  # esto determina la velocidad del juego (mayor -> mas rapido)

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

# el tamaño de la ventana y el gridsize tienen que ser divisible, y de resultado un n par, si no se mama (hay que añadir excepciones y tal)
screen_width = 480
screen_height = 480

gridsize = 20
grid_width = screen_width/gridsize
grid_height = screen_height/gridsize

# direcciones usadas para el movimiento de la serpiente
up = (0, -1)
down = (0, 1)
left = (-1, 0)
right = (1, 0)


class Snake():
    def __init__(self):
        self.length = SIZE_SNAKE
        # vector de las pociciones del cuerpo -> indice 0 es la cabeza
        self.positions = [((screen_width/2), (screen_height/2))]
        # random.choice([up, down, left, right])  #La dir inicial es aleatoria
        self.direction = right
        self.color = COLOR_SNAKE
        self.score = 0
        # este bool evita que la serpiente se pise a si misma al hacer buffering de input en el mismo frame 
        self.turned = False     # es dependiente de la implementación del juego, no se si será<necesario serializarlo 

    def get_head_position(self):
        return self.positions[0]

    def turn(self, dirMov):

        # si la serpiente mide mas de 1 cubito, no puede girar hacia atras
        if self.turned or (self.length > 1 and (dirMov[0] * -1, dirMov[1] * -1) == self.direction):
            return  # tampoco puede girar si no ha caminado desde la ultima vez que giro
        else:  # si podemos, giramos
            self.direction = dirMov
            self.turned = True

    def move(self):
        cur = self.get_head_position()  # pos actual cabeza
        x, y = self.direction  # direction es una tupla x, y

        new = (((cur[0]+(x*gridsize)) % screen_width),  # nueva pos cabeza en pixeles, haciendo warp al salir de la pantalla
               (cur[1]+(y*gridsize)) % screen_height)

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
        self.positions = [((screen_width/2), (screen_height/2))]
        self.direction = right  # random.choice([up, down, left, right])
        self.score = 0

    def draw(self, surface):
        i = 1
        for p in self.positions:
            # rectangulo que vamos a pintar (pos_x,pos_y, tam_x,tam_y)
            r = pygame.Rect((p[0], p[1]), (gridsize, gridsize))
            pygame.draw.rect(surface, self.color, r)  # lo pintamos
            # efecto de hacer chikita la cola
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


class Food():           # Donuts (*/ω＼*)
    def __init__(self):
        self.position = (0, 0)
        self.color = COLOR_FOOD

    # ponemos el donut en un lugar aleatorio no ocupado por la serpiente
    def randomize_position(self, snake):
        self.position = (random.randint(0, grid_width-1)*gridsize,
                         random.randint(0, grid_height-1)*gridsize)

        # lo unico mas dulce que un donut es la recursion (●'◡'●)
        if self.position in snake.positions:
            self.randomize_position(snake)

    def draw(self, surface):
        r = pygame.Rect((self.position[0]+int(gridsize*0.1), self.position[1] +  # magia para que parezca un donut de verdad
                         int(gridsize*0.1)), (int(gridsize*0.8), int(gridsize*0.8)))
        pygame.draw.rect(surface, self.color, r, SIZE_FOOD)


def drawGrid(surface):  # dibujamos el fondo
    for y in range(0, int(grid_height)):
        for x in range(0, int(grid_width)):
            r = pygame.Rect((x*gridsize, y*gridsize), (gridsize, gridsize))

            if TILE_BG and (x+y) % 2 == 0:  # hacemos el patron de ajedrez
                pygame.draw.rect(surface, COLOR_BG_1, r)
            else:
                pygame.draw.rect(surface, COLOR_BG_2, r)


def main():
    pygame.init()  # inicializamos movidas de pygame

    clock = pygame.time.Clock()
    screen = pygame.display.set_mode((screen_width, screen_height), 0, 32)

    surface = pygame.Surface(screen.get_size())
    surface = surface.convert()

    snake = Snake()  # movidas del juego
    food = Food()
    food.randomize_position(snake)

    myfont = pygame.font.SysFont("monospace", 16)  # IU de los donuts ingeridos

    while (True):
        clock.tick(GAME_SPEED)
        drawGrid(surface)

        snake.handle_keys()
        snake.move()
        # si la cabeza de la serpiente esta encima de comida, nos la comemos y spawneamos mas
        if snake.get_head_position() == food.position:
            snake.length += LENGTH__PER_FOOD
            snake.score += SCORE_PER_FOOD
            food.randomize_position(snake)

        snake.draw(surface)
        food.draw(surface)
        # esto manda la surface a la ventana para pintarla
        screen.blit(surface, (0, 0))

        text = myfont.render("Donuts {0}".format(snake.score), 1, COLOR_SCORE)
        screen.blit(text, (5, 10))  # lo mismo de arriba pero con el texto

        pygame.display.update()  # esto updatea la ventana en si


main()  # 1 linea de juego am I right boys (⌐■_■)
