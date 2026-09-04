// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
#include "TimerManager.h"
#include "NavigationSystem.h"


AMonsterAIController::AMonsterAIController()
{
}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	//타이머를 불러 일정 시간마다 MoveToRandomLocation 함수를 호출하도록 설정
	GetWorldTimerManager().SetTimer(
		RandomMoveTimer,//타이머 핸들
		this,//타이머를 설정할 객체
		&AMonsterAIController::MoveToRandomLocation, //호출할 함수
		3.0f, //반복 간격
		true,//반복 여부
		1.0f//초기 지연 시간
	);
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{

	}
}

void AMonsterAIController::MoveToRandomLocation()
{
	APawn* MyPawn = GetPawn();

	//현재 월드의 네비게이션	시스템을 가져옴
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

	//네비게이션 시스템이 반환할 위치정보를 저장할 구조체 선언
	FNavLocation RandomLocation;
	bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(
		MyPawn->GetActorLocation(),//현재 폰의 위치를 기준으로
		MoveRadius,//반환할 반경 지정
		RandomLocation//랜덤 좌표를 반환할 위치정보 구조체
	);

	if (bFoundLocation)
	{
		//AIController의 MoveToLocation 함수를 호출하여 랜덤 좌표로 이동
		MoveToLocation(RandomLocation.Location);
	}
}