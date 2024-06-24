#include <iostream>
#include "SDL.h"
#undef main

#include <random>

#include "Source/p2particle.h"

// Function prototypes
void handleEvents(bool& running);
void update(float deltaTime);
void render();
void InitializeParticles();

const int NUM_PARTICLES = 20;


p2::particle particles[NUM_PARTICLES];

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("SDL Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 800,
        SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    bool running = true;
    Uint32 lastTime = SDL_GetTicks();

    InitializeParticles();

    while (running)
    {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        handleEvents(running);
        update(deltaTime);
        render();

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

Vector2D ComputeForce(p2::particle* particle) {
    Vector2D vec = { 0, static_cast<float>(particle->mass * -9.81) };
    return vec;
}

std::random_device rd;  // Obtain a random number from hardware
std::mt19937 gen(rd()); // Seed the generator
std::uniform_int_distribution<> distr(100, 800); // Define the range

void InitializeParticles() {
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        Vector2D position = { static_cast<float>(distr(gen)), static_cast<float>(distr(gen)) };
        Vector2D velocity = { 0, 0 };
        particles[i].position = position;
        particles[i].velocity = velocity;
        particles[i].mass = 1;
    }
}


void handleEvents(bool& running)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        // Handle other events like keyboard, mouse, etc.
    }
}

void update(float deltaTime)
{
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        p2::particle * particle = &particles[i];
        Vector2D force = ComputeForce(particle);
        Vector2D acceleration = { force.x / particle->mass, force.y / particle->mass };
        particle->velocity.x += acceleration.x * deltaTime;
        particle->velocity.y += acceleration.y * deltaTime;
        particle->position.x += particle->velocity.x * deltaTime;
        particle->position.y += particle->velocity.y * deltaTime;
    }
}

void render()
{
    SDL_Renderer* renderer = SDL_GetRenderer(SDL_GetWindowFromID(1));
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color for particles

    for (int i = 0; i < NUM_PARTICLES; ++i) {
        p2::particle* particle = &particles[i];
        SDL_RenderDrawPoint(renderer, static_cast<int>(particle->position.x), static_cast<int>(particle->position.y));
    }

    SDL_RenderPresent(renderer);
}