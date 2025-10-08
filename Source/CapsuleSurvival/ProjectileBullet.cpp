#include "ProjectileBullet.h"
#include "EnemyCube.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileBullet::AProjectileBullet()
{
    PrimaryActorTick.bCanEverTick = false;

    BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
    RootComponent = BulletMesh;

    // Collision setup
    BulletMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BulletMesh->SetCollisionObjectType(ECC_WorldDynamic);
    BulletMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    BulletMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block); // collide with enemies
    BulletMesh->SetNotifyRigidBodyCollision(true); // hit events

    // Movement
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 2000.f;
    ProjectileMovement->MaxSpeed = 2000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.f;

    // Destroy after 3s
    InitialLifeSpan = 3.f;
}

void AProjectileBullet::BeginPlay()
{
    Super::BeginPlay();

    BulletMesh->OnComponentHit.AddDynamic(this, &AProjectileBullet::OnHit);
}

void AProjectileBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this) return;

    if (AEnemyCube* Enemy = Cast<AEnemyCube>(OtherActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("Bullet hit enemy: %s"), *Enemy->GetName());
        Enemy->Destroy();
        Destroy();
    }
}
