// GameManager.cpp

#include "GameManager.h"
#include "EnemySpawner.h"
#include "EnemyCube.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AGameManager::AGameManager()
{
    PrimaryActorTick.bCanEverTick = false; // No need to tick every frame
}

void AGameManager::BeginPlay()
{
    Super::BeginPlay();

    // Start the main game timer
    GetWorldTimerManager().SetTimer(
        GameTimerHandle,
        this,
        &AGameManager::OnGameEnd,
        GameDuration,
        false // Only once
    );

    UE_LOG(LogTemp, Warning, TEXT("Game started. Timer set for %.1f seconds."), GameDuration);
}

void AGameManager::OnGameEnd()
{
    UE_LOG(LogTemp, Warning, TEXT("Time's up! Stopping spawners and clearing enemies."));

    StopAllSpawners();
    DestroyAllEnemies();
}

void AGameManager::StopAllSpawners()
{
    TArray<AActor*> FoundSpawners;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), FoundSpawners);

    for (AActor* Actor : FoundSpawners)
    {
        if (AEnemySpawner* Spawner = Cast<AEnemySpawner>(Actor))
        {
            Spawner->StopSpawning();
        }
    }
}


void AGameManager::DestroyAllEnemies()
{
    TArray<AActor*> FoundEnemies;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCube::StaticClass(), FoundEnemies);

    for (AActor* Actor : FoundEnemies)
    {
        if (Actor)
        {
            UE_LOG(LogTemp, Warning, TEXT("Destroying enemy: %s"), *Actor->GetName());
            Actor->Destroy();
        }
    }
}

void AGameManager::GameOver()
{
    UE_LOG(LogTemp, Warning, TEXT("Player died — stopping game."));

    StopAllSpawners();
    DestroyAllEnemies();

    
}

