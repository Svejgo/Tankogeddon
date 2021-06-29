// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cannon.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoBox.generated.h"

UCLASS()
class TANKOGEDDON_API AAmmoBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoBox();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Ammo")
		TSubclassOf<ACannon> CannonClass;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Ammo")
		float AddedAmmo = 5.f;

protected:

	UFUNCTION()
		void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
