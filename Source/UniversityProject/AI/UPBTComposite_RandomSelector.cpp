// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/UPBTComposite_RandomSelector.h"

UUPBTComposite_RandomSelector::UUPBTComposite_RandomSelector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), LastSuccessfulChildIdx(INDEX_NONE)
{
	NodeName = "Random Selector";
}

void UUPBTComposite_RandomSelector::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTMemoryInit::Type InitType) const
{
	InitializeNodeMemory<FBTCompositeMemory>(NodeMemory, InitType);
	ExecutedChildren.Empty();
	LastSuccessfulChildIdx = INDEX_NONE;
}

void UUPBTComposite_RandomSelector::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTMemoryClear::Type CleanupType) const
{
	CleanupNodeMemory<FBTCompositeMemory>(NodeMemory, CleanupType);
	ExecutedChildren.Empty();
	LastSuccessfulChildIdx = INDEX_NONE;
}

int32 UUPBTComposite_RandomSelector::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild,
	EBTNodeResult::Type LastResult) const
{
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;
 
	if (PrevChild == BTSpecialChild::NotInitialized)
	{
		// 랜덤 인덱스 생성
		NextChildIdx = FMath::RandRange(0, GetChildrenNum() - 1);
	}
	else
	{
		// 마지막 결과 처리
		if (LastResult == EBTNodeResult::Succeeded)
		{
			ExecutedChildren.Empty();
			return BTSpecialChild::ReturnToParent;
		}
		else if (LastResult == EBTNodeResult::Failed)
		{
			// If last child failed, mark it as executed
			ExecutedChildren.Add(PrevChild);
 
			// 모든 자식이 실패하면 Fail 반환
			if (ExecutedChildren.Num() >= GetChildrenNum())
			{
				ExecutedChildren.Empty();
				return BTSpecialChild::ReturnToParent;
			}
 
			// 아직 실행되지 않은 자식 노드들 검사
			TArray<int32> AvailableChildren;
			for (int32 ChildIdx = 0; ChildIdx < GetChildrenNum(); ++ChildIdx)
			{
				if (!ExecutedChildren.Contains(ChildIdx))
				{
					AvailableChildren.Add(ChildIdx);
				}
			}
 
			// 실행 가능한 랜덤 자식 노드 선택
			NextChildIdx = AvailableChildren[FMath::RandRange(0, AvailableChildren.Num() - 1)];
		}
	}
 
	return NextChildIdx;
}
