// EnemyCube.cpp

#include "EnemyCube.h"
#include "PlayerPawnCapsule.h"
#include "ProjectileBullet.h"
#include "GameManager.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

AEnemyCube::AEnemyCube()
{
    PrimaryActorTick.bCanEverTick = true;

    CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
    RootComponent = CubeMesh;

    CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CubeMesh->SetCollisionObjectType(ECC_WorldDynamic);
    CubeMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    CubeMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    CubeMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    CubeMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // allows player overlap
    CubeMesh->SetGenerateOverlapEvents(true);

    CubeMesh->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCube::OnOverlapBegin);
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

// Called when overlapping another actor
void AEnemyCube::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this) return;

    if (AProjectileBullet* Bullet = Cast<AProjectileBullet>(OtherActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy destroyed by bullet!"));

        // Award points through GameManager
        AGameManager* GameManager = Cast<AGameManager>(
            UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass())
        );

        if (GameManager)
        {
            GameManager->AddScore(100);
        }

        Bullet->Destroy();
        Destroy();
    }
    else if (APlayerPawnCapsule* PlayerPawn = Cast<APlayerPawnCapsule>(OtherActor))
    {
        // Deal damage properly using Unreal’s damage system
        UGameplayStatics::ApplyDamage(PlayerPawn, 1.0f, nullptr, this, nullptr);
        UE_LOG(LogTemp, Warning, TEXT("Enemy overlapped player! Damage applied."));
        Destroy(); // Enemy destroys itself on hit
    }
}
