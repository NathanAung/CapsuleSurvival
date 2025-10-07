#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBullet.generated.h"

UCLASS()
class CAPSULESURVIVAL_API AProjectileBullet : public AActor
{
    GENERATED_BODY()

public:
    AProjectileBullet();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    // The bullet’s collision shape
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* BulletMesh;

    // Bullet movement component
    UPROPERTY(VisibleAnywhere)
    class UProjectileMovementComponent* ProjectileMovement;

    // Called when bullet hits something
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);
};
