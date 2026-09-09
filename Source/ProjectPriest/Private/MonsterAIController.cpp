// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAIController.h"
//#include "TimerManager.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"


AMonsterAIController::AMonsterAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	//감지시작 거리
	SightConfig->SightRadius = 1500.0f;
	//감지 끝 거리
	SightConfig->LoseSightRadius = 2000.0f;
	//시야각
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	//감지 지속시간
	SightConfig->SetMaxAge(5.0f);
	//적 감지
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	//중맆 감지
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	//아군 감지
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	//마지막으로 본 위치에서 범위 자동 성공
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 300.0f;
	//설정 적용
	AIPerception->ConfigureSense(*SightConfig);
	//시각컴퍼넌트를 우선 사용
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AMonsterAIController::StartBehaviorTree()
{
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		//UE_LOG(LogTemp, Warning, TEXT("Behavior Tree start"));
	}
}

 
void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();

	StartBehaviorTree();

	if (AIPerception)
	{
		//함수 바인딩
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(
			this,
			&AMonsterAIController::OnPerceptionUpdated
		);
	}
}	

void AMonsterAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		//감지된 정보 저장
		DetectedPlayer = Actor;
		DetectedPlayerLocation = Stimulus.StimulusLocation;
	}
	else
	{
		//시간 종료시 지울건가 지우는 테이블을 따로 만들건가
		DetectedPlayer = nullptr;
		DetectedPlayerLocation = FVector::ZeroVector;
	}
}

AActor* AMonsterAIController::GetDetectedPlayer()
{
	return DetectedPlayer;
}

FVector AMonsterAIController::GetDetectedPlayerLocation()
{
	return DetectedPlayerLocation;
}