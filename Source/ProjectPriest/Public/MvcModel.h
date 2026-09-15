#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UObject/Object.h"
#include "MvcEvents.h"
#include "MvcModel.generated.h"

/*
* TODO: 뭔가 더 좋은 방법이 있을 텐데 떠오르는건 없다.
*/
class UMvcControl;

UINTERFACE(BlueprintType)
class PROJECTPRIEST_API UMvcModel : public UInterface
{
    GENERATED_BODY()
};

/// <summary>
/// <para>class AExampleCharacter : public ACharacter, public IMvcModel             </para>
/// <para> {                                                                        </para>
/// <para>     //...                                                                </para>
/// <para>     virtual void AddListener(UMvcControl* Control) override              </para>
/// <para>     {                                                                    </para>
/// <para>                                                                          </para>
/// <para>     }                                                                    </para>
/// <para>     virtual void InvokePropertyChanged(uint8 PropertyName) override      </para>
/// <para>     {                                                                    </para>
/// <para>     }                                                                    </para>
/// <para>     //...                                                                </para>
/// <para>     protected:                                                           </para>
/// <para>         FModelChangedDelegate OnModelChanged                             </para>
/// <para> };                                                                       </para>
/// </summary>
/// 

class PROJECTPRIEST_API IMvcModel
{
    GENERATED_BODY()

public: 
    virtual FDelegateHandle AddListener(UMvcControl* Control) = 0;
    virtual void RemoveListener(FDelegateHandle Handle) = 0;
    virtual void InvokePropertyChanged(uint8 PropertyName) = 0;
};
