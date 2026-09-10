#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HolyGenerade.generated.h"

class USphereComponent;

UCLASS()
class PROJECTPRIEST_API AHolyGenerade : public AActor
{
	GENERATED_BODY()
	
public:	
	AHolyGenerade();

	// 외부에서 수류탄의 상태를 변경할 때 사용
	void SetIsThrown(bool bThrown);

	FName GetItemType() const;

	// 외부에서 수류탄을 투척할 때 사용
	void Throw(const FVector& Direction, float Force);

protected:
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "===Item===|Components")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "===Item===|Components")
	USphereComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Item===|Components")
	USphereComponent* ExplosionCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "===Item===|Components")
	UStaticMeshComponent* StaticMesh;

	// Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Item===|Properties")
	FName ItemType = "HolyGenerade";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Item===|Properties")
	float ExplosionDelay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Item===|Properties")
	float ExplosionRadius = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "===Item===|Properties")
	int32 ExplosionDamage = 100;

	// State
	bool bIsThrown = false;
	bool bHasExploded = false;

	FTimerHandle ExplosionTimerHandle;

	// Internal Functions
	UFUNCTION()
	virtual void OnItemOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnItemEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void ActivateItem(AActor* Activator);

	void Explode();

	virtual void DestroyItem();
};