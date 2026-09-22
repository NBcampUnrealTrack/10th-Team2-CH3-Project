// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include "PartViewData.h"-> 뷰에 전송할 구조체를 인클루드 -> 필요하다면 만들기
#include "PartData.h"
#include "MvcModel.h"
#include "PartModel.generated.h"


class UPartSubsystem;

UCLASS()
class PROJECTPRIEST_API UPartModel : public UObject , public IMvcModel
{
	GENERATED_BODY()
	
public:
    void Initialize(UPartSubsystem* PartSubsystem);

    void Disconnect();

    //bool SetCategory(EItemCategory InCategory);

    const FPartViewData& GetData() const
    {
        return Data;
    }

    virtual FDelegateHandle AddListener(UMvcControl* Control) override;

    virtual void RemoveListener(FDelegateHandle Handle) override;

    virtual void InvokePropertyChanged(uint8 PropertyName) override;

protected:
    virtual void BeginDestroy() override;

private:
    UFUNCTION()
    void Refresh();

    UPROPERTY(Transient)
    TObjectPtr<UPartSubsystem> PartSubsystem;

    FPartViewData Data;

    FModelChangedDelegate Changed;

};
