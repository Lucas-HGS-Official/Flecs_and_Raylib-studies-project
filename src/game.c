#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "flecs.h"

// Creating a simple component
typedef struct Position {
    float x;
    float y;
} Position;

typedef struct GoblinStats {
    int health;
    int damage;
    int amountOfGold;
    char* clanName;
} GoblinStats;

int main(int argc, char* argv[]) {
// Creating a flecs world
    ecs_world_t* world = ecs_init();

// Declaring components
    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, GoblinStats);

// Creating and naming simple entities
    ecs_entity_t lhgs = ecs_entity(world, {.name = "LucasHGS"});
    ecs_entity_t lucas = ecs_new(world);

// Creating tags, Captal case by convention
    ecs_entity_t Good = ecs_new(world);
    ecs_entity_t Bad = ecs_new(world);

// Adding components to entities
    ecs_add(world, lhgs, Position);
    ecs_add(world, lhgs, GoblinStats);
    
    ecs_add(world, lucas, Position);
    ecs_add(world, lucas, GoblinStats);

// Setting component values for the entities
    ecs_set(world, lhgs, Position, { 10, 2 });
    ecs_set(world, lhgs, GoblinStats, {
        .health = 10,
        .damage = 5,
        .amountOfGold = 2,
        .clanName = "5o1st"
    });

    ecs_set(world, lucas, Position, { 12, 2 });
    ecs_set(world, lucas, GoblinStats, {
        .health = 7,
        .damage = 3,
        .amountOfGold = 3,
        .clanName = "5o1st"
    });

// Changing component values for the entities
    GoblinStats *lucasStats = ecs_get_mut(world, lucas, GoblinStats);
    lucasStats->clanName = "renegade";

// Adding tags to entities
    ecs_add_id(world, lhgs, Bad);
    ecs_add_id(world, lucas, Good);

// Checking for tags
    if (ecs_has_id(world, lucas, Good)) {
        printf("Lucas is good doer.\n");
    }
}

// cc -o game src/game.c libs/flecs.c -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11