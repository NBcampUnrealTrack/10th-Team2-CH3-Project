// Fill out your copyright notice in the Description page of Project Settings.


#include "RangedEnemyCharacter.h"

ARangedEnemyCharacter::ARangedEnemyCharacter()
{
}

void ARangedEnemyCharacter::Attack(ACharacter* PlayerCharacter)
{
	Super::Attack(PlayerCharacter);
	//원거리 공격 로직 구현
}