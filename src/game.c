#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "flecs.h"

// Creating a simple component
    typedef struct Position {
        float x;
        float y;
    } Position;

int main(int argc, char *argv[]) {
// Creating and naming a simple entity
    ecs_world_t *world = ecs_init();
    ecs_entity_t lhgs = ecs_entity(world, {.name = "LucasHGS"});
    printf("My entity's name is %s\n", ecs_get_name(world, lhgs));
}

// cc -o game src/game.c libs/flecs.c -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11