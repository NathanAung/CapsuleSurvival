// EnemySpawner.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

// Forward declare AEnemyCube to avoid unnecessary includes
class AEnemyCube;

UCLASS()
class CAPSULESURVIVAL_API AEnemySpawner : public AActor
{
    GENERATED_BODY()

public:
    AEnemySpawner();

    void StopSpawning();

protected:
    virtual void BeginPlay() override;

private:
    // The enemy type to spawn
    // You can set this in the Unreal Editor (Blueprint defaults panel)
    UPROPERTY(EditAnywhere, Category = "Spawner")
    TSubclassOf<AEnemyCube> EnemyClass;

    // How often to spawn enemies (in seconds)
    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnInterval = 3.0f;

    // How far away from the spawner enemies can appear (for random spawn)
    UPROPERTY(EditAnywhere, Category = "Spawner")
    float SpawnRadius = 500.0f;

    // Timer handle to repeatedly call the spawn function
    FTimerHandle SpawnTimerHandle;

    // Function that actually spawns the enemy
    void SpawnEnemy();
};
