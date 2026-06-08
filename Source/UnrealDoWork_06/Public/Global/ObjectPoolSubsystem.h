#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "ObjectPoolSubsystem.generated.h"

// 태그 하나에 대응하는 대기(비활성) 액터 목록
USTRUCT()
struct FPooledActorArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<AActor>> Actors;
};

UCLASS()
class UNREALDOWORK_06_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
#pragma region Override
	virtual void Deinitialize() override;
#pragma endregion

#pragma region PoolingFunc
	// 풀에서 액터를 꺼냄 (대기 풀에 있으면 재사용, 없으면 새로 스폰)
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	AActor* AcquireActor(FGameplayTag PoolTag, TSubclassOf<AActor> ActorClass, const FTransform& SpawnTransform);

	// 액터를 사용 종료 처리하고 풀로 반납
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void ReleaseActor(AActor* Actor, FGameplayTag PoolTag);

	// 미리 스폰 후 즉시 반납하여 대기 풀을 채워둠 (런타임 스폰 비용 절감)
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void PrewarmPool(FGameplayTag PoolTag, TSubclassOf<AActor> ActorClass, int32 Count);
#pragma endregion 

private:
	UPROPERTY()
	TMap<FGameplayTag, FPooledActorArray> PooledActors;
};