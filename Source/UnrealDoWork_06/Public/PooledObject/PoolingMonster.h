#pragma once

#include "CoreMinimal.h"
#include "PoolableActor.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "PoolingMonster.generated.h"

UCLASS()
class UNREALDOWORK_06_API APoolingMonster : public ACharacter,public IPoolableActor
{
	GENERATED_BODY()

public:
	APoolingMonster();
	
#pragma region Override
	// 풀에서 꺼내질때
	virtual void OnAcquiredFromPool() override;
	// 풀로 회수 될때
	virtual void OnReturnedToPool() override;
#pragma endregion

#pragma region Pooling
	// 이 액터가 속한 풀의 태그 (스폰 매니저가 Acquire 직후 세팅, Release 시 사용)
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Object Pool")
	FGameplayTag PoolTag;

	virtual void SetPoolTag(const FGameplayTag& InTag) override { PoolTag = InTag; }
	virtual FGameplayTag GetPoolTag() const override { return PoolTag; }
#pragma endregion

protected:
	virtual void BeginPlay() override;
};
