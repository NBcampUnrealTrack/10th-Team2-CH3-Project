// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "RangedEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTPRIEST_API ARangedEnemyCharacter : public AEnemyCharacter
{
	GENERATED_BODY()
	
public:
	ARangedEnemyCharacter();

protected:
	//몬스터 공격
	virtual void Attack(ACharacter* PlayerCharacter) override;

};
