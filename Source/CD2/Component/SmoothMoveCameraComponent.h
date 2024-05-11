// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "SmoothMoveCameraComponent.generated.h"

DECLARE_DELEGATE(FOnCameraMoveDelegate);

/**
 * 카메라 자체에서 자연스러운 움직임을 구현하기 위해 만든 컴포넌트
 *
 * 기존엔 캐릭터 tick에서 처리하다보니 가독성이 떨어지고 시간변수 초기화를 해야할 때
 * 모든 타이밍에서 초기화를 해줘야하는 번거로움이 있었음
 *
 * 이렇게 따로 컴포넌트를 만들면 카메라 자체에서 타이밍을 정할수 있으므로
 * 변수 초기화에 용이하다.
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class CD2_API USmoothMoveCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	// 카메라 최대 이동 시간
	// 카메라가 도착하기 직전에 도착 위치가 계속 변경되면
	// 도착위치가 고정되기 전가지 카메라가 도착하지 못하는 상황이 발생하므로
	// 최대 시간을 두어 강제로 카메라 움직임을 멈추게한다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move")
	float MaxMoveTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move")
	float MoveSpeed;

	UPROPERTY()
	float CurrentMoveTime;

	UPROPERTY()
	bool bIsCameraMoveable;

	// 카메라가 이동해야할 위치
	// 위치는 언제나 바뀔수 있으므로 캐릭터에 귀속된 무언가를 받아와서
	// 계속 위치를 갱신해줘야한다.
	UPROPERTY()
	class USceneComponent* Target = nullptr;

	// 카메라 이동 시작을 알리는 함수
	UFUNCTION()
	void SmoothMoveToTargetLocation(USceneComponent* _Target);

	// 카메라 이동 로직
	UFUNCTION()
	void SmoothMove(float DeltaTime);

	// 카메라 이동이 끝남을 알리는 함수
	UFUNCTION()
	void SmoothMoveEnd();

	// 카메라 이동이 끝나면 발생할 델리게이트
	// 캐릭터에서 이 델리게이트에 Attach관련 함수를 바인딩한다.
	FOnCameraMoveDelegate OnCameraMoveEnd;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
