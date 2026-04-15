#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
    ECS_Unequipped UMETA(DisplayName = "Unequipped"),
    ECS_EquippedOneHandWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"),
    ECS_EquippedTwoHandWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon"),
    ECS_Alive UMETA(DisplayName = "Is Live"),
    ECS_Die UMETA(DisplayName = "Is Died")
};
UENUM(BlueprintType)
enum class EActionState : uint8
{
    ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
    ECS_Attacking UMETA(DisplayName = "Attacking"),
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{   
    EES_Unoccupied UMETA(DisplayName = "Unoccupied"),
    EES_Patrolling UMETA(DisplayName = "Patrolling"),
    EES_Chasing UMETA(DisplayName ="Chasing"),
    EES_Attacking UMETA(DisplayName = "Attacking"),
    EES_Reacting UMETA(DisplayName = "Reacting")
};