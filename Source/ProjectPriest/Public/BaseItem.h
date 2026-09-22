#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "BaseItem.generated.h"

class USphereComponent;

//TODO: 클래스를의 멤버들을 선언할때 public Method(Interface)를 가장 처음에 기술합니다.
//이는 독자로 하여금 클래스의 기능을 가장 먼저 이해할 수 있도록 하기 위함입니다.
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Item===|Properties")
	TObjectPtr<USoundWave> PickupSound;
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