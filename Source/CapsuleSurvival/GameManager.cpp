// GameManager.cpp

#include "GameManager.h"
#include "EnemySpawner.h"
#include "EnemyCube.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/GameViewportClient.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"

AGameManager::AGameManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AGameManager::BeginPlay()
{
    Super::BeginPlay();

    // --- Force Windowed Mode ---
    if (GEngine)
    {
        GEngine->Exec(GetWorld(), TEXT("r.SetRes 1280x720w"));
    }


	Score = 0;

    // Start the main game timer
    GetWorldTimerManager().SetTimer(
        GameTimerHandle,
        this,
        &AGameManager::OnGameEnd,
        GameDuration,
        false // Only once
    );

    UE_LOG(LogTemp, Warning, TEXT("Game started. Timer set for %.1f seconds."), GameDuration);

    // Create and display UI
    if (GameUIClass)
    {
        GameUI = CreateWidget<UGameUIWidget>(GetWorld(), GameUIClass);
        if (GameUI)
        {
            GameUI->AddToViewport();
            GameUI->UpdateTime(FMath::RoundToInt(GameDuration));
            GameUI->UpdateScore(Score);
        }
    }
}


void AGameManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // Update UI
    UpdateUI();

    if (gameEnded)
    {
        APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (!PC) return;

        PC->bShowMouseCursor = true;

        if (PC->WasInputKeyJustPressed(EKeys::SpaceBar))
        {
            RestartGame();
        }
        else if (PC->WasInputKeyJustPressed(EKeys::Escape))
        {
            QuitGame();
        }
    }
}


void AGameManager::AddScore(int32 Points)
{
    Score += Points;
    UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}


void AGameManager::UpdateUI()
{
    if (GameUI)
    {
        // Get remaining time from timer
        float RemainingTime = GetWorldTimerManager().GetTimerRemaining(GameTimerHandle);
        int32 DisplayTime = FMath::Max(0, FMath::RoundToInt(RemainingTime)); // convert float to int
        GameUI->UpdateTime(DisplayTime);

        GameUI->UpdateScore(Score);
    }
}


void AGameManager::OnGameEnd()
{
	if (gameEnded) return; // Prevent multiple calls

    UE_LOG(LogTemp, Warning, TEXT("Time's up! Stopping spawners and clearing enemies."));

    StopAllSpawners();
    DestroyAllEnemies();

    if (GameUI)
    {
        GameUI->ShowStatusMessage(TEXT("Game Clear"));
    }

	gameEnded = true;
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
	if (gameEnded) return; // Prevent multiple calls

    UE_LOG(LogTemp, Warning, TEXT("Player died — stopping game."));

    StopAllSpawners();
    DestroyAllEnemies();
	GetWorldTimerManager().PauseTimer(GameTimerHandle);

    if (GameUI)
    {
        GameUI->ShowStatusMessage(TEXT("Game Over"));
    }

	gameEnded = true;
}

void AGameManager::RestartGame()
{
    UE_LOG(LogTemp, Warning, TEXT("Restarting game..."));
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName())); // reload current level
}

void AGameManager::QuitGame()
{
    UE_LOG(LogTemp, Warning, TEXT("Quitting game..."));
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}


