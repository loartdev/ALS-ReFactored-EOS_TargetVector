// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SkeletalMeshMergerLibrary.generated.h"

/**
 * 等效于FSkeletalMeshMergeSectionMapping的蓝图结构体
 * 用于将单个源骨架网格体的所有分段映射到
 * 合并后骨架网格体中的最后一个分段条目的信息。
 */
USTRUCT(BlueprintType)
struct SKELETALMESHMERGER_API FSkeletalMeshMergeSectionMapping
{
	GENERATED_BODY()
	
	/**合并后骨架网格体的最终分段条目的索引*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Merge Params")
	TArray<int32> SectionIDs;
};

/**
 * 用于包含一个网格体的一组UV变换。
 */
USTRUCT(BlueprintType)
struct SKELETALMESHMERGER_API FSkeletalMeshMergeUVTransform
{
	GENERATED_BODY()
	
	/** 在给定网格体上应如何变换UV的列表，其中索引表示具体的UV通道。*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Merge Params")
	TArray<FTransform> UVTransforms;
};

/**
 * 等效于FSkeletalMeshMergeUVTransforms的蓝图
 * 用于映射所有分段的有关如何变换UV的信息
 */
USTRUCT(BlueprintType)
struct SKELETALMESHMERGER_API FSkeletalMeshMergeUVTransformMapping
{
	GENERATED_BODY()
	
	/** 对于每个网格体上的每个UV通道，描述应如何变换UVS。*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Merge Params")
	TArray<FSkeletalMeshMergeUVTransform> UVTransformsPerMesh;
};

/**
 * 包含用于执行骨架网格体合并的所有参数的结构。
 */
USTRUCT(BlueprintType)
struct SKELETALMESHMERGER_API FSkeletalMeshMergeParams
{
	GENERATED_BODY()
	
	FSkeletalMeshMergeParams()
	{
		StripTopLODS = 0;
		bNeedsCpuAccess = false;
		bSkeletonBefore = false;
		Skeleton = nullptr;
	}
	
	// 一个可选数组，用于将源网格体的分段映射到合并后的分段条目
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkeletalMeshMergeSectionMapping> MeshSectionMappings;
	
	// 一个可选数组，用于变换每个网格体中的UV
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkeletalMeshMergeUVTransformMapping> UVTransformsPerMesh;
	
	// 要合并的骨架网格体的列表。
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<USkeletalMesh*> MeshesToMerge;
	
	// 要从输入网格体移除的高LOD的数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StripTopLODS;
	
	// CPU是否会因任何原因（例如，产生粒子效果）而需要访问所产生的网格体。
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bNeedsCpuAccess :1;
	
	// 先更新骨架再合并。否则，合并后更新。
	// 还必须提供骨架。
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bSkeletonBefore :1;
	
	// 将用于合并后网格体的骨架。
	// 如果生成的骨架正常，则留空。
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeleton* Skeleton;
};

/**
 * 
 */
UCLASS()
class SKELETALMESHMERGER_API USkeletalMeshMergerLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/**
     * 将指定的网格体合并为一个网格体。
     * @return合并的网格体（如果合并失败将失效）。
     */
	UFUNCTION(BlueprintCallable, Category = "Mesh Merge", meta = (UnsafeDuringActorConstruction = "true"))
	static class USkeletalMesh* MergeMeshes(const FSkeletalMeshMergeParams& Params);
};
