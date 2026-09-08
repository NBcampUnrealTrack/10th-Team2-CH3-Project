#pragma once

#include "CoreMinimal.h"

class UState;

FUNC_DECLARE_DELEGATE(FTransitionCheckingDelegate, bool, UState*, UState*)