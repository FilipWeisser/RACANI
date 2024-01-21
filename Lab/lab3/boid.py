import numpy as np

class Boid:
    def __init__(self, width, height):
        self.x=np.random.randint(0,width)
        self.y=np.random.randint(0,height)
        self.vx=np.random.normal()
        self.vy=np.random.normal()
        self.hunter = False
        self.freeze = 0