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

// Declaring a method for Flecs system
void iterateKweebecs(ecs_iter_t* it);

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
    ecs_entity_t kweebeck4 = ecs_entity(world, { .name = "kweeback 4" });

// Creating types of entity relations
    ecs_entity_t likes = ecs_new(world);
    ECS_TAG(world, FriendsWith);

// Creating tags, Captal case by convention
    ecs_entity_t Good = ecs_new(world);
    ecs_entity_t Bad = ecs_new(world);
    ECS_TAG(world, Player);

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

    ecs_add_pair(world, kweebeck1, EcsChildOf, kweebeck3);
    ecs_add_pair(world, kweebeck2, EcsChildOf, kweebeck3);

    ecs_add_pair(world, kweebeck1, FriendsWith, kweebeck3);
    ecs_add_pair(world, kweebeck2, FriendsWith, kweebeck4);

    // Prefab
    ecs_add_pair(world, chihuahua, EcsIsA, dog);

// Changing component values for the entities
    GoblinStats *lucasStats = ecs_get_mut(world, lucas, GoblinStats);
    lucasStats->clanName = "renegade";

// Adding tags to entities
    ecs_add_id(world, lhgs, Bad);
    ecs_add_id(world, lucas, Good);

    ecs_add_id(world, lhgs, Player);

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
            { .id = ecs_id(Position), .oper = EcsOr  /* Adding a Query Operator */},
            { .id = ecs_id(Elder), .inout = EcsIn /* Adding a access modifier, Read Only mode*/ }
        }
    }); // Returns kweebeck 1 and kweebeck 3

    /*
    All Query Operators:
        EcsAnd forces the queried entities to have this component and another, this is the default for all components demands in a query;
        EcsOr allows the queried entities to not have all components demands in a query;
        EcsNot forces the queries entities to not have this component;

    All access modifiers:
        EcsIn for read only;
        EcsOut for write only;
        EcsInOut for read/write, this is the default;
        EcsNone for no read or write, to make sure entities have this component
    */

   ecs_query_t* kweebeckQueryChildren = ecs_query(world, {
    .terms = {
        { .id = ecs_pair(EcsChildOf, kweebeck3) }
    }
   });

    // Query for FriendsWith ralation, using a generic entity
   ecs_query_t* kweebeckQueryFriendsWith = ecs_query(world, {
    .terms = {
        { .id = ecs_pair(FriendsWith, EcsWildcard) }
    }
   });

    // Query for a Tag
   ecs_query_t* playerQuery = ecs_query(world, {
    .terms = {
        { .id = Player }
    }
   });


   // Simple System exemple
   ecs_entity_t kweebeckSystem = ecs_system(world, {
        .query.terms = {
            { .id = ecs_id(Position), .oper = EcsOr  /* Adding a Query Operator */},
            { .id = ecs_id(Elder), .inout = EcsIn /* Adding a access modifier, Read Only mode*/ }
        },
        // Calling a method in a system
        .callback = iterateKweebecs
    });



// Creating a iterator
    ecs_iter_t it = ecs_query_iter(world, kweebeckQueryPos);
    // Loob through all of the types from the query
    while (ecs_iter_next(&it)) {
        // Loop through the entities of this type
        for (int i = 0; i < it.count; i++) {
            ecs_entity_t kweebeck = it.entities[i];
            printf("Name: %s\n", ecs_get_name(world, kweebeck));
        }
    }

    ecs_iter_t it2 = ecs_query_iter(world, kweebeckQueryPosAndElder);
    while (ecs_iter_next(&it2)) {
        // Getting the first component from an itarator in a generic fashion
        ecs_id_t component = ecs_field_id(&it2, 0);
        for (int i = 0; i < it2.count; i++) {
            if (component == ecs_id(Position)) {
                Position* pos = ecs_table_get_id(world, it2.table, ecs_id(Position), it2.offset + i);
            } else if (component == ecs_id(Elder)) {
                Elder* pos = ecs_table_get_id(world, it2.table, ecs_id(Elder), it2.offset + i);
            }
        }
        // Getting the components from all of the queried entities
        Position* allPositions = ecs_field(&it, Position, 0);
        const Elder* allElders = ecs_field(&it, Elder, 1);

        for (int i = 0; i < it.count; i++) {
            ecs_entity_t kweebeck = it.entities[i];
            printf("Name: %s\n", ecs_get_name(world, kweebeck));

            Position kweebeckPos = allPositions[i];
            printf("Position: { %f, %f }\n", kweebeckPos.x, kweebeckPos.y);

            Elder kweebeckElder = allElders[i];
            printf("The elder's age is %d and is wise is: %d", kweebeckElder.age, kweebeckElder.isWise);
        }
    }

    ecs_run(world, kweebeckSystem, 0, NULL);
}

void iterateKweebecs(ecs_iter_t* it) {
    Position* position = ecs_field(it, Position, 0);
    Elder* elder = ecs_field(it, Elder, 1);
    for (int i = 0; i < it->count; i++) {
        const char* name = ecs_get_name(it->world, it->entities[i]);
        printf("%s has Position {%f, %f} and is %d years old\n",
            name,
            position[i].x,
            position[i].y,
            elder[i].age
        );
    }
}