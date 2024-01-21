from boidFlockHunting import BoidFlockHunting
import pygame


WIDTH, HEIGHT = 1000.0, 800.0
BOID_COLOR = (0, 0, 255)
HUNTER_COLOR = (255, 0, 0)
BOID_SIZE = WIDTH / 40
TELEPORT = 0
NUM_OF_BOIDS = 50

pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Boids")

running = True
clock = pygame.time.Clock()


flock = BoidFlockHunting(50, WIDTH, HEIGHT, TELEPORT, BOID_SIZE, BOID_COLOR, HUNTER_COLOR)
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