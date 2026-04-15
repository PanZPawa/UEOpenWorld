// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MyHUD.h"
#include "HUD/MyUserWidget.h"
void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World) {
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && MyUserWidgetClass) {
			UMyUserWidget *MyUserWidget =  CreateWidget<UMyUserWidget>(Controller,MyUserWidgetClass);
			MyUserWidget->AddToViewport();
		}
	}

}
