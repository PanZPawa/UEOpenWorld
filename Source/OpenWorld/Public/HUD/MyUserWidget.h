// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class OPENWORLD_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void SetMyHealth(float Percent);
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* MyHealth;
};
