// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthyBarComponent.generated.h"

/**
 * 
 */
class UHealthBar;
UCLASS()
class OPENWORLD_API UHealthyBarComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	void SetHealthPercent(float Percent);

private:
	UHealthBar* HealthBarWidget;

};
