#pragma once
//#define JDEBUG

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"

#ifdef JDEBUG
#define JLog(Format, ...) \
{\
    FString MethodName = FString::Printf(TEXT("[%hs]: "), __FUNCTION__); \
    FString FormatString = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    FString CombinedString = FString::Printf(TEXT("%s: %s"), *MethodName, *FormatString); \
    UE_LOG(LogTemp, Log, TEXT("%s"),*CombinedString); \
    if(GEngine) \
    { \
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, *CombinedString); \
    } \
}

#define JWarning(Format, ...) \
{\
    FString MethodName = FString::Printf(TEXT("[%hs]: "), __FUNCTION__); \
    FString FormatString = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    FString CombinedString = FString::Printf(TEXT("%s: %s"), *MethodName, *FormatString); \
    UE_LOG(LogTemp, Warning, TEXT("%s"),*CombinedString); \
    if(GEngine) \
    { \
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, *CombinedString); \
    } \
}

#define JError(Format, ...) \
UE_LOG(LogTemp, Error, TEXT(Format), ##__VA_ARGS__); \
{\
    FString MethodName = FString::Printf(TEXT("[%hs]: "), __FUNCTION__); \
    FString FormatString = FString::Printf(TEXT(Format), ##__VA_ARGS__); \
    FString CombinedString = FString::Printf(TEXT("%s: %s"), *MethodName, *FormatString); \
    UE_LOG(LogTemp, Error, TEXT("%s"),*CombinedString); \
    if(GEngine) \
    { \
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, *CombinedString); \
    } \
}
 
#else

#define JLog(Format, ...) {}
#define JWarning(Format, ...) {}
#define JError(Format, ...) {}
 
#endif

#define JASSERT(Condition, Format, ...) \
if(!(Condition)) \
{ \
    JError(Format, ##__VA_ARGS__); \
    return; \
}

#define JASSERT_INT(Condition, Format, ...) \
if(!Condition) \
{ \
    JError(Format, ##__VA_ARGS__); \
    return -1;\
}

#define JASSERT_BOOL(Condition, Format, ...) \
if(!Condition) \
{ \
    JError(Format, ##__VA_ARGS__); \
    return false;\
}

#define JASSERT_NULLPTR(Condition, Format, ...) \
if(!Condition) \
{ \
    JError(Format, ##__VA_ARGS__); \
    return nullptr;\
}

#define JASSERT_TASK(Condition, Format, ...) \
if(!Condition) \
{ \
    JError(Format, ##__VA_ARGS__); \
    return EBTNodeResult::Failed;\
}

#define JASSERT_RETURN(Condition, Return, Format, ...) \
if(!Condition) \
{ \
    JError(Format, ##__VA_ARGS__); \
    return  Return;\
}


#define GET_ENUM_NAME_STRING(EnumType, EnumVariable) \
StaticEnum<EnumType>()->GetNameStringByValue((int64)EnumVariable)

#define GET_ENUM_DISPLAY_STRING(EnumType, EnumVariable) \
StaticEnum<EnumType>()->GetDisplayNameTextByValue((int64)EnumVariable).ToString()