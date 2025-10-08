#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
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
	UCapsuleComponent* CapsuleComponent; // Root component for collision

    UPROPERTY(VisibleAnywhere)
    UFloatingPawnMovement* MovementComponent; // simple movement for pawn class

    UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm; // Camera boom positioning the camera behind the character

    UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera; // Follow camera

    // Input functions
    void MoveForward(float Value);
    void MoveRight(float Value);

    UPROPERTY(EditAnywhere, Category = "Stats")
    int32 HP = 5;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
        class AController* EventInstigator, AActor* DamageCauser) override;


private:
    UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<AProjectileBullet> ProjectileClass; // Bullet projectile class to spawn

    void Shoot();

    
};
