from boidFlock import BoidFlock
import pygame


WIDTH, HEIGHT = 1000.0, 800.0
BOID_COLOR = (0, 0, 255)
BOID_SIZE = WIDTH / 40
TELEPORT = False
NUM_OF_BOIDS = 30

pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Boids")

running = True
clock = pygame.time.Clock()


flock = BoidFlock(50, WIDTH, HEIGHT, TELEPORT, BOID_SIZE, BOID_COLOR)
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT or event.type == pygame.KEYDOWN and event.key == pygame.K_ESCAPE:
            running = False

    screen.fill((255, 255, 255))

    flock.update()
    flock.draw(screen)

    pygame.display.flip()
    clock.tick(60)

pygame.quit()