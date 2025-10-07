#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "ProjectileBullet.h"
#include "PlayerPawnCapsule.generated.h"

UCLASS()
class CAPSULESURVIVAL_API APlayerPawnCapsule : public APawn
{
    GENERATED_BODY()

public:
    APlayerPawnCapsule();

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
    virtual void Tick(float DeltaTime) override;

    // Components
    UPROPERTY(VisibleAnywhere)
    UCapsuleComponent* CapsuleComponent;

    UPROPERTY(VisibleAnywhere)
    UFloatingPawnMovement* MovementComponent;

    UPROPERTY(VisibleAnywhere)
    USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere)
    UCameraComponent* Camera;

    // Input functions
    void MoveForward(float Value);
    void MoveRight(float Value);

private:
    UPROPERTY(EditAnywhere, Category = "Combat")
    TSubclassOf<AProjectileBullet> ProjectileClass;

    void Shoot();
};
