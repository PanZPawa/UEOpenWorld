// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyUserWidget.h"
#include "Components/ProgressBar.h"
void UMyUserWidget::SetMyHealth(float Percent)
{
	if (MyHealth) {
		MyHealth->SetPercent(Percent);
	}

}
