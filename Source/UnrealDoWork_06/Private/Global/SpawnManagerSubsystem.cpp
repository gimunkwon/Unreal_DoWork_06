#include "UnrealDoWork_06/Public/Global/SpawnManagerSubsystem.h"
#include "UnrealDoWork_06/Public/Global/ObjectPoolSubsystem.h"
#include "UnrealDoWork_06/Public/Global/MyTagsContanier.h"
#include "UnrealDoWork_06/Public/PooledObject/PoolingMonster.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

USpawnManagerSubsystem::USpawnManagerSubsystem()
{
	// 메시 등이 셋업된 BP_Monster를 우선 사용하고, 못 찾으면 C++ 기본 클래스로 대체
	static ConstructorHelpers::FClassFinder<APoolingMonster> MonsterBPClass(TEXT("/Game/Actor/BP_Monster"));
	MonsterClass = MonsterBPClass.Class ? static_cast<UClass*>(MonsterBPClass.Class) : APoolingMonster::StaticClass();

	// 같은 BP_Monster를 서로 다른 풀(태그)로 분리해서 관리 - 풀링 메커니즘 검증용
	MonsterPoolTags = {
		ObjectPoolTags::Monster_TypeA.GetTag(),
		ObjectPoolTags::Monster_TypeB.GetTag()
	};
}

void USpawnManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	PlayerPawn = UGameplayStatics::GetPlayerPawn(&InWorld, 0);

	InWorld.GetTimerManager().SetTimer(SpawnCheckTimerHandle, this, &USpawnManagerSubsystem::CheckSpawnAndDespawn, CheckInterval, true);
}

void USpawnManagerSubsystem::Deinitialize()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SpawnCheckTimerHandle);
	}

	ActiveMonsters.Empty();

	Super::Deinitialize();
}

void USpawnManagerSubsystem::CheckSpawnAndDespawn()
{
	if (!IsValid(PlayerPawn) || !MonsterClass || MonsterPoolTags.Num() == 0)
	{
		return;
	}

	UObjectPoolSubsystem* Pool = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	if (!Pool)
	{
		return;
	}

	const FVector PlayerLocation = PlayerPawn->GetActorLocation();

	// 1. 회수 거리(DespawnDistance)를 벗어난 몬스터를, 자신이 속했던 태그의 풀로 반납
	for (int32 Index = ActiveMonsters.Num() - 1; Index >= 0; --Index)
	{
		APoolingMonster* Monster = ActiveMonsters[Index];
		const bool bOutOfRange = !IsValid(Monster) || FVector::Dist(Monster->GetActorLocation(), PlayerLocation) > DespawnDistance;

		if (bOutOfRange)
		{
			ActiveMonsters.RemoveAt(Index);

			if (IsValid(Monster))
			{
				Pool->ReleaseActor(Monster, Monster->PoolTag);
			}
		}
	}

	// 2. 목표 개체 수보다 부족하면 풀 태그를 무작위로 골라 해당 풀에서 꺼내 재배치
	while (ActiveMonsters.Num() < TargetActiveCount)
	{
		const FGameplayTag SelectedTag = GetRandomMonsterPoolTag();
		const FVector SpawnLocation = GetRandomSpawnLocation(PlayerLocation);
		const FTransform SpawnTransform(FRotator::ZeroRotator, SpawnLocation);

		APoolingMonster* Monster = Cast<APoolingMonster>(Pool->AcquireActor(SelectedTag, MonsterClass, SpawnTransform));
		if (!Monster)
		{
			break;
		}

		ActiveMonsters.Add(Monster);
	}
}

FGameplayTag USpawnManagerSubsystem::GetRandomMonsterPoolTag() const
{
	return MonsterPoolTags[FMath::RandRange(0, MonsterPoolTags.Num() - 1)];
}

FVector USpawnManagerSubsystem::GetRandomSpawnLocation(const FVector& PlayerLocation) const
{
	const float Angle = FMath::RandRange(0.0f, 2.0f * PI);
	const float Distance = FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);

	return PlayerLocation + FVector(FMath::Cos(Angle) * Distance, FMath::Sin(Angle) * Distance, 0.0f);
}