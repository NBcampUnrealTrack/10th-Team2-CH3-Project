#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IngamePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class PROJECTPRIEST_API AIngamePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    AIngamePlayerController();

    virtual void BeginPlay() override;

public:
    TObjectPtr<UInputAction> GetMoveAction();
    TObjectPtr<UInputAction> GetLookAction();
    TObjectPtr<UInputAction> GetFireAction();
    TObjectPtr<UInputAction> GetThrowction();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Enhanced Inputs")
    UInputMappingContext* InputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Enhanced Inputs")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Enhanced Inputs")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Enhanced Inputs")
    TObjectPtr<UInputAction> FireAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===PlayerCharacter===|Enhanced Inputs")
    TObjectPtr<UInputAction> ThrowAction;
};
