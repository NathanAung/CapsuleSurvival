// GameManager.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameUIWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameManager.generated.h"

UCLASS()
class CAPSULESURVIVAL_API AGameManager : public AActor
{
    GENERATED_BODY()

public:
    AGameManager();

    int32 Score = 0;

    void AddScore(int32 Points);

	void GameOver();

    // Add UI reference
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UGameUIWidget> GameUIClass;

    UPROPERTY()
    UGameUIWidget* GameUI;

    // Timer update helper
    void UpdateUI();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    // How long the game lasts in seconds (1 minute = 60 seconds)
    UPROPERTY(EditAnywhere, Category = "Game Rules")
    float GameDuration = 60.0f;

    // Timer handle for the main game timer
    FTimerHandle GameTimerHandle;

    // Called when the timer ends
    void OnGameEnd();

    // Helper function to stop all enemy spawners
    void StopAllSpawners();

    // Helper function to destroy all existing enemies
    void DestroyAllEnemies();

	bool gameEnded = false;
};
