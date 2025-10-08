// GameManager.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

UCLASS()
class CAPSULESURVIVAL_API AGameManager : public AActor
{
    GENERATED_BODY()

public:
    AGameManager();

	void GameOver();

protected:
    virtual void BeginPlay() override;

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
};
