# -*- coding: utf-8 -*-
import json
from pygame.display import update
from serializable import Serializable
import pygame
import sys
import random
from pygame.math import Vector2
from gameSocket import GameSocket
import threading
from enum import Enum
from threading import Lock
import time

# el tamaño de la ventana y el gridsize tienen que ser divisible, y de resultado un n par, si no se mama (hay que añadir excepciones y tal)
screen_width = 700
screen_height = 700

GRID_SIZE = 21
TILE_SIZE = 24

GAME_SPEED = 5  # esto determina la velocidad del juego (mayor -> mas rapido)

#(rgb(50,50,50),rgb(50,100,20),rgb(100,20,90),rgb(204,122,0),rgb(0,153,255))
COLOR_SNAKES = ((10,10,10),(50,100,20),(120,20,190),(250,122,0),(130,60,50), (220,210,10), (210,30,20), (220,90,150), (20,40,250))
SIZE_SNAKE = 7  # tam inicial

TILE_BG = True  # patron de ajedrez
COLOR_BG_1 = (45,45,45)
COLOR_BG_2 = (35,35,35)

COLOR_FOOD = (223, 50, 90)
SIZE_FOOD = 8
SCORE_PER_FOOD = 1
LENGTH__PER_FOOD = 2

COLOR_SCORE = (200,200,200)
SIZE_SCORE = 100

COLOR_BORDES = (10,20,40)

# direcciones usadas para el movimiento de la serpiente
up =  Vector2(0,-1)
down = Vector2(0,1)
left = Vector2(-1,0)
right = Vector2(1,0)

mtx_gamestate = Lock()

#ids de los headers de red
class messageID(Enum):
    LOGIN = 0
    RESPONSE = 1
    INPUT = 2
    GAMESTATE = 3
    GAMESTART = 4
    REQUESTSTART = 5
    QUIT = 6
    GAMEOVER = 7 

    def __int__(self):
        return self.value

class ClientState(Enum):
    WAITING = 0
    PLAYING = 1
    WON = 2
    LOST = 3

    def __int__(self):
        return self.value

g_cState = ClientState.WAITING
g_thisClientID = -1

class Snake(Serializable):
    def __init__(self,id):
        self.length = SIZE_SNAKE
        # vector de las pociciones del cuerpo -> indice 0 es la cabeza
        self.positions = [Vector2(GRID_SIZE/2,GRID_SIZE/2)]
        # random.choice([up, down, left, right])  #La dir inicial es aleatoria
        self.direction = right
        self.score = 0
        # este bool evita que la serpiente se pise a si misma al hacer buffering de input en el mismo frame 
        self.alive = True
        # self.turned = False     # es dependiente de la implementación del juego, no se si será<necesario serializarlo 
        self.id = id
        self._json = dict()

    def get_json(self):
        json = dict()
        json["playerId"] = self.id
        json["direction"] = dict()
        json["direction"]["x"] = int(self.direction.x)
        json["direction"]["y"]= int(self.direction.y)
        json["alive"] = self.alive
        json["body"] = []
        
        for b in self.positions:
            j = dict()
            j["x"] = b.x
            j["y"] = b.y
            json["body"].append(j)

        json["length"] = self.length
        
        return json
    def from_json(self,json):
        self.id = json["playerId"]
        self.direction = Vector2(json["direction"]["x"],json["direction"]["y"])
        self.positions.clear()
        for p in json["body"]:
            self.positions.append(Vector2(p["x"],p["y"]))
        self.length = json["length"]
        self.alive = json["alive"]

    def get_head_position(self):
        return self.positions[0]

    def turn(self, dirMov):
        # si la serpiente mide mas de 1 cubito, no puede girar hacia atras
        if self.turned or (self.length > 1 and (dirMov.x * -1, dirMov.y * -1) == self.direction):
            return  # tampoco puede girar si no ha caminado desde la ultima vez que giro
        else:  # si podemos, giramos
            self.direction = dirMov
            self.turned = True

    def move(self, gridSize):
        head = self.get_head_position()  # pos actual cabeza
        dir = self.direction  # direction es una tupla x, y

        new = Vector2((head.x+dir.x)%gridSize,(head.y+dir.y)%gridSize)

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
        self.positions = [Vector2(gridSize/2,gridSize/2)]
        self.direction = right  # random.choice([up, down, left, right])
        self.score = 0

    def draw(self, surface):
        i = 0
        borderMax = 10  #en pixeles
        borderMin = 2   #en pixeles

        color = COLOR_SNAKES[self.id]
        if(not self.alive): color = COLOR_SNAKES[0]

        for p in self.positions:
            porc = i/len(self.positions)
            pos = (p.x*TILE_SIZE + int(porc*borderMax/2) + borderMin,
                   p.y*TILE_SIZE + int(porc*borderMax/2) + borderMin)

            tam = (TILE_SIZE - int(porc*borderMax + 2*borderMin), 
                   TILE_SIZE - int(porc*borderMax + 2*borderMin))

            # rectangulo que vamos a pintar (pos_x,pos_y, tam_x,tam_y)
            r = pygame.Rect(pos, tam)
            pygame.draw.rect(surface, color, r)  # lo pintamos
            
            # efecto de hacer chikita la colae
            #pygame.draw.rect(surface, COLOR_BG_1, r, 
            #                int(8 * i/len(self.positions) + 0))
            i += 1


    # Solo para debug,esto lo gestiona el servidor
    # def handle_keys(self):  # no hay cebolla de input (┬┬﹏┬┬)
    #     for event in pygame.event.get():
    #         if event.type == pygame.QUIT:
    #             pygame.quit()
    #             sys.exit()
    #         elif event.type == pygame.KEYDOWN:
    #             if event.key == pygame.K_UP:
    #                 self.turn(up)
    #             elif event.key == pygame.K_DOWN:
    #                 self.turn(down)
    #             elif event.key == pygame.K_LEFT:
    #                 self.turn(left)
    #             elif event.key == pygame.K_RIGHT:
    #                 self.turn(right)
    #             elif event.key == pygame.K_l:
    #                 print('loading snake')
    #                 file = open("pysnake.data","rb")
    #                 self.from_bin(file.read())
    #             elif event.key == pygame.K_s:
    #                 print('saving snake')
    #                 self.to_bin()
    #                 file = open("pysnake.data","wb")
    #                 file.write(self._data)
    #                 file.close()
    #             elif event.key == pygame.K_u:
    #                 sock = gameSocket.GameSocket();
    #                 sock.connect('127.0.0.1',22222)
    #                 sock.send(self,48)
    #             elif event.key == pygame.K_r:
    #                 sock = gameSocket.GameSocket();
    #                 sock.sock.bind(('127.0.0.1',22222))
    #                 j = sock.recvObj()
    #                 if(j["ID"]==48):
    #                     self.from_bin(j["OBJ"])
                    
class Food():           # Donuts (*/ω＼*)
    def __init__(self):
        self.position = Vector2(0, 0)
        self.color = COLOR_FOOD

    # ponemos el donut en un lugar aleatorio no ocupado por la serpiente
    def randomize_position(self, snakes):
        
        self.position = Vector2(random.randint(0, GRID_SIZE-1),
                         random.randint(0, GRID_SIZE-1))        

        # lo unico mas dulce que un donut es la recursion (●'◡'●)
        for snake in snakes:
            if self.position in snake.positions:
                self.randomize_position(snakes)

    def draw(self, surface):
        r = pygame.Rect((self.position.x*TILE_SIZE + 2, self.position.y*TILE_SIZE + 2) # magia para que parezca un donut de verdad
                         , (int(TILE_SIZE*0.8), int(TILE_SIZE*0.8)))
        pygame.draw.rect(surface, self.color, r, SIZE_FOOD)


def drawGrid(surface, gridSize):  # dibujamos el fondo
    for y in range(0, gridSize):
        for x in range(0, gridSize):
            r = pygame.Rect((x*TILE_SIZE, y*TILE_SIZE), (TILE_SIZE, TILE_SIZE))

            if TILE_BG and (x+y) % 2 == 0:  # hacemos el patron de ajedrez
                pygame.draw.rect(surface, COLOR_BG_1, r)
            else:
                pygame.draw.rect(surface, COLOR_BG_2, r)


class GameState(Serializable):
    def __init__(self):
        self.snakes = [Snake(1)]
        self.food = Food()
        self.food.position = Vector2(10,10)
        self.tick = 0

    def update(self, gridSize):
        self.tick= self.tick+1
        for snake in self.snakes:
            snake.move(gridSize)
            
            if snake.get_head_position() == self.food.position:
                snake.length += LENGTH__PER_FOOD
                snake.score += SCORE_PER_FOOD
                self.food.randomize_position(self.snakes)
        
    def get_json(self):
        json = dict()
        json["food"] = dict()
        json["food"]["x"] = self.food.position.x
        json["food"]["y"] = self.food.position.y
        json["snakes"] = []
        for snake in self.snakes:
            json["snakes"].append(snake.get_json())
        json["tick"] = self.tick
        return json
    def from_json(self, json):
        while(len(json["snakes"])>len(self.snakes)):
            self.snakes.append(Snake(0))
        for snake, snakeJSON in zip(self.snakes,json["snakes"]):
            snake.from_json(snakeJSON)
        self.food.position = Vector2(json["food"]["x"],json["food"]["y"])
        self.tick = json["tick"]


    def draw(self,surface, gridSize):
        drawGrid(surface, gridSize)
        for snake in self.snakes:
            snake.draw(surface)
        self.food.draw(surface)

def recGameStateThread(gs, socket, g_thisClientID, gridSize):
    global g_cState
    while(True):
        jObj = socket.recvObj()
        i = jObj["ID"]
        if i == int(messageID.GAMESTART):
            print('recvstart')
            g_cState = ClientState.PLAYING
        elif i == int(messageID.GAMESTATE):
            #print(jObj["OBJ"])
            print("Recibido gamestate")
            mtx_gamestate.acquire()
            diff = jObj["OBJ"]["tick"] - gs.tick
            print(f'''rec {jObj["OBJ"]["tick"]} we in {gs.tick}''')
            gs.from_json(jObj["OBJ"])
            if(not gs.snakes[g_thisClientID - 1].alive): g_cState = ClientState.LOST
            else: 
                for i in range(diff): 
                    print("stepsip")
                    gs.update(gridSize)
            mtx_gamestate.release()
        elif i == int(messageID.GAMEOVER):
            winner = jObj["OBJ"]["idWinner"]
            #print(f"Winner winner chicken diner id:{winner} , idpropia: {g_thisClientID}")
            if  winner == g_thisClientID:
                g_cState = ClientState.WON
        else:
            print('No existe ningun mensaje con ese mensajeId')

def vec2toJson(v2):
    json = dict()
    json["x"] = v2.x
    json["y"] = v2.y
    return json

def sendInput(socket):
    movDir = Vector2(0,0)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_UP:
                movDir = Vector2(0,-1)
            elif event.key == pygame.K_DOWN:
                movDir = Vector2(0,1)
            elif event.key == pygame.K_LEFT:
                movDir = Vector2(-1,0)
            elif event.key == pygame.K_RIGHT:
                movDir = Vector2(1,0)
            #el cliente puede pedir que empiece la partida, pero solo el primero 
            elif event.key == pygame.K_RETURN:
                socket.send({},messageID.REQUESTSTART)
            elif event.key == pygame.K_ESCAPE:
                socket.send({},messageID.QUIT)
    
    if movDir != Vector2(0,0):
        socket.send(vec2toJson(movDir), messageID.INPUT)

def conectaServer(socket, nick):
    ip = "127.0.0.1"
    ip = input("Enter server ip: ")
    socket.connect(ip,55555)
    
    jNick = dict()
    jNick["nick"] = str(nick)

    socket.send(jNick, messageID.LOGIN)

    jObj = socket.recvObj()
    a = jObj["ID"]
    #print(f"respuesta al login: {a}")
    if jObj["ID"] == int(messageID.RESPONSE):
        a = jObj["OBJ"]["playerId"]
        #print(f"mensaje de id de jugador id: {a}")
        return jObj["OBJ"]["playerId"], jObj["OBJ"]["arenaSize"]
    else:
        a = 2
        # print(f"ERROR No se pudo conectar con el servidor, devolvió mensajeID: {}", jObj["ID"])

def drawMargins(surface, gridSize):

    #r = pygame.Rect((x*TILE_SIZE, y*TILE_SIZE), (TILE_SIZE, TILE_SIZE))
    #   pygame.draw.rect(surface, COLOR_BG_1, r)
    gameSize = gridSize * TILE_SIZE

    grosorH = (screen_width - gameSize)/2
    grosorV = (screen_height - gameSize)/2

    r = pygame.Rect((0,0),(screen_width, grosorV))  #borde arriba
    pygame.draw.rect(surface, COLOR_BORDES, r)

    r = pygame.Rect((0,screen_height - grosorV),(screen_width, grosorV)) #borde abajo
    pygame.draw.rect(surface, COLOR_BORDES, r)

    r = pygame.Rect((0,0),(grosorH, screen_height)) #borde iz
    pygame.draw.rect(surface, COLOR_BORDES, r)

    r = pygame.Rect((screen_width - grosorH,0),(grosorH, screen_height)) #borde der
    pygame.draw.rect(surface, COLOR_BORDES, r)

def getFont(size, bold):
    font = pygame.font.SysFont(("purisa","comicsansms","monospace"), 16)
    if (bold): font.bold = True
    return font

#TODO mirar por que el texto no cambia de tamaño, comprobar windows
def drawUI(screen, idCliente):

    if(g_cState == ClientState.WAITING):
        font = getFont(16, True)
        text = font.render("WAITING, PLAYER 1 PRESS START", 1, COLOR_SCORE)
        screen.blit(text, (10, 5))  # lo mismo de arriba pero con el texto
    elif(g_cState == ClientState.PLAYING):
        font = getFont(16, True)
        text = font.render("Controla tu serpiente con las flechas y no mueras!", 1, COLOR_SNAKES[idCliente])
        screen.blit(text, (10, 5))  # lo mismo de arriba pero con el texto
    elif(g_cState == ClientState.WON):
        font = getFont(50, True)
        text = font.render("Has ganado :D", 1, COLOR_SCORE)
        screen.blit(text, (10, 5))  # lo mismo de arriba pero con el texto
    elif(g_cState == ClientState.LOST):
        font = getFont(50, True)
        text = font.render("Nooooo, perdiste :c", 1, COLOR_SCORE)
        screen.blit(text, (10, 5))  # lo mismo de arriba pero con el texto

def drawNPredict(gs,screen,surface,surfBordes,marginL,marginT, g_thisClientID, gridSize):
    while True:
        time.sleep(1/3)
        mtx_gamestate.acquire()
        print("drawn")
        gs.draw(surface, gridSize)
        mtx_gamestate.release()


        drawMargins(surfBordes, gridSize)
        # esto manda la surface a la ventana para pintarla
        screen.blit(surfBordes, ((0, 0)))
        #calculamos los bordes y los ponemos
        r = pygame.Rect((0,0),(TILE_SIZE * gridSize, TILE_SIZE * gridSize))
        screen.blit(surface, (marginL, marginT), r)

        drawUI(screen, g_thisClientID)

        pygame.display.update()  # esto updatea la ventana en si
        
        #Predeciremos el siguiente frame sin input, que se renderizara
        #Si se pierde el paquete
        if(g_cState == ClientState.PLAYING):
            mtx_gamestate.acquire()
            print("predicted")
            gs.update(gridSize)
            mtx_gamestate.release()

def main():

    socketCliente = GameSocket()

    g_thisClientID, GRID_SIZE = conectaServer(socketCliente, "snake")

    gs = GameState()


    pygame.init()  # inicializamos movidas de pygame

    clock = pygame.time.Clock()
    screen = pygame.display.set_mode((screen_width, screen_height), 0, 32)

    surface = pygame.Surface(screen.get_size()) #superficie donde se pinta el juego en si
    surface = surface.convert()

    surfBordes = pygame.Surface(screen.get_size())  #superficie para los bordes
    surfBordes = surface.convert()

    myfont = pygame.font.SysFont(("purisa","comicsansms","monospace"), 16)
    myfont.bold = True

    marginL = (screen_width - GRID_SIZE * TILE_SIZE)/2
    marginT = (screen_height - GRID_SIZE * TILE_SIZE)/2
    #Bucle de recepcion de mensajes
    t1 = threading.Thread(target = recGameStateThread, args=(gs, socketCliente, g_thisClientID, GRID_SIZE))
    #Bucle de renderizado y prediccion
    t2 = threading.Thread(target = drawNPredict, args=(gs,screen,surface,surfBordes,marginL,marginT, g_thisClientID, GRID_SIZE))
    t1.start()
    t2.start()

    #Hilo de input
    while (True):
        clock.tick(GAME_SPEED)
        sendInput(socketCliente)


main()  # 1 linea de juego am I right boys (⌐■_■)
