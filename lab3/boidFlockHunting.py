from boidFlock import BoidFlock
from boid import Boid
import pygame
import math


visual_range=40*3
protected_range=8*4
centering_factor=0.0005
matching_factor=0.05
avoid_factor=0.05*2
turn_factor=0.2*3
turn_margin=100*2
minspeed = 5
maxspeed = 8
hunterspeed = 9
caught_distance = 10


class BoidFlockHunting(BoidFlock):
    def __init__(self, numBoids, width, height, teleport, size, boid_color, hunter_color):
        super().__init__(numBoids, width, height, teleport, size, boid_color)
        self.boids[0].hunter=True
        self.HUNTER_COLOR = hunter_color

    def calculate_velocity(self, boid : Boid):
        # Zero all accumulator variables
        xpos_avg=ypos_avg=xvel_avg=yvel_avg=neighboring_boids=close_dx=close_dy=0

        # For every other boid in the flock . . .
        for boid2 in self.boids:
            if boid2!=boid:
                # Compute differences in x and y coordinates
                dx = boid.x - boid2.x
                dy = boid.y - boid2.y
                if self.teleport:
                    dx = min(dx, self.WIDTH - dx, key=abs)
                    dy = min(dy, self.HEIGHT - dy, key=abs)
                distance = math.sqrt(dx*dx + dy*dy)

                # Is distance less than the protected range?
                if distance < protected_range:

                    # If so, calculate difference in x/y-coordinates to nearfield boid
                    close_dx += dx
                    close_dy += dy

                # If not in protected range, is the boid in the visual range?
                elif distance < visual_range:
                    if boid2.hunter:
                        boid.vx += dx*avoid_factor/4
                        boid.vy += dy*avoid_factor/4
                        continue

                    # Add other boid's x/y-coord and x/y vel to accumulator variables
                    xpos_avg += boid2.x 
                    ypos_avg += boid2.y 
                    xvel_avg += boid2.vx
                    yvel_avg += boid2.vy

                    # Increment number of boids within visual range
                    neighboring_boids += 1 

        # If there were any boids in the visual range . . .            
        if (neighboring_boids > 0): 

            # Divide accumulator variables by number of boids in visual range
            xpos_avg /= neighboring_boids 
            ypos_avg /= neighboring_boids
            xvel_avg /= neighboring_boids
            yvel_avg /= neighboring_boids

            # Add the centering/matching contributions to velocity
            boid.vx = (boid.vx + 
                    (xpos_avg - boid.x)*centering_factor + 
                    (xvel_avg - boid.vx)*matching_factor)

            boid.vy = (boid.vy + 
                    (ypos_avg - boid.y)*centering_factor + 
                    (yvel_avg - boid.vy)*matching_factor)

        # Add the avoidance contribution to velocity
        boid.vx = boid.vx + (close_dx*avoid_factor)
        boid.vy = boid.vy + (close_dy*avoid_factor)

        
        if not self.teleport:
            # If the boid is near an edge, make it turn by turnfactor
            if boid.y > self.HEIGHT-turn_margin:
                boid.vy = boid.vy - turn_factor
            elif boid.y<turn_margin:
                boid.vy = boid.vy + turn_factor
            if boid.x> self.WIDTH - turn_margin:
                boid.vx = boid.vx - turn_factor
            elif boid.x<turn_margin:
                boid.vx = boid.vx + turn_factor

        magnitude = math.sqrt(boid.vx**2 + boid.vy**2)
        boid.vx = boid.vx / magnitude
        boid.vy = boid.vy / magnitude

        if magnitude > maxspeed:
            magnitude = maxspeed
        elif magnitude < minspeed:
            magnitude = minspeed
        boid.vx = boid.vx * magnitude
        boid.vy = boid.vy * magnitude

            

    def calculate_hunter_velocity(self, hunter : Boid):
        min_distance = self.WIDTH
        closest_boid = None
        for boid2 in self.boids:
            if not boid2.hunter:
                # Compute differences in x and y coordinates
                dx = hunter.x - boid2.x
                dy = hunter.y - boid2.y
                if self.teleport:
                    dx = min(dx, self.WIDTH - dx, key=abs)
                    dy = min(dy, self.HEIGHT - dy, key=abs)
                distance = math.sqrt(dx**2 + dy**2)
                if distance < min_distance:
                    min_distance=distance
                    closest_boid = boid2
        if not closest_boid:
            hunter.vx=hunter.vy=0
            return
        if min_distance < caught_distance:
            closest_boid.hunter = True
            closest_boid.freeze = 30
            hunter.hunter = False
            
        hunter.vx += 0.1*(closest_boid.x-hunter.x)
        hunter.vy += 0.1*(closest_boid.y-hunter.y)
        magnitude = math.sqrt(hunter.vx**2 + hunter.vy**2)
        hunter.vx = hunter.vx / magnitude * hunterspeed
        hunter.vy = hunter.vy / magnitude * hunterspeed


    def update(self):
        for boid in self.boids:
            if boid.hunter:
                if boid.freeze>0:
                    boid.freeze-=1
                    continue
                self.calculate_hunter_velocity(boid)
            else:
                self.calculate_velocity(boid)
            
            #update the position
            boid.x += boid.vx
            boid.y += boid.vy

            if self.teleport:    
                if boid.x < 0:
                    boid.x = self.WIDTH
                elif boid.x > self.WIDTH:
                    boid.x = 0
                if boid.y < 0:
                    boid.y = self.HEIGHT
                elif boid.y > self.HEIGHT:
                    boid.y = 0

            else:# If boid left the screen, teleport it back on the border
                if boid.x>self.WIDTH-10:
                    boid.x=self.WIDTH-10
                elif boid.x<10:
                    boid.x=10
                if boid.y>self.HEIGHT-10:
                    boid.y=self.HEIGHT-10
                elif boid.y<10:
                    boid.y=10


    def draw(self, screen):
        for boid in self.boids:
            angle = math.atan2(boid.vy, boid.vx)
            vertices = [
                (boid.x + self.size * math.cos(angle), boid.y + self.size *math.sin(angle)),
                (boid.x + 0.5 * self.size * math.cos(angle - 2 * math.pi / 3), boid.y + 0.5 * self.size * math.sin(angle - 2 * math.pi / 3)),
                (boid.x, boid.y),
                (boid.x + 0.5 * self.size * math.cos(angle + 2 * math.pi / 3), boid.y + 0.5 * self.size * math.sin(angle + 2 * math.pi / 3))
            ]
            pygame.draw.polygon(screen, self.HUNTER_COLOR if boid.hunter else self.BOID_COLOR, vertices)
