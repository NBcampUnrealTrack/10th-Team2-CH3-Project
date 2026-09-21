#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ECompareOperatorType.generated.h"

UENUM(BlueprintType)
enum class ECompareOperatorType : uint8
{
	Equal				UMETA( DisplayName = "=="),
	NotEqual			UMETA( DisplayName = "!="),
	Greater				UMETA( DisplayName = ">"),
	GreaterOrEqual		UMETA( DisplayName = ">="),
	Less				UMETA( DisplayName = "<"),
	LessOrEqual			UMETA( DisplayName = "<=")
};

