// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class APlayerCharacter;
class USphereComponent;

//TODO: 몬스터 스탯을 테이블로 해야한다
UCLASS()
class PROJECTPRIEST_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();


protected:

	//스피어 컴퍼넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent;

	//몬스터 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float Health;
	//몬스터 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float Damage;

	//몬스터 최소 피해량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MinimumDamage = 0.0f;

	//몬스터 방어력
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float Defense;

	//몬스터 공격
	UFUNCTION(BlueprintCallable)
	virtual void Attack(ACharacter* PlayerCharacter);

	//몬스터 피격
	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(float DamageAmount);
	
	//몬스터 사망
	UFUNCTION(BlueprintCallable)
	virtual void Die();


	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
