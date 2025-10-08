// EnemySpawner.cpp

#include "EnemySpawner.h"
#include "EnemyCube.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

AEnemySpawner::AEnemySpawner()
{
    PrimaryActorTick.bCanEverTick = false; // no need to tick every frame
}

void AEnemySpawner::BeginPlay()
{
    Super::BeginPlay();

    // Start a repeating timer that calls SpawnEnemy() every few seconds
    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &AEnemySpawner::SpawnEnemy,
        SpawnInterval,
        true // loop
    );
}

void AEnemySpawner::SpawnEnemy()
{
    // Safety check: Make sure we have a valid enemy class set
    if (!EnemyClass)
    {
        UE_LOG(LogTemp, Error, TEXT("EnemySpawner: EnemyClass not set!"));
        return;
    }

    // Calculate random spawn position within radius
    FVector Origin = GetActorLocation();
    FVector RandomOffset = FVector(
        FMath::RandRange(-SpawnRadius, SpawnRadius),
        FMath::RandRange(-SpawnRadius, SpawnRadius),
        0.f
    );
    FVector SpawnLocation = Origin + RandomOffset;

    // Default rotation (no rotation needed)
    FRotator SpawnRotation = FRotator::ZeroRotator;

    // Spawn parameters (safe spawn)
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // Actually spawn the enemy
    AEnemyCube* SpawnedEnemy = GetWorld()->SpawnActor<AEnemyCube>(
        EnemyClass,
        SpawnLocation,
        SpawnRotation,
        SpawnParams
    );

    if (SpawnedEnemy)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy spawned at: %s"), *SpawnLocation.ToString());
    }
}
