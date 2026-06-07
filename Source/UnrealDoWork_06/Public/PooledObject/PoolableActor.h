#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
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
};
