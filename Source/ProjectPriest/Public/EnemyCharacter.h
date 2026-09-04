// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class PROJECTPRIEST_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float Damage;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float Defense;*/


	UFUNCTION(BlueprintCallable)
	virtual void Attack();

	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(float DamageAmount);
	
	UFUNCTION(BlueprintCallable)
	virtual void Die();



	virtual void Destroyed() override;

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
