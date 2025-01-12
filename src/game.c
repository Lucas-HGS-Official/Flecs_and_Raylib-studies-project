#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "flecs.h"

// Creating a simple component
typedef struct Position {
    float x;
    float y;
} Position;

typedef struct Elder {
    int age;
    bool isWise;
} Elder;

typedef struct GoblinStats {
    int health;
    int damage;
    int amountOfGold;
    char* clanName;
} GoblinStats;

typedef struct Dog {
    float weight;
    int age;
} Dog;

typedef struct Chihuahua {
    int agressionLevel;
} Chihuahua;

typedef struct Greyhound {
    bool isAsleep;
} Greyhound;

int main(int argc, char* argv[]) {
// Creating a flecs world
    ecs_world_t* world = ecs_init();

// Declaring components
    ECS_COMPONENT(world, Position);
    ECS_COMPONENT(world, Elder);
    ECS_COMPONENT(world, GoblinStats);

    ECS_COMPONENT(world, Dog);
    ECS_COMPONENT(world, Chihuahua);
    ECS_COMPONENT(world, Greyhound);

// Creating and naming simple entities
    ecs_entity_t lhgs = ecs_entity(world, { .name = "LucasHGS" });
    ecs_entity_t lucas = ecs_new(world);

    ecs_entity_t bob = ecs_entity(world, { .name = "Bob" });
    ecs_entity_t alice = ecs_entity(world, { .name = "Alice" });

    ecs_entity_t jacob = ecs_entity(world, { .name = "Jacob" });
    ecs_entity_t evan = ecs_entity(world, { .name = "Evan" });

    // Adding the EcsPrefab tag
    ecs_entity_t dog = ecs_entity(world, {
        .name = "Dog",
        .add = ecs_ids(EcsPrefab)
    });
    ecs_entity_t chihuahua = ecs_new(world);

    ecs_entity_t kweebeck1 = ecs_entity(world, { .name = "kweebeck 1" });
    ecs_entity_t kweebeck2 = ecs_entity(world, { .name = "kweebeck 2" });
    ecs_entity_t kweebeck3 = ecs_entity(world, { .name = "kweebeck 3" });

// Creating types of entity relations
    ecs_entity_t likes = ecs_new(world);

// Creating tags, Captal case by convention
    ecs_entity_t Good = ecs_new(world);
    ecs_entity_t Bad = ecs_new(world);

// Adding components to entities
    ecs_add(world, lhgs, Position);
    ecs_add(world, lhgs, GoblinStats);
    
    ecs_add(world, lucas, Position);
    ecs_add(world, lucas, GoblinStats);

    ecs_add(world, dog, Dog);
    ecs_add(world, chihuahua, Chihuahua);

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

    ecs_set(world, dog, Dog, { .weight = 2.5f, .age = 4 });
    ecs_set(world, chihuahua, Chihuahua, { .agressionLevel = 5 });

// Adding components to entities and Setting their values (ecs_add() is not mandatory)
    ecs_set(world, kweebeck1, Position, { 4, 7 });
    ecs_set(world, kweebeck1, Elder, { 200, true });

    ecs_set(world, kweebeck2, Position, { 1, 8 });

    ecs_set(world, kweebeck3, Position, { 5, 9 });
    ecs_set(world, kweebeck3, Elder, { 250, true });

// Creating relations between entities
    ecs_add_pair(world, bob, likes, alice);
    ecs_add_pair(world, alice, likes, bob);
    // EcsChildOf relation
    ecs_add_pair(world, evan, EcsChildOf, jacob);
    // Prefab
    ecs_add_pair(world, chihuahua, EcsIsA, dog);

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

// Checking for relations
    if (ecs_has_pair(world, bob, likes, alice)) {
        printf("Bob does like Alice\n");
    }
    if (ecs_has_pair(world, evan, EcsChildOf, jacob)) {
        char* pathToEvan = ecs_get_path(world, evan);
        printf("Path to Evan is %s\n", pathToEvan);

        ecs_os_free(pathToEvan);
    }
// Looks for Evan
    ecs_entity_t evanFromParent = ecs_lookup(world, "Jacob.Evan");

// Creating queries
    ecs_query_t* kweebeckQueryPos = ecs_query(world, {
        .terms = {
            { .id = ecs_id(Position) }
        }
    }); // Returns all 3 kweebeck

    ecs_query_t* kweebeckQueryPosAndElder = ecs_query(world, {
        .terms = {
            { .id = ecs_id(Position) }
            { .id = ecs_id(Elder) }
        }
    }); // Returns kweebeck 1 and kweebeck 3
}

// cc -o game src/game.c libs/flecs.c -Iinclude -lraylib -lGL -lm -lpthread -ldl -lrt -lX11