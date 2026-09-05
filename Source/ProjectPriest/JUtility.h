#pragma once

#include "CoreMinimal.h"

#define JLog(Format, ...) \
UE_LOG(LogTemp, Log, TEXT(Format), ##__VA_ARGS__); \
if(GEngine) { \
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::White, FString::Printf(TEXT(Format), ##__VA_ARGS__)); \
}

#define JWarning(Format, ...) \
UE_LOG(LogTemp, Log, TEXT(Format), ##__VA_ARGS__); \
if(GEngine) { \
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT(Format), ##__VA_ARGS__)); \
}

#define JError(Format, ...) \
UE_LOG(LogTemp, Log, TEXT(Format), ##__VA_ARGS__); \
if(GEngine) { \
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT(Format), ##__VA_ARGS__)); \
}
 

#define JASSERT(Condition, Format, ...) \
if(!(Condition)) { \
    JError(Format, ##__VA_ARGS__); \
    return; \
}

#define JASSERT_INT(Condition, Format, ...) \
if(!Condition) { \
    JError(Format, ##__VA_ARGS__); \
    return -1;\
}

#define JASSERT_BOOL(Condition, Format, ...) \
if(!Condition) { \
    JError(Format, ##__VA_ARGS__); \
    return false;\
}

#define JASSERT_NULLPTR(Condition, Format, ...) \
if(!Condition) { \
    JError(Format, ##__VA_ARGS__); \
    return nullptr;\
}

#define GET_ENUM_STRING(EnumType, EnumVariable) \
StaticEnum<EnumType>()->GetNameStringByValue((int64)EnumVariable)