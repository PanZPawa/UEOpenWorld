// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthyBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"
void UHealthyBarComponent::SetHealthPercent(float Percent)
{
	if(!HealthBarWidget)HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	
	if (HealthBarWidget &&HealthBarWidget->HealthBar) {
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}

}
