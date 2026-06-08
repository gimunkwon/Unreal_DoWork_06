#include "UnrealDoWork_06/Public/Global/ObjectPoolSubsystem.h"
#include "UnrealDoWork_06/Public/PooledObject/PoolableActor.h"
#include "Engine/World.h"

void UObjectPoolSubsystem::Deinitialize()
{
	PooledActors.Empty();

	Super::Deinitialize();
}

AActor* UObjectPoolSubsystem::AcquireActor(FGameplayTag PoolTag, TSubclassOf<AActor> ActorClass, const FTransform& SpawnTransform)
{
	if (!ActorClass)
	{
		return nullptr;
	}

	FPooledActorArray& Pool = PooledActors.FindOrAdd(PoolTag);

	AActor* Actor = nullptr;
	if (Pool.Actors.Num() > 0)
	{
		Actor = Pool.Actors.Pop();
		Actor->SetActorTransform(SpawnTransform);
	}
	else
	{
		UWorld* World = GetWorld();
		if (!World)
		{
			return nullptr;
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Actor = World->SpawnActor<AActor>(ActorClass, SpawnTransform, SpawnParams);
	}

	if (Actor)
	{
		Actor->SetActorHiddenInGame(false);
		Actor->SetActorEnableCollision(true);
		Actor->SetActorTickEnabled(true);

		if (IPoolableActor* Poolable = Cast<IPoolableActor>(Actor))
		{
			// OnAcquiredFromPool 시점에 이미 유효한 태그를 참조할 수 있도록 콜백보다 먼저 주입
			Poolable->SetPoolTag(PoolTag);
			Poolable->OnAcquiredFromPool();
		}
	}

	return Actor;
}

void UObjectPoolSubsystem::ReleaseActor(AActor* Actor, FGameplayTag PoolTag)
{
	if (!IsValid(Actor))
	{
		return;
	}

	if (IPoolableActor* Poolable = Cast<IPoolableActor>(Actor))
	{
		Poolable->OnReturnedToPool();
	}

	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
	Actor->SetActorTickEnabled(false);

	PooledActors.FindOrAdd(PoolTag).Actors.Add(Actor);
}

void UObjectPoolSubsystem::PrewarmPool(FGameplayTag PoolTag, TSubclassOf<AActor> ActorClass, int32 Count)
{
	for (int32 Index = 0; Index < Count; ++Index)
	{
		if (AActor* Actor = AcquireActor(PoolTag, ActorClass, FTransform::Identity))
		{
			ReleaseActor(Actor, PoolTag);
		}
	}
}