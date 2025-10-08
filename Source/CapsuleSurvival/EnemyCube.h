// EnemyCube.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCube.generated.h"

class UStaticMeshComponent;

UCLASS()
class CAPSULESURVIVAL_API AEnemyCube : public AActor
{
    GENERATED_BODY()

public:
    AEnemyCube();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Reference to player
    class APlayerPawnCapsule* Player;

    // Movement speed
    UPROPERTY(EditAnywhere)
    float MoveSpeed = 200.f;

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* CubeMesh;

    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);
};
