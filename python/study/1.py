import numpy as np
import pygame


GRAVITY = np.array([0, 1])         # gravity 



def normalize(arr: np.ndarray):
    if arr[0] * arr[1] != 0:
        return arr / np.linalg.norm(arr)
    if arr[0] != 0:
        return np.array([1, 0])
    elif arr[1] != 0:
        return np.array([0, 1])
    return np.array([0, 0])

class Point:

    def __init__(self, pos: tuple):
        '''@pos: 节点的坐标位置'''

        self.pos = np.array(pos, dtype=np.float32)                  # current position
        self.prev = np.copy(self.pos)                               # old position

    def update(self, deltaTime:float):
        '''call in each frame
        @deltaTime: frame time'''

        delta = self.pos - self.prev
        self.prev = np.copy(self.pos)
        self.pos += delta + GRAVITY * deltaTime

    @property
    def drawpos(self):
        return (int(self.pos[0]), int(self.pos[1]))


class Rope:

    def __init__(self, pos:tuple, count=30, spring_const=2.3, baseLength=10):
        '''
        @pos: 头节点的位置
        @count: 节点的数量
        @spring_const: 绳索连接线的弹性系数
        @damping: 绳索连接线的弹性阻尼系数
        @baseLength: 绳索连接线的基准长度值 '''

        self.baseL = baseLength
        self.lockPosition = np.array(pos, dtype=np.float32)
        self.points = [Point((pos[0], pos[1] - i * baseLength)) for i in range(count)]
        self.spconst = spring_const

    def update(self, deltaTime):
        '''模拟绳索
        @deltaTime: 每帧的时间'''

        self.points[0].pos = self.lockPosition          # 将第一个节点的位置固定在lockPos
        for pt in self.points[1:]:
            pt.update(deltaTime)                        # 更新每个节点的位置
        self.constraintBasedOnHooke(deltaTime)

    def constraintBasedOnHooke(self, deltaTime):
        '''将两个节点之间视为弹性连接
        @deltaTime: 每帧的时间'''

        for step in range(10):
            # 重复十次以达到快速约束
            for idx in range(len(self.points) - 1):
                pt1, pt2 = self.points[idx], self.points[idx + 1]

                vec = pt1.pos - pt2.pos
                dst = np.linalg.norm(vec)
                if dst > self.baseL:
                    err = dst - self.baseL
                    delta = err * self.spconst * deltaTime * normalize(vec)
                    if idx != 0:
                        delta /= 2
                        pt1.pos -= delta
                        pt2.pos += delta
                    else:
                        pt2.pos += delta

    def constraintUnknown(self, deltaTime:float):
        '''约束'''

        for step in range(10):
            for i in range(len(self.points) - 1):
                pt1, pt2 = self.points[i], self.points[i + 1]
                dst = np.linalg.norm(pt1.pos - pt2.pos)
                err = abs(dst - self.baseL)
                d = np.array((0, 0), dtype=np.float32)
                if dst > self.baseL:
                    _dir = normalize(pt1.pos - pt2.pos)
                else:
                    _dir = normalize(pt2.pos - pt1.pos)
                changeAmount = _dir * err
                if i != 0:
                    changeAmount /= 2
                    pt1.pos -= changeAmount
                    pt2.pos += changeAmount
                else:
                    pt2.pos += changeAmount
    
    def draw(self, s):
        '''绘制绳索
        @s: pygame.Surface'''

        last = self.points[0]
        for pt in self.points[1:]:
            pygame.draw.line(s, (255,255,235), last.drawpos, pt.drawpos)
            last = pt

rope = Rope((500, 100))
screen = pygame.display.set_mode((1000, 600))
clock = pygame.time.Clock()
while 1:
    deltaTime = 1/clock.tick(240)
    screen.fill((67, 67, 79))
    rope.update(deltaTime)
    rope.lockPosition = np.array(pygame.mouse.get_pos(), dtype=np.float32)
    rope.draw(screen)
    pygame.display.flip()
    for e in pygame.event.get():
        if e.type == pygame.QUIT:
            pygame.quit()
            exit(0)
