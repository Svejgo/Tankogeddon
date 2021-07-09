// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cannon.h"
#include "CoreMinimal.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class ATankPlayerController;
class UArrowComponent;
class ACannon;

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
		UStaticMeshComponent* BodyMesh;

	UPROPERTY(BlueprintReadWrite, VisibleDefaultsOnly, Category = "Components")
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Components")
		USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Components")
		UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Components")
		UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float MoveSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float RotateSpeed = 90.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float RotationSensitivity = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float TurretRotationSensitivity = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
		TSubclassOf<ACannon> FirstCannonClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
		TSubclassOf<ACannon> SecondCannonClass;

	UFUNCTION()
		void TakeDamage(FDamageData DamageData);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UHealthComponent* HealthComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitCollider;
	UFUNCTION()
		void Die();

	UFUNCTION()
		void DamageTaked(float DamageValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void MoveForward(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void MoveRight(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void RotateRight(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Fire")
		void Fire();
	UFUNCTION(BlueprintCallable, Category = "Fire")
		void AltFire();
	UFUNCTION(BlueprintCallable, Category = "Fire")
		void SwapWeapon();
	UFUNCTION(BlueprintCallable, Category = "Turret|Cannon")
	    void SetupCannon(TSubclassOf<class ACannon> NewCannon);
	UFUNCTION(BlueprintCallable, Category = "Turret|Cannon")
		void AddAmmo(int32 Ammo);

private:
	UPROPERTY()
	class ACannon* Cannon;

	UPROPERTY()
	ATankPlayerController* TankController = nullptr;

	float CurrentMoveForwardAxis = 0.f;
	float CurrentMoveRightAxis = 0.f;
	float CurrentRotateRightAxis = 0.f;
	float TargetRotateRightAxis = 0.f;

};
