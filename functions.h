#ifndef MFUNCTIONS_H
#define MFUNCTIONS_H

#pragma once


#include "Source/Tree/p2qt.h"
#include <random>

#include "Source/p2particle.h"
#include "Source/p2rigidBody.h"

const int NUM_PARTICLES = 20;
const int NUM_RIGIDBODIES = 20;

// Function prototypes
void handleEvents(bool& running);
void update(float deltaTime);
void render();
void InitializeParticles();
void InitializeBodies();
void AddParticleAtPosition(Vector2D position);

void handleEvents(bool& running)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                int x = event.button.x;
                int y = event.button.y;
                Vector2D position = { static_cast<float>(x), static_cast<float>(y) };

                p2::bodyDef bodyDef;
                bodyDef.position = position;
                bodyDef.type = p2::bodyType::RIGID;
                bodyDef.angle = 0.0f;
                world->CreateBody(&bodyDef);
            }
        }
        // Handle other events like keyboard, mouse, etc.
    }
}


void  UpdateRigidBodies(float deltaTime)
{
    /*for (int i = 0; i < NUM_RIGIDBODIES; ++i) {
        p2::rigidBody* rigidBody = &rigidBodies[i];
        ComputeForceAndTorque(rigidBody);
        Vector2D linearAcceleration = { rigidBody->force.x / rigidBody->shape.mass, rigidBody->force.y / rigidBody->shape.mass };
        rigidBody->linearVelocity.x += linearAcceleration.x * deltaTime;
        rigidBody->linearVelocity.y += linearAcceleration.y * deltaTime;
        rigidBody->position.x += rigidBody->linearVelocity.x * deltaTime;
        rigidBody->position.y += rigidBody->linearVelocity.y * deltaTime;
        float angularAcceleration = rigidBody->torque / rigidBody->shape.momentOfInertia;
        rigidBody->angularVelocity += angularAcceleration * deltaTime;
        rigidBody->angle += rigidBody->angularVelocity * deltaTime;
    }*/
}

void RenderRigidBodies()
{
    /*for (int i = 0; i < NUM_PARTICLES; ++i) {
       p2::particle* particle = &particles[i];
       SDL_RenderDrawPoint(renderer, static_cast<int>(particle->position.x), static_cast<int>(particle->position.y));
   }*/

    //qtree->draw(renderer);

    //for (int i = 0; i < NUM_RIGIDBODIES; ++i) {
    //    p2::rigidBody* rigidBody = &rigidBodies[i];
    //    SDL_Rect rect;
    //    //SDL_RenderDrawRect(renderer, static_cast<int>(rigidBody->position.x), static_cast<int>(rigidBody->position.y), static_cast<int>(rigidBody->angle));
    //    rect.x = static_cast<int>(rigidBody->position.x);
    //    rect.y = static_cast<int>(rigidBody->position.y);
    //    rect.w = rigidBody->shape.width;
    //    rect.h = rigidBody->shape.width;
    //    SDL_RenderFillRect(renderer, &rect);
    //}
}

Vector2D ComputeForce(p2::particle* particle) {
    Vector2D vec = { 0, static_cast<float>(particle->mass * -9.81) };
    return vec;
}

std::random_device rd;  // Obtain a random number from hardware
std::mt19937 gen(rd()); // Seed the generator
std::uniform_int_distribution<> distr(100, 800); // Define the range
std::uniform_int_distribution<> distr_angle(0, 360); // Define the range
std::uniform_int_distribution<> distr_length(10, 30);

void InitializeParticles() {
    /*for (int i = 0; i < NUM_PARTICLES; ++i) {
        Vector2D position = { static_cast<float>(distr(gen)), static_cast<float>(distr(gen)) };
        Vector2D velocity = { 0, 0 };
        particles[i].position = position;
        particles[i].velocity = velocity;
        particles[i].mass = 1;
    }*/
}

void CalculateBoxInertia(p2::shapeBox* boxShape) {
    float m = boxShape->mass;
    float w = boxShape->width;
    float h = boxShape->height;
    boxShape->momentOfInertia = m * (w * w + h * h) / 12;
}

void InitializeBodies()
{
    /*for (int i = 0; i < NUM_RIGIDBODIES; ++i) {
        p2::rigidBody* rigidBody = &rigidBodies[i];
        Vector2D position = { static_cast<float>(distr(gen)), static_cast<float>(distr(gen)) };
        rigidBody->position = position;
        rigidBody->angle = distr_angle(gen) / 360.f * M_PI * 2;
        Vector2D linearVelocity = { 0, 0 };
        rigidBody->linearVelocity = linearVelocity;
        rigidBody->angularVelocity = 0;

        p2::shapeBox shape;
        shape.mass = 0.2f;
        shape.width = 1 + distr_length(gen);
        shape.height = 1 + distr_length(gen);
        CalculateBoxInertia(&shape);
        rigidBody->shape = shape;
    }*/
}


void AddParticleAtPosition(Vector2D position) {
    //static int particleIndex = NUM_PARTICLES; // Start adding new particles after the initial ones

    //if (particleIndex < NUM_PARTICLES * 2) { // Assuming a maximum of twice the initial number of particles
    //    particles[particleIndex % NUM_PARTICLES].position = position;
    //    particles[particleIndex % NUM_PARTICLES].velocity = { 0, 0 };
    //    particles[particleIndex % NUM_PARTICLES].mass = 1;

    //    qtree->insert(position, 1);

    //    ++particleIndex;
    //}

    //qtree->insert(position, 1);
}




void ComputeForceAndTorque(p2::rigidBody* rigidBody) {
    Vector2D f = { 0, 100 };
    rigidBody->force = f;
    // r is the 'arm vector' that goes from the center of mass to the point of force application
    Vector2D r = { rigidBody->shape.width / 2, rigidBody->shape.height / 2 };
    rigidBody->torque = r.x * f.y - r.y * f.x;

}

#endif // !MFUNCTIONS_H