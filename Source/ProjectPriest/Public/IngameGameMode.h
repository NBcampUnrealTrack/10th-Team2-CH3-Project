#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IngameGameMode.generated.h"

class UWorld;
class APlayerCharacter;
class AIngameGameState;

UCLASS()
class PROJECTPRIEST_API AIngameGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    virtual void BeginPlay() override;
    virtual void InitGameState() override;
    
    void OnPlayerDead();
    void OnOpenBossRoomDoor(APlayerCharacter* Player);
    void OnMonsterSpawned();
	void OnMonsterDead();
    void OnCrossBowPickuped();
protected:
    TSoftObjectPtr<UWorld> LobbyLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== IngameGameMode ===")
	TSoftObjectPtr<UWorld> BossLevel;
	
    TObjectPtr< AIngameGameState> IngameState;
};
