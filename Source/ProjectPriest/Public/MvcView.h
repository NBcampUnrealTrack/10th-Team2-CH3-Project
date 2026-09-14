#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MvcEvents.h"
#include "MvcView.generated.h"

class UMvcControl;


/// <summary>
/// <para>class AExampleWidget : public UUserWidget, public IMvcView                </para>
/// <para> {                                                                        </para>
/// <para>     //...                                                                </para>
/// <para>     virtual void AddListener(IMvcControl* Control) override              </para>
/// <para>     {                                                                    </para>
/// <para>                                                                          </para>
/// <para>     }                                                                    </para>
/// <para>     virtual void InvokeViewEvent(uint8 EventName) override               </para>
/// <para>     {                                                                    </para>
/// <para>     }                                                                    </para>
/// <para>     //...                                                                </para>
/// <para>     protected:                                                           </para>
/// <para>         FModelChangedDelegate OnModelChanged                             </para>
/// <para> };                                                                       </para>
/// </summary>
/// 
UINTERFACE(BlueprintType)
class PROJECTPRIEST_API UMvcView : public UInterface
{
    GENERATED_BODY()
};

class PROJECTPRIEST_API IMvcView 
{
    GENERATED_BODY()
public:
    virtual FDelegateHandle AddListener(UMvcControl* Control) = 0;
    virtual void RemoveListener(FDelegateHandle DelegateHandle) = 0;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="EventName">must be casted from enum type</param>
    virtual void InvokeViewEvent(EViewEventType EventName, UEventParameterBase* Parameter) = 0;
};