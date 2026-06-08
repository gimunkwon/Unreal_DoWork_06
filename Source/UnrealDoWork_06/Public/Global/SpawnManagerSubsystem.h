#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "SpawnManagerSubsystem.generated.h"

class APoolingMonster;

UCLASS()
class UNREALDOWORK_06_API USpawnManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	USpawnManagerSubsystem();

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

protected:
	// 소환할 몬스터 클래스 (모든 태그 풀이 동일한 클래스를 공유 - 메커니즘 검증용)
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TSubclassOf<AActor> MonsterClass;

	// 사용할 풀 태그 목록 - 소환 시 이 중 하나를 무작위로 골라 해당 태그의 풀에서 꺼내 씀
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	TArray<FGameplayTag> MonsterPoolTags;

	// 신규 소환은 플레이어로부터 [MinSpawnDistance, MaxSpawnDistance] 범위 안에서만 (너무 가까이 출현 방지)
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float MinSpawnDistance = 800.f;

	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float MaxSpawnDistance = 2000.f;

	// 이 거리를 벗어난 몬스터는 회수 (MaxSpawnDistance보다 충분히 크게 두어 경계에서 깜빡이는 것을 방지)
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float DespawnDistance = 3000.f;

	// 유지할 목표 활성 개체 수
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	int32 TargetActiveCount = 5;

	// 스폰/회수 체크 주기 (초)
	UPROPERTY(EditDefaultsOnly, Category = "Spawn")
	float CheckInterval = 1.0f;

private:
	void CheckSpawnAndDespawn();
	FVector GetRandomSpawnLocation(const FVector& PlayerLocation) const;
	FGameplayTag GetRandomMonsterPoolTag() const;

	UPROPERTY()
	TObjectPtr<APawn> PlayerPawn;

	UPROPERTY()
	TArray<TObjectPtr<APoolingMonster>> ActiveMonsters;

	FTimerHandle SpawnCheckTimerHandle;
};