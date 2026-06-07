#pragma once

#include "CoreMinimal.h"
#include "PoolableActor.h"
#include "GameFramework/Character.h"
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
	
protected:
	virtual void BeginPlay() override;
};
