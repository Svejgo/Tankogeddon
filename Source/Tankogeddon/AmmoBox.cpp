// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"
#include "TankPawn.h"
#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = SceneComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
	Mesh->SetCollisionProfileName(FName("OverlapAll"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);

}



void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* Tank = Cast<ATankPawn>(OtherActor);
	ACannon* Cannon = Cast<ACannon>(Tank->FirstCannonClass);
	if (Tank && CannonClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Picked up the ammo box"));
		Tank->SetupCannon(CannonClass);
		Destroy();
	}
	else if (Tank && !CannonClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Picked up the ammo box"));
		Tank->AddAmmo(AddedAmmo);
		Destroy();
	}
}

