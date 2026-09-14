#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IngameGameMode.generated.h"

class UWorld;
class AIngameGameState;

UCLASS()
class PROJECTPRIEST_API AIngameGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    virtual void BeginPlay() override;
    virtual void InitGameState() override;
    
    void OnPlayerDead();
    void OnOpenBossRoomDoor();
    void OnMonsterSpawned();
	void OnMonsterDead();

protected:
    TSoftObjectPtr<UWorld> LobbyLevel;
    TObjectPtr< AIngameGameState> IngameState;
};
