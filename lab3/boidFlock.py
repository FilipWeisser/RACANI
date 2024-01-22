from boid import Boid
import math
import pygame

visual_range=40*2
protected_range=8*4
centering_factor=0.0005
matching_factor=0.05
avoid_factor=0.05
turn_factor=0.2*2
turn_margin=100*2
minspeed = 5
maxspeed = 8


class BoidFlock:
    def __init__(self, numBoids, width, height, teleport, size, boid_color):
        self.boids = [Boid(width, height) for _ in range(numBoids)]
        self.WIDTH = width
        self.HEIGHT = height
        self.teleport = teleport
        self.size = size
        self.BOID_COLOR = boid_color

    def calculate_velocity(self, boid : Boid):
        # Zero all accumulator variables
        xpos_avg=ypos_avg=xvel_avg=yvel_avg=neighboring_boids=close_dx=close_dy=0

        # For every other boid in the flock . . .
        for boid2 in self.boids:
            if boid2!=boid:
                # Compute differences in x and y coordinates
                dx = boid.x - boid2.x
                dy = boid.y - boid2.y
                distance = math.sqrt(dx*dx + dy*dy)

                # Is distance less than the protected range?
                if distance < protected_range:

                    # If so, calculate difference in x/y-coordinates to nearfield boid
                    close_dx += dx
                    close_dy += dy

                # If not in protected range, is the boid in the visual range?
                elif distance < visual_range:

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

            if boid.y > self.HEIGHT-turn_margin:
                boid.vy = boid.vy - turn_factor
            if boid.x> self.WIDTH - turn_margin:
                boid.vx = boid.vx - turn_factor
            if boid.x<turn_margin:
                boid.vx = boid.vx + turn_factor
            if boid.y<turn_margin:
                boid.vy = boid.vy + turn_factor

        # Calculate the boid's speed
        speed = math.sqrt(boid.vx**2 + boid.vy**2)

        boid.vx = boid.vx / speed
        boid.vy = boid.vy / speed

        if speed > maxspeed:
            speed = maxspeed
        elif speed < minspeed:
            speed = minspeed
        boid.vx = boid.vx * speed
        boid.vy = boid.vy * speed


    def update(self):
        for boid in self.boids:
            self.calculate_velocity(boid)
            
            # Update boid's position
            boid.x = boid.x + boid.vx
            boid.y = boid.y + boid.vy

    def draw(self, screen):
        for boid in self.boids:
            angle = math.atan2(boid.vy, boid.vx)
            vertices = [
                (boid.x + self.size * math.cos(angle), boid.y + self.size *math.sin(angle)),
                (boid.x + 0.5 * self.size * math.cos(angle - 2 * math.pi / 3), boid.y + 0.5 * self.size * math.sin(angle - 2 * math.pi / 3)),
                (boid.x, boid.y),
                (boid.x + 0.5 * self.size * math.cos(angle + 2 * math.pi / 3), boid.y + 0.5 * self.size * math.sin(angle + 2 * math.pi / 3))
            ]
            #pygame.draw.polygon(screen, CATCHER_COLOR if is_catcher else BOID_COLOR, triangle_points)
            pygame.draw.polygon(screen, self.BOID_COLOR, vertices)





