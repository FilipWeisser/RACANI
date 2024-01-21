import pyglet
import numpy as np
import math


class Particle:
    def __init__(self, position, velocity, size, age_limit, texture):
        self.age = 0
        self.age_limit = age_limit
        self.texture = texture

        self.velocity = velocity
        self.texture.x = position[0]
        self.texture.y = position[1]
        self.texture.scale = size

    def update(self, dt):
        self.age+=dt
        self.texture.scale = self.texture.scale - 0.0001
        self.texture.y = self.texture.y - self.velocity
        if np.random.rand()>0.8:
            self.texture.x = self.texture.x + np.random.randn()


class SnowParticleSource:
    def __init__(self, img, position_x_interval, position_y_interval, velocity, size, age_limit_interval, spawn_frequency):
        self.attractors=[]
        self.particles = []
        self.img = img
        self.position_x_interval = position_x_interval
        self.position_y_interval = position_y_interval
        self.velocity = velocity
        self.size = size
        self.age_limit_interval = age_limit_interval
        self.spawn_frequency = int(spawn_frequency/60.0)
        pyglet.clock.schedule_interval(self.update_particles, interval=1/60.0)

    def update_particles(self, dt):
        #print(len(self.particles))
        # spawn new
        for _ in range(self.spawn_frequency):
            self.particles.append(Particle(position=(np.random.randint(self.position_x_interval[0], self.position_x_interval[1]), np.random.randint(self.position_y_interval[0], self.position_y_interval[1])), 
                                        velocity=(np.random.rand()+1)*self.velocity,
                                        size=np.random.rand()*self.size/8,
                                        age_limit=np.random.randint(self.age_limit_interval[0], self.age_limit_interval[1]),
                                        texture=pyglet.sprite.Sprite(self.img)))
        # update all
        for p in self.particles:
            p.update(dt)
            if p.age >= p.age_limit or p.texture.y <= 0 or p.texture.x <= 0 or p.texture.x>=width:
                self.particles.remove(p)

            for a in self.attractors:
                vector = np.array([a.x - p.texture.x, a.y - p.texture.y])
                vector = vector / np.linalg.norm(vector)
                p.texture.x = p.texture.x + a.force * vector[0]
                p.texture.y = p.texture.y + a.force * vector[1]

    def draw_particles(self):
        for p in self.particles:
            p.texture.draw()

class Attractor:
    def __init__(self, position, force):
        self.x = position[0]
        self.y = position[1]
        self.force = force

width = 1280
height = 720
window = pyglet.window.Window(width, height)
img = pyglet.image.load('snow.bmp')

source = SnowParticleSource(img=img, 
                            position_x_interval=(0,width),
                            position_y_interval=(height-1,height),
                            velocity=2, 
                            size=1,
                            age_limit_interval=(5,7),
                            spawn_frequency=100)

attr1 = Attractor((width/2+400, width/2), -4)
attr2 = Attractor((width/2-400, height/2), 2)
#source.attractors.append(attr1)
#ssource.attractors.append(attr2)


@window.event
def on_draw():
    window.clear()
    source.draw_particles()
        
pyglet.app.run()