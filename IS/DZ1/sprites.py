from __future__ import annotations
import itertools
import math
import queue
import random
from dataclasses import dataclass
import pygame
import os
import config


class BaseSprite(pygame.sprite.Sprite):
    images = dict()

    def __init__(self, x, y, file_name, transparent_color=None, wid=config.SPRITE_SIZE, hei=config.SPRITE_SIZE):
        pygame.sprite.Sprite.__init__(self)
        if file_name in BaseSprite.images:
            self.image = BaseSprite.images[file_name]
        else:
            self.image = pygame.image.load(os.path.join(config.IMG_FOLDER, file_name)).convert()
            self.image = pygame.transform.scale(self.image, (wid, hei))
            BaseSprite.images[file_name] = self.image
        # making the image transparent (if needed)
        if transparent_color:
            self.image.set_colorkey(transparent_color)
        self.rect = self.image.get_rect()
        self.rect.topleft = (x, y)


class Surface(BaseSprite):
    def __init__(self):
        super(Surface, self).__init__(0, 0, 'terrain.png', None, config.WIDTH, config.HEIGHT)


class Coin(BaseSprite):
    def __init__(self, x, y, ident):
        self.ident = ident
        super(Coin, self).__init__(x, y, 'coin.png', config.DARK_GREEN)

    def get_ident(self):
        return self.ident

    def position(self):
        return self.rect.x, self.rect.y

    def draw(self, screen):
        text = config.COIN_FONT.render(f'{self.ident}', True, config.BLACK)
        text_rect = text.get_rect(center=self.rect.center)
        screen.blit(text, text_rect)


class CollectedCoin(BaseSprite):
    def __init__(self, coin):
        self.ident = coin.ident
        super(CollectedCoin, self).__init__(coin.rect.x, coin.rect.y, 'collected_coin.png', config.DARK_GREEN)

    def draw(self, screen):
        text = config.COIN_FONT.render(f'{self.ident}', True, config.RED)
        text_rect = text.get_rect(center=self.rect.center)
        screen.blit(text, text_rect)


class Agent(BaseSprite):
    def __init__(self, x, y, file_name):
        super(Agent, self).__init__(x, y, file_name, config.DARK_GREEN)
        self.x = self.rect.x
        self.y = self.rect.y
        self.step = None
        self.travelling = False
        self.destinationX = 0
        self.destinationY = 0

    def set_destination(self, x, y):
        self.destinationX = x
        self.destinationY = y
        self.step = [self.destinationX - self.x, self.destinationY - self.y]
        magnitude = math.sqrt(self.step[0] ** 2 + self.step[1] ** 2)
        self.step[0] /= magnitude
        self.step[1] /= magnitude
        self.step[0] *= config.TRAVEL_SPEED
        self.step[1] *= config.TRAVEL_SPEED
        self.travelling = True

    def move_one_step(self):
        if not self.travelling:
            return
        self.x += self.step[0]
        self.y += self.step[1]
        self.rect.x = self.x
        self.rect.y = self.y
        if abs(self.x - self.destinationX) < abs(self.step[0]) and abs(self.y - self.destinationY) < abs(self.step[1]):
            self.rect.x = self.destinationX
            self.rect.y = self.destinationY
            self.x = self.destinationX
            self.y = self.destinationY
            self.travelling = False

    def is_travelling(self):
        return self.travelling

    def place_to(self, position):
        self.x = self.destinationX = self.rect.x = position[0]
        self.y = self.destinationX = self.rect.y = position[1]

    # coin_distance - cost matrix
    # return value - list of coin identifiers (containing 0 as first and last element, as well)
    def get_agent_path(self, coin_distance):
        pass


class ExampleAgent(Agent):
    def __init__(self, x, y, file_name):
        super().__init__(x, y, file_name)

    def get_agent_path(self, coin_distance):
        path = [i for i in range(1, len(coin_distance))]
        random.shuffle(path)
        return [0] + path + [0]


class Aki(Agent):
    def __init__(self, x, y, file_name):
        super().__init__(x, y, file_name)

    def get_agent_path(self, coin_distance: list[list[int]]) -> list[int]:
        num_of_coins = len(coin_distance)

        path = [0]
        possible_coins = set(range(1, num_of_coins))

        while len(path) < num_of_coins:
            best_coin = None
            best_coin_distance = None

            available_paths = coin_distance[path[-1]]
            for coin in possible_coins:
                current_coin_distance = available_paths[coin]

                if best_coin_distance is None or current_coin_distance < best_coin_distance:
                    best_coin = coin
                    best_coin_distance = current_coin_distance

            path.append(best_coin)
            possible_coins.remove(best_coin)

        path.append(0)

        return path


class Jocke(Agent):
    def __init__(self, x, y, file_name):
        super().__init__(x, y, file_name)

    def get_agent_path(self, coin_distance: list[list[int]]) -> list[int]:
        def calc_path_distance(path: list[int]) -> int:
            distance = 0

            last_coin = 0
            for curr_coin in path:
                distance += coin_distance[last_coin][curr_coin]
                last_coin = curr_coin
            distance += coin_distance[last_coin][0]

            return distance

        num_of_coins = len(coin_distance)
        permutations = list(itertools.permutations(range(1, num_of_coins)))
        best_permutation = min(permutations, key=calc_path_distance)

        return [0] + list(best_permutation) + [0]


class Uki(Agent):
    def __init__(self, x, y, file_name):
        super().__init__(x, y, file_name)

    @dataclass
    class PartialPath:
        path: list[int]
        distance: int

        def __len__(self):
            return len(self.path)

        def __getitem__(self, index: int):
            return self.path[index]

        def __lt__(self, other: Uki.PartialPath) -> bool:
            if self.distance < other.distance:
                return True
            elif self.distance > other.distance:
                return False

            if len(self) > len(other):
                return True
            elif len(self) < len(other):
                return False

            return self.path[-1] < other.path[-1]

        def generate_paths(self, coin_distance: list[list[int]], possible_coins: set[int]) -> list[Uki.PartialPath]:
            last_coin = self.path[-1]
            available_paths = coin_distance[last_coin]

            paths: list[Uki.PartialPath] = []
            for coin in possible_coins:
                current_coin_distance = available_paths[coin]

                new_path = self.path + [coin]
                new_distance = self.distance + current_coin_distance

                paths.append(Uki.PartialPath(new_path, new_distance))

            return paths

    def get_agent_path(self, coin_distance: list[list[int]]) -> list[int]:
        num_of_coins = len(coin_distance)
        all_coins = set(range(num_of_coins))

        pending_paths = queue.PriorityQueue[Uki.PartialPath]()
        pending_paths.put(Uki.PartialPath([0], 0))

        while True:
            curr_path: Uki.PartialPath = pending_paths.get()

            if len(curr_path) == num_of_coins + 1:
                return curr_path.path

            possible_coins = (all_coins - set(curr_path.path)) if len(curr_path) < num_of_coins else {0}

            new_paths = curr_path.generate_paths(coin_distance, possible_coins)
            for new_path in new_paths:
                pending_paths.put(new_path)


class Micko(Agent):
    def __init__(self, x, y, file_name):
        super().__init__(x, y, file_name)

    @dataclass
    class Graph:
        coin_distance: list[list[int]]

        def __find_best_mst_edge(self, vertices: set[int], unconnected_vertices: set[int]) -> tuple[int, int]:
            best_start_coin, best_end_coin = None, None

            for start_coin in vertices:
                for end_coin in unconnected_vertices:
                    if best_start_coin is None or self.coin_distance[start_coin][end_coin] < self.coin_distance[best_start_coin][best_end_coin]:
                        best_start_coin, best_end_coin = start_coin, end_coin

            return best_start_coin, best_end_coin

        def __find_mst_edges(self, possible_coins: set[int]) -> list[tuple[int, int]]:
            vertices = {0}
            unconnected_vertices = possible_coins.copy()
            edges: list[tuple[int, int]] = []

            while len(unconnected_vertices) > 0:
                start_coin, end_coin = self.__find_best_mst_edge(vertices, unconnected_vertices)
                vertices.add(end_coin)
                unconnected_vertices.remove(end_coin)
                edges.append((start_coin, end_coin))

            return edges

        def calc_mst_distance(self, possible_coins: set[int]) -> int:
            mst_edges = self.__find_mst_edges(possible_coins)

            mst_distance = 0
            for start_coin, end_coin in mst_edges:
                mst_distance += self.coin_distance[start_coin][end_coin]

            return mst_distance

        def calc_vertex_length(self, start: int, end: int) -> int:
            return self.coin_distance[start][end]

    @dataclass
    class PartialPath:
        path: list[int]
        heuristic: int
        distance: int

        def __len__(self):
            return len(self.path)

        def __getitem__(self, index: int):
            return self.path[index]

        def __lt__(self, other: Micko.PartialPath) -> bool:
            if self.distance + self.heuristic < other.distance + self.heuristic:
                return True
            elif self.distance + self.heuristic > other.distance + self.heuristic:
                return False

            if len(self) > len(other):
                return True
            elif len(self) < len(other):
                return False

            return self.path[-1] < other.path[-1]

        def generate_paths(self, graph: Micko.Graph, possible_coins: set[int]) -> list[Micko.PartialPath]:
            last_coin = self.path[-1]
            new_heuristic = graph.calc_mst_distance(possible_coins)

            paths: list[Micko.PartialPath] = []
            for coin in possible_coins:
                new_path = self.path.copy()
                new_path.append(coin)

                new_distance = self.distance + graph.calc_vertex_length(last_coin, coin)

                paths.append(Micko.PartialPath(new_path, new_heuristic, new_distance))

            return paths

    def get_agent_path(self, coin_distance: list[list[int]]) -> list[int]:
        graph = Micko.Graph(coin_distance)
        num_of_coins = len(coin_distance)
        all_coins = set(range(num_of_coins))

        pending_paths = queue.PriorityQueue[Micko.PartialPath]()
        pending_paths.put(Micko.PartialPath([0], 0, 0))

        while True:
            curr_path: Micko.PartialPath = pending_paths.get()

            if len(curr_path) == num_of_coins + 1:
                return curr_path.path

            possible_coins = (all_coins - set(curr_path.path)) if len(curr_path) < num_of_coins else {0}

            new_paths = curr_path.generate_paths(graph, possible_coins)
            for new_path in new_paths:
                pending_paths.put(new_path)
