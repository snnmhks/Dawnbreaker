// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "SmoothMoveCameraComponent.generated.h"

DECLARE_DELEGATE(FOnCameraMoveDelegate);

/**
 * ī�޶� ��ü���� �ڿ������� �������� �����ϱ� ���� ���� ������Ʈ
 *
 * ������ ĳ���� tick���� ó���ϴٺ��� �������� �������� �ð����� �ʱ�ȭ�� �ؾ��� ��
 * ��� Ÿ�ֿ̹��� �ʱ�ȭ�� ������ϴ� ���ŷο��� �־���
 *
 * �̷��� ���� ������Ʈ�� ����� ī�޶� ��ü���� Ÿ�̹��� ���Ҽ� �����Ƿ�
 * ���� �ʱ�ȭ�� �����ϴ�.
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class CD2_API USmoothMoveCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	// ī�޶� �ִ� �̵� �ð�
	// ī�޶� �����ϱ� ������ ���� ��ġ�� ��� ����Ǹ�
	// ������ġ�� �����Ǳ� ������ ī�޶� �������� ���ϴ� ��Ȳ�� �߻��ϹǷ�
	// �ִ� �ð��� �ξ� ������ ī�޶� �������� ���߰��Ѵ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move")
	float MaxMoveTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move")
	float MoveSpeed;

	UPROPERTY()
	float CurrentMoveTime;

	UPROPERTY()
	bool bIsCameraMoveable;

	// ī�޶� �̵��ؾ��� ��ġ
	// ��ġ�� ������ �ٲ�� �����Ƿ� ĳ���Ϳ� �ͼӵ� ���𰡸� �޾ƿͼ�
	// ��� ��ġ�� ����������Ѵ�.
	UPROPERTY()
	class USceneComponent* Target = nullptr;

	// ī�޶� �̵� ������ �˸��� �Լ�
	UFUNCTION()
	void SmoothMoveToTargetLocation(USceneComponent* _Target);

	// ī�޶� �̵� ����
	UFUNCTION()
	void SmoothMove(float DeltaTime);

	// ī�޶� �̵��� ������ �˸��� �Լ�
	UFUNCTION()
	void SmoothMoveEnd();

	// ī�޶� �̵��� ������ �߻��� ��������Ʈ
	// ĳ���Ϳ��� �� ��������Ʈ�� Attach���� �Լ��� ���ε��Ѵ�.
	FOnCameraMoveDelegate OnCameraMoveEnd;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
