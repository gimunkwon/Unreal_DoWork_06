#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "PoolableActor.generated.h"


UINTERFACE(MinimalAPI,Blueprintable)
class UPoolableActor : public UInterface
{
	GENERATED_BODY()
};

class UNREALDOWORK_06_API IPoolableActor
{
	GENERATED_BODY()
public:
	virtual void OnAcquiredFromPool() = 0;
	virtual void OnReturnedToPool() = 0;

	// 풀이 액터를 활성화하기 직전에 자신이 속한 풀의 태그를 주입해줌 (OnAcquiredFromPool 시점에 이미 유효한 값을 보장)
	// 태그 보관이 필요 없는 액터는 굳이 오버라이드하지 않아도 됨
	virtual void SetPoolTag(const FGameplayTag& InTag) {}
	virtual FGameplayTag GetPoolTag() const { return FGameplayTag(); }
};
