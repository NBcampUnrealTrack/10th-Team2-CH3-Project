#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PriestMainMenuGameMode.generated.h"

// 빈 메뉴 맵에서 전투 캐릭터와 HUD를 생성하지 않는다.
UCLASS()
class PROJECTPRIEST_API APriestMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APriestMainMenuGameMode();
};
