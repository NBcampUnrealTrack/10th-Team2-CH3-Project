#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "BaseItem.generated.h"

class USphereComponent;

UCLASS()
class PROJECTPRIEST_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	ABaseItem();

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "===Item===|Components")
	TObjectPtr<USceneComponent> Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "===Item===|Components")
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "===Item===|Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Item===|Properties")
	FName ItemType = TEXT("BaseItem");

protected:
	// Internal Functions
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;

	virtual void ActivateItem(AActor* Activator) override;

	virtual FName GetItemType() const override;

	virtual void DestroyItem();
};