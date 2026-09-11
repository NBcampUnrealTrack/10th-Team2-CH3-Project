// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemyCharacter.h"

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
}

void AMeleeEnemyCharacter::Attack(ACharacter* PlayerCharacter)
{
	Super::Attack(PlayerCharacter);
	//근접 공격 로직 구현
}