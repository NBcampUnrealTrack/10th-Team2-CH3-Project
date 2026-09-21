#pragma once

#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "MvcModel.h"
#include "SevarogHud.h"
#include "Sevarog.generated.h"

UENUM(BlueprintType)
enum class ESevarogPropertyName : uint8
{
	Health				UMETA(DisplayName = "Health"),
	NovaCastingStarted	UMETA(DisplayName = "NovaCastingStarted"),
};

UCLASS()
class PROJECTPRIEST_API ASevarog : public AEnemyCharacter, public IMvcModel
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASevarog();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	void ApplyDamage();
	
	UFUNCTION(BlueprintCallable)
	void IncreasePhase();
	
	UFUNCTION(BlueprintCallable)
	int GetPhase();
	
	UFUNCTION(BlueprintCallable)
	bool GetNovaCastingStarted();
	
	UFUNCTION(BlueprintCallable)
	void SetNovaCastingStarted(bool bIsStarted);
	
	
	virtual FDelegateHandle AddListener(UMvcControl* Control) override;
	virtual void RemoveListener(FDelegateHandle Handle) override;
	virtual void InvokePropertyChanged(uint8 PropertyName) override;

	virtual float TakeDamage(float DamageAmount
		, struct FDamageEvent const& DamageEvent
		, class AController* EventInstigator
		, AActor* DamageCauser) override;
	
	virtual void Heal(float HealAmount) override;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Sevarog ===|Properties")
	float MeleeAttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Sevarog ===|Properties")
    TSubclassOf<USevarogHud> HudClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "=== Sevarog ===|Properties")
	int Phase;
	
	FModelChangedDelegate Delegate;
	
	bool bNovaCastingStarted;
};
