#include "EnemyCube.h"
#include "PlayerPawnCapsule.h"
#include "ProjectileBullet.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

AEnemyCube::AEnemyCube()
{
    PrimaryActorTick.bCanEverTick = true;

    CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
    RootComponent = CubeMesh;

    CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CubeMesh->SetCollisionObjectType(ECC_Pawn);
    CubeMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    CubeMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block); // bullets
    CubeMesh->SetNotifyRigidBodyCollision(true);

    CubeMesh->OnComponentHit.AddDynamic(this, &AEnemyCube::OnHit);
}

void AEnemyCube::BeginPlay()
{
    Super::BeginPlay();
    Player = Cast<APlayerPawnCapsule>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AEnemyCube::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Player)
    {
        FVector Dir = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        SetActorLocation(GetActorLocation() + Dir * MoveSpeed * DeltaTime);
    }
}

void AEnemyCube::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this) return;

    if (AProjectileBullet* Bullet = Cast<AProjectileBullet>(OtherActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy destroyed by bullet!"));
        Bullet->Destroy();
        Destroy();
    }
    else if (APlayerPawnCapsule* PlayerPawn = Cast<APlayerPawnCapsule>(OtherActor))
    {
        PlayerPawn->HP -= 1;
        UE_LOG(LogTemp, Warning, TEXT("Player hit! HP: %d"), PlayerPawn->HP);
    }
}
