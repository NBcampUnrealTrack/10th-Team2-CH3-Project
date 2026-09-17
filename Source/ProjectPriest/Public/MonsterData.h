// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DropItemData.h"
#include "MonsterData.generated.h"

USTRUCT(BlueprintType)
struct FMonsterData : public FTableRowBase
{
	GENERATED_BODY()

	//몬스터 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health = 0.0f;

	//몬스터 공격력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Damage = 0.0f;

	//몬스터 방어력
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	float Defense = 0.0f;

	//몬스터 최소 피해량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float MinimumDamage = 0.0f;

	//몬스터 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float AttackRange = 0.0f;

	//몬스터의 순찰반경
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Patrol")
	float PatrolRadius = 0.0f;

	//드롭 아이템 관리 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|DropItem")
	FDropItemData DropItem;
};