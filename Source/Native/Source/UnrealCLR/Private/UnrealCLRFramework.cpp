/*
 *  Copyright (c) 2020 Stanislav Denisov (nxrighthere@gmail.com)
 *
 *  Permission to use, copy, modify, and/or distribute this software free of
 *  charge is hereby granted, provided that the above copyright notice and this
 *  permission notice appear in all copies or portions of this software with
 *  respect to the following terms and conditions:
 *
 *  1. Without specific prior written permission of the copyright holder,
 *  this software is forbidden for rebranding, sublicensing, and the exploitation
 *  to get payments in any form.
 *
 *  2. In accordance with DMCA (Digital Millennium Copyright Act), the copyright
 *  holder reserves exclusive permission to take down at any time any publicly
 *  available copy of this software in the original, partial, or modified form.
 *
 *  3. Any modifications that were made by third-parties to this software or its
 *  portions can be used by the copyright holder for any purposes, without any
 *  limiting factors and restrictions.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 *  OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 *  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "UnrealCLRFramework.h"

DEFINE_LOG_CATEGORY(LogUnrealManaged);
DEFINE_LOG_CATEGORY(LogUnrealAssert);

namespace UnrealCLRFramework {
	#define UNREALCLR_GET_ATTACHMENT_RULE(Rule, Result) {\
		switch (Rule) {\
			case AttachmentTransformRule::KeepRelativeTransform:\
				Result = FAttachmentTransformRules::KeepRelativeTransform;\
				break;\
			case AttachmentTransformRule::KeepWorldTransform:\
				Result = FAttachmentTransformRules::KeepWorldTransform;\
				break;\
			case AttachmentTransformRule::SnapToTargetIncludingScale:\
				Result = FAttachmentTransformRules::SnapToTargetIncludingScale;\
				break;\
			case AttachmentTransformRule::SnapToTargetNotIncludingScale:\
				Result = FAttachmentTransformRules::SnapToTargetNotIncludingScale;\
				break;\
			default:\
				break;\
		}\
	}

	#define UNREALCLR_GET_ACTOR_TYPE(Type, Head, Tail, Result) {\
		switch (Type) {\
			case ActorType::Base:\
				Result = Head AActor Tail;\
				break;\
			case ActorType::Camera:\
				Result = Head ACameraActor Tail;\
				break;\
			case ActorType::Pawn:\
				Result = Head APawn Tail;\
				break;\
			case ActorType::Character:\
				Result = Head ACharacter Tail;\
				break;\
			case ActorType::AIController:\
				Result = Head AAIController Tail;\
				break;\
			case ActorType::PlayerController:\
				Result = Head APlayerController Tail;\
				break;\
			case ActorType::Brush:\
				Result = Head ABrush Tail;\
				break;\
			case ActorType::AmbientSound:\
				Result = Head AAmbientSound Tail;\
				break;\
			case ActorType::DirectionalLight:\
				Result = Head ADirectionalLight Tail;\
				break;\
			case ActorType::PointLight:\
				Result = Head APointLight Tail;\
				break;\
			case ActorType::RectLight:\
				Result = Head ARectLight Tail;\
				break;\
			case ActorType::SpotLight:\
				Result = Head ASpotLight Tail;\
				break;\
			default:\
				break;\
		}\
	}

	#define UNREALCLR_GET_COMPONENT_TYPE(Type, Head, Tail, Result) {\
		switch (Type) {\
			case ComponentType::Actor:\
				Result = Head UActorComponent Tail;\
				break;\
			case ComponentType::Input:\
				Result = Head UInputComponent Tail;\
				break;\
			case ComponentType::Scene:\
				Result = Head USceneComponent Tail;\
				break;\
			case ComponentType::Audio:\
				Result = Head UAudioComponent Tail;\
				break;\
			case ComponentType::Camera:\
				Result = Head UCameraComponent Tail;\
				break;\
			case ComponentType::MotionController:\
				Result = Head UMotionControllerComponent Tail;\
				break;\
			case ComponentType::StaticMesh:\
				Result = Head UStaticMeshComponent Tail;\
				break;\
			case ComponentType::InstancedStaticMesh:\
				Result = Head UInstancedStaticMeshComponent Tail;\
				break;\
			case ComponentType::Box:\
				Result = Head UBoxComponent Tail;\
				break;\
			case ComponentType::Sphere:\
				Result = Head USphereComponent Tail;\
				break;\
			case ComponentType::Capsule:\
				Result = Head UCapsuleComponent Tail;\
				break;\
			case ComponentType::SkeletalMesh:\
				Result = Head USkeletalMeshComponent Tail;\
				break;\
			default:\
				break;\
		}\
	}

	#define UNREALCLR_GET_ATTACHABLE_COMPONENT_TYPE(Type, Head, Tail, Result) {\
		switch (Type) {\
			case ComponentType::Scene:\
				Result = Head USceneComponent Tail;\
				break;\
			case ComponentType::Audio:\
				Result = Head UAudioComponent Tail;\
				break;\
			case ComponentType::Camera:\
				Result = Head UCameraComponent Tail;\
				break;\
			case ComponentType::MotionController:\
				Result = Head UMotionControllerComponent Tail;\
				break;\
			case ComponentType::StaticMesh:\
				Result = Head UStaticMeshComponent Tail;\
				break;\
			case ComponentType::InstancedStaticMesh:\
				Result = Head UInstancedStaticMeshComponent Tail;\
				break;\
			case ComponentType::Box:\
				Result = Head UBoxComponent Tail;\
				break;\
			case ComponentType::Sphere:\
				Result = Head USphereComponent Tail;\
				break;\
			case ComponentType::Capsule:\
				Result = Head UCapsuleComponent Tail;\
				break;\
			case ComponentType::SkeletalMesh:\
				Result = Head USkeletalMeshComponent Tail;\
				break;\
			default:\
				break;\
		}\
	}

	#define UNREALCLR_SET_BONE_NAME(Name)\
		FName boneName;\
		if (!Name)\
			boneName = NAME_None;\
		else\
			boneName = FName(ANSI_TO_TCHAR(Name));

	#define UNREALCLR_COLOR_TO_INTEGER(Color) (Color.A << 24) + (Color.R << 16) + (Color.G << 8) + Color.B

	namespace Assert {
		void OutputMessage(const char* Message) {
			FString message(ANSI_TO_TCHAR(Message));

			UE_LOG(LogUnrealAssert, Error, TEXT("%s: %s"), ANSI_TO_TCHAR(__FUNCTION__), *message);

			GEngine->AddOnScreenDebugMessage((uint64)-1, 60.0f, FColor::Red, *message);
		}
	}

	namespace CommandLine {
		void Get(char* Arguments) {
			const char* arguments = TCHAR_TO_ANSI(FCommandLine::Get());

			UnrealCLR::Utility::Strcpy(Arguments, arguments, UnrealCLR::Utility::Strlen(arguments));
		}

		void Set(const char* Arguments) {
			FCommandLine::Set(ANSI_TO_TCHAR(Arguments));
		}

		void Append(const char* Arguments) {
			FCommandLine::Append(ANSI_TO_TCHAR(Arguments));
		}
	}

	namespace Debug {
		void Log(LogLevel Level, const char* Message) {
			#define UNREALCLR_FRAMEWORK_LOG(Verbosity) UE_LOG(LogUnrealManaged, Verbosity, TEXT("%s: %s"), ANSI_TO_TCHAR(__FUNCTION__), *FString(ANSI_TO_TCHAR(Message)));

			if (Level == LogLevel::Display) {
				UNREALCLR_FRAMEWORK_LOG(Display);
			} else if (Level == LogLevel::Warning) {
				UNREALCLR_FRAMEWORK_LOG(Warning);
			} else if (Level == LogLevel::Error) {
				UNREALCLR_FRAMEWORK_LOG(Error);
			} else if (Level == LogLevel::Fatal) {
				UNREALCLR_FRAMEWORK_LOG(Fatal);
			}
		}

		void HandleException(const char* Message) {
			GEngine->AddOnScreenDebugMessage((uint64)-1, 60.0f, FColor::Red, *FString(ANSI_TO_TCHAR(Message)));
		}

		void AddOnScreenMessage(int32 Key, float TimeToDisplay, Color DisplayColor, const char* Message) {
			GEngine->AddOnScreenDebugMessage((uint64)Key, TimeToDisplay, DisplayColor, *FString(ANSI_TO_TCHAR(Message)));
		}

		void ClearOnScreenMessages() {
			GEngine->ClearOnScreenDebugMessages();
		}

		void DrawBox(const Vector3* Center, const Vector3* Extent, const Quaternion* Rotation, Color Color, bool PersistentLines, float LifeTime, uint8 DepthPriority, float Thickness) {
			DrawDebugBox(UnrealCLR::Engine::World, *Center, *Extent, *Rotation, Color, PersistentLines, LifeTime, DepthPriority, Thickness);
		}

		void DrawCapsule(const Vector3* Center, float HalfHeight, float Radius, const Quaternion* Rotation, Color Color, bool PersistentLines, float LifeTime, uint8 DepthPriority, float Thickness) {
			DrawDebugCapsule(UnrealCLR::Engine::World, *Center, HalfHeight, Radius, *Rotation, Color, PersistentLines, LifeTime, DepthPriority, Thickness);
		}

		void DrawCone(const Vector3* Origin, const Vector3* Direction, float Length, float AngleWidth, float AngleHeight, int32 Sides, Color Color, bool PersistentLines, float LifeTime, uint8 DepthPriority, float Thickness) {
			DrawDebugCone(UnrealCLR::Engine::World, *Origin, *Direction, Length, AngleWidth, AngleHeight, Sides, Color, PersistentLines, LifeTime, DepthPriority, Thickness);
		}

		void DrawCylinder(const Vector3* Start, const Vector3* End, float Radius, int32 Segments, Color Color, bool PersistentLines, float LifeTime, uint8 DepthPriority, float Thickness) {
			DrawDebugCylinder(UnrealCLR::Engine::World, *Start, *End, Radius, Segments, Color, PersistentLines, LifeTime, DepthPriority, Thickness);
		}

		void DrawSphere(const Vector3* Center, float Radius, int32 Segments, Color Color, bool PersistentLines, float LifeTime, uint8 DepthPriority, float Thickness) {
			DrawDebugSphere(UnrealCLR::Engine::World, *Center, Radius, Segments, Color, PersistentLines, LifeTime, DepthPriority, Thickness);
		}

		void DrawLine(const Vector3* Start, const Vector3* End, Color Color, bool PersistentLines, float LifeTime, uint8 DepthPriority, float Thickness) {
			DrawDebugLine(UnrealCLR::Engine::World, *Start, *End, Color, PersistentLines, LifeTime, DepthPriority, Thickness);
		}

		void DrawPoint(const Vector3* Location, float Size, Color Color, bool PersistentLines, float LifeTime, uint8 DepthPriority) {
			DrawDebugPoint(UnrealCLR::Engine::World, *Location, Size, Color, PersistentLines, LifeTime, DepthPriority);
		}

		void FlushPersistentLines() {
			FlushPersistentDebugLines(UnrealCLR::Engine::World);
		}
	}

	namespace Object {
		bool IsPendingKill(UObject* Object) {
			return Object->IsPendingKill();
		}

		bool IsValid(UObject* Object) {
			return Object->IsValidLowLevel();
		}

		UObject* Load(ObjectType Type, const char* Name) {
			UObject* object = nullptr;

			switch (Type) {
				case ObjectType::Blueprint: {
					#if WITH_EDITOR
						object = StaticLoadObject(UBlueprint::StaticClass(), nullptr, *FString(ANSI_TO_TCHAR(Name)));
					#else
						FString name(ANSI_TO_TCHAR(Name));
						int32 index = INDEX_NONE;

						if (name.FindLastChar(TCHAR('/'), index)) {
							name.AppendChar(TCHAR('.'));
							name.Append(name.Mid(index + 1, name.Len() - index - 2));
						}

						name.Append(TEXT("_C"));

						object = StaticLoadObject(UClass::StaticClass(), nullptr, *name);
					#endif
					break;
				}

				case ObjectType::SoundWave: {
					object = StaticLoadObject(USoundWave::StaticClass(), nullptr, *FString(ANSI_TO_TCHAR(Name)));
					break;
				}

				case ObjectType::AnimationSequence: {
					object = StaticLoadObject(UAnimSequence::StaticClass(), nullptr, *FString(ANSI_TO_TCHAR(Name)));
					break;
				}

				case ObjectType::AnimationMontage: {
					object = StaticLoadObject(UAnimMontage::StaticClass(), nullptr, *FString(ANSI_TO_TCHAR(Name)));
					break;
				}

				case ObjectType::StaticMesh: {
					object = StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *FString(ANSI_TO_TCHAR(Name)));
					break;
				}

				case ObjectType::SkeletalMesh: {
					object = StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *FString(ANSI_TO_TCHAR(Name)));
					break;
				}

				case ObjectType::Material: {
					object = StaticLoadObject(UMaterial::StaticClass(), nullptr, *FString(ANSI_TO_TCHAR(Name)));
					break;
				}

				default:
					break;
			}

			return object;
		}

		void Rename(UObject* Object, const char* Name) {
			FString name(ANSI_TO_TCHAR(Name));

			Object->Rename(*name);
		}

		void GetName(UObject* Object, char* Name) {
			const char* name = TCHAR_TO_ANSI(*Object->GetName());

			UnrealCLR::Utility::Strcpy(Name, name, UnrealCLR::Utility::Strlen(name));
		}
	}

	namespace Application {
		bool IsCanEverRender() {
			return FApp::CanEverRender();
		}

		bool IsPackagedForDistribution() {
			return FGenericPlatformMisc::IsPackagedForDistribution();
		}

		bool IsPackagedForShipping() {
			#if UE_BUILD_SHIPPING
				return true;
			#else
				return false;
			#endif
		}

		void GetProjectDirectory(char* Directory) {
			const char* directory = TCHAR_TO_ANSI(FGenericPlatformMisc::ProjectDir());

			UnrealCLR::Utility::Strcpy(Directory, directory, UnrealCLR::Utility::Strlen(directory));
		}

		void GetDefaultLanguage(char* Language) {
			const char* language = TCHAR_TO_ANSI(*FGenericPlatformMisc::GetDefaultLanguage());

			UnrealCLR::Utility::Strcpy(Language, language, UnrealCLR::Utility::Strlen(language));
		}

		void GetProjectName(char* ProjectName) {
			const char* projectName = TCHAR_TO_ANSI(FApp::GetProjectName());

			UnrealCLR::Utility::Strcpy(ProjectName, projectName, UnrealCLR::Utility::Strlen(projectName));
		}

		float GetVolumeMultiplier() {
			return FApp::GetVolumeMultiplier();
		}

		void SetProjectName(const char* ProjectName) {
			FApp::SetProjectName(ANSI_TO_TCHAR(ProjectName));
		}

		void SetVolumeMultiplier(float Value) {
			FApp::SetVolumeMultiplier(Value);
		}
	}

	namespace ConsoleManager {
		bool IsRegisteredVariable(const char* Name) {
			return IConsoleManager::Get().IsNameRegistered(ANSI_TO_TCHAR(Name));
		}

		IConsoleVariable* FindVariable(const char* Name) {
			return IConsoleManager::Get().FindConsoleVariable(ANSI_TO_TCHAR(Name));
		}

		IConsoleVariable* RegisterVariableBool(const char* Name, const char* Help, bool DefaultValue, bool ReadOnly) {
			return IConsoleManager::Get().RegisterConsoleVariable(ANSI_TO_TCHAR(Name), DefaultValue, ANSI_TO_TCHAR(Help), !ReadOnly ? ECVF_Default : ECVF_ReadOnly);
		}

		IConsoleVariable* RegisterVariableInt(const char* Name, const char* Help, int32 DefaultValue, bool ReadOnly) {
			return IConsoleManager::Get().RegisterConsoleVariable(ANSI_TO_TCHAR(Name), DefaultValue, ANSI_TO_TCHAR(Help), !ReadOnly ? ECVF_Default : ECVF_ReadOnly);
		}

		IConsoleVariable* RegisterVariableFloat(const char* Name, const char* Help, float DefaultValue, bool ReadOnly) {
			return IConsoleManager::Get().RegisterConsoleVariable(ANSI_TO_TCHAR(Name), DefaultValue, ANSI_TO_TCHAR(Help), !ReadOnly ? ECVF_Default : ECVF_ReadOnly);
		}

		IConsoleVariable* RegisterVariableString(const char* Name, const char* Help, const char* DefaultValue, bool ReadOnly) {
			return IConsoleManager::Get().RegisterConsoleVariable(ANSI_TO_TCHAR(Name), ANSI_TO_TCHAR(DefaultValue), ANSI_TO_TCHAR(Help), !ReadOnly ? ECVF_Default : ECVF_ReadOnly);
		}

		void RegisterCommand(const char* Name, const char* Help, ConsoleCommandDelegate Function, bool ReadOnly) {
			auto function = [Function](const TArray<FString>& Arguments) {
				if (UnrealCLR::Status == UnrealCLR::StatusType::Running) {
					float value = 0.0f;

					if (Arguments.Num() > 0)
						FDefaultValueHelper::ParseFloat(Arguments[0], value);

					Function(value);
				}
			};

			IConsoleManager::Get().RegisterConsoleCommand(ANSI_TO_TCHAR(Name), ANSI_TO_TCHAR(Help), FConsoleCommandWithArgsDelegate::CreateLambda(function), !ReadOnly ? ECVF_Default : ECVF_ReadOnly);
		}

		void UnregisterObject(const char* Name) {
			IConsoleManager::Get().UnregisterConsoleObject(ANSI_TO_TCHAR(Name), false);
		}
	}

	namespace Engine {
		bool IsSplitScreen() {
			return GEngine->IsSplitScreen(UnrealCLR::Engine::World);
		}

		bool IsEditor() {
			return UnrealCLR::Engine::World->IsPlayInEditor();
		}

		bool IsForegroundWindow() {
			return UnrealCLR::Engine::World->GetGameViewport()->Viewport->IsForegroundWindow();
		}

		bool IsExitRequested() {
			return IsEngineExitRequested();
		}

		NetMode GetNetMode() {
			return UnrealCLR::Engine::World->GetNetMode();
		}

		uint32 GetFrameNumber() {
			return GFrameNumber;
		}

		void GetViewportSize(Vector2* Value) {
			*Value = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
		}

		void GetScreenResolution(Vector2* Value) {
			*Value = FVector2D(static_cast<float>(GSystemResolution.ResX), static_cast<float>(GSystemResolution.ResY));
		}

		WindowMode GetWindowMode() {
			return UnrealCLR::Engine::World->GetGameViewport()->Viewport->GetWindowMode();
		}

		void GetVersion(char* Version) {
			const char* version = TCHAR_TO_ANSI(*FEngineVersion::Current().ToString());

			UnrealCLR::Utility::Strcpy(Version, version, UnrealCLR::Utility::Strlen(version));
		}

		float GetMaxFPS() {
			return GEngine->GetMaxFPS();
		}

		void SetMaxFPS(float MaxFPS) {
			GEngine->SetMaxFPS(MaxFPS);
		}

		void SetTitle(const char* Title) {
			UGameEngine* gameEngine = Cast<UGameEngine>(GEngine);

			if (gameEngine) {
				TSharedPtr<SWindow> gameViewportWindow = gameEngine->GameViewportWindow.Pin();

				if (gameViewportWindow.IsValid())
					gameViewportWindow->SetTitle(FText::FromString(FString(ANSI_TO_TCHAR(Title))));
			}
		}

		void AddActionMapping(const char* ActionName, const char* Key, bool Shift, bool Ctrl, bool Alt, bool Cmd) {
			UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(FName(ANSI_TO_TCHAR(ActionName)), FKey(ANSI_TO_TCHAR(Key)), Shift, Ctrl, Alt, Cmd));
		}

		void AddAxisMapping(const char* AxisName, const char* Key, float Scale) {
			UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping(FName(ANSI_TO_TCHAR(AxisName)), FKey(ANSI_TO_TCHAR(Key)), Scale));
		}

		void ForceGarbageCollection(bool FullPurge) {
			GEngine->ForceGarbageCollection(FullPurge);
		}

		void DelayGarbageCollection() {
			GEngine->DelayGarbageCollection();
		}

		void RequestExit(bool Force) {
			FGenericPlatformMisc::RequestExit(Force);
		}
	}

	namespace HeadMountedDisplay {
		bool IsEnabled() {
			return UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
		}

		void GetDeviceName(char* Name) {
			FName deviceName = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName();

			const char* name = TCHAR_TO_ANSI(*deviceName.ToString());

			UnrealCLR::Utility::Strcpy(Name, name, UnrealCLR::Utility::Strlen(name));
		}
	}

	namespace World {
		bool GetSimulatePhysics() {
			return UnrealCLR::Engine::World->bShouldSimulatePhysics;
		}

		int32 GetActorCount() {
			return UnrealCLR::Engine::World->GetActorCount();
		}

		float GetDeltaSeconds() {
			return UnrealCLR::Engine::World->GetDeltaSeconds();
		}

		float GetRealTimeSeconds() {
			return UnrealCLR::Engine::World->GetRealTimeSeconds();
		}

		float GetTimeSeconds() {
			return UnrealCLR::Engine::World->GetTimeSeconds();
		}

		void GetWorldOrigin(Vector3* Value) {
			*Value = FVector(UnrealCLR::Engine::World->OriginLocation);
		}

		AActor* GetActor(const char* Name, ActorType Type) {
			FString name;
			AActor* actor = nullptr;
			TSubclassOf<AActor> type;

			if (Name)
				name = FString(ANSI_TO_TCHAR(Name));

			UNREALCLR_GET_ACTOR_TYPE(Type, UNREALCLR_NONE, ::StaticClass(), type);

			for (TActorIterator<AActor> currentActor(UnrealCLR::Engine::World, type); currentActor; ++currentActor) {
				if (!Name || Name && *currentActor->GetName() == name) {
					actor = *currentActor;
					break;
				}
			}

			return actor;
		}

		AActor* GetActorByTag(const char* Tag, ActorType Type) {
			FName tag(ANSI_TO_TCHAR(Tag));
			AActor* actor = nullptr;
			TSubclassOf<AActor> type;

			UNREALCLR_GET_ACTOR_TYPE(Type, UNREALCLR_NONE, ::StaticClass(), type);

			for (TActorIterator<AActor> currentActor(UnrealCLR::Engine::World, type); currentActor; ++currentActor) {
				if (currentActor->ActorHasTag(tag)) {
					actor = *currentActor;
					break;
				}
			}

			return actor;
		}

		APlayerController* GetFirstPlayerController() {
			return UnrealCLR::Engine::World->GetFirstPlayerController();
		}

		void SetSimulatePhysics(bool Value) {
			UnrealCLR::Engine::World->bShouldSimulatePhysics = Value;
		}

		void SetGravity(float Value) {
			AWorldSettings* worldSettings = UnrealCLR::Engine::World->GetWorldSettings();

			worldSettings->bGlobalGravitySet = true;
			worldSettings->GlobalGravityZ = Value;
		}

		void SetWorldOrigin(const Vector3* Value) {
			UnrealCLR::Engine::World->SetNewWorldOrigin(FIntVector(*Value));
		}
	}

	namespace ConsoleObject {
		bool IsBool(IConsoleObject* ConsoleObject) {
			return ConsoleObject->IsVariableBool();
		}

		bool IsInt(IConsoleObject* ConsoleObject) {
			return ConsoleObject->IsVariableInt();
		}

		bool IsFloat(IConsoleObject* ConsoleObject) {
			return ConsoleObject->IsVariableFloat();
		}

		bool IsString(IConsoleObject* ConsoleObject) {
			return ConsoleObject->IsVariableString();
		}
	}

	namespace ConsoleVariable {
		bool GetBool(IConsoleVariable* ConsoleVariable) {
			return ConsoleVariable->GetBool();
		}

		int32 GetInt(IConsoleVariable* ConsoleVariable) {
			return ConsoleVariable->GetInt();
		}

		float GetFloat(IConsoleVariable* ConsoleVariable) {
			return ConsoleVariable->GetFloat();
		}

		void GetString(IConsoleVariable* ConsoleVariable, char* Value) {
			const char* value = TCHAR_TO_ANSI(*ConsoleVariable->GetString());

			UnrealCLR::Utility::Strcpy(Value, value, UnrealCLR::Utility::Strlen(value));
		}

		void SetBool(IConsoleVariable* ConsoleVariable, bool Value) {
			ConsoleVariable->Set(Value);
		}

		void SetInt(IConsoleVariable* ConsoleVariable, int32 Value) {
			ConsoleVariable->Set(Value);
		}

		void SetFloat(IConsoleVariable* ConsoleVariable, float Value) {
			ConsoleVariable->Set(Value);
		}

		void SetString(IConsoleVariable* ConsoleVariable, const char* Value) {
			ConsoleVariable->Set(ANSI_TO_TCHAR(Value));
		}

		void SetOnChangedCallback(IConsoleVariable* ConsoleVariable, ConsoleVariableDelegate Function) {
			auto function = [Function](IConsoleVariable* ConsoleVariable) {
				Function();
			};

			ConsoleVariable->SetOnChangedCallback(FConsoleVariableDelegate::CreateLambda(function));
		}

		void ClearOnChangedCallback(IConsoleVariable* ConsoleVariable) {
			FConsoleVariableDelegate emptyDelegate;

			ConsoleVariable->SetOnChangedCallback(emptyDelegate);
		}
	}

	namespace Actor {
		bool IsPendingKill(AActor* Actor) {
			return Actor->IsPendingKillPending();
		}

		bool IsRootComponentMovable(AActor* Actor) {
			return Actor->IsRootComponentMovable();
		}

		bool IsOverlappingActor(AActor* Actor, AActor* Other) {
			return Actor->IsOverlappingActor(Other);
		}

		AActor* Spawn(const char* Name, ActorType Type, UObject* Blueprint) {
			AActor* actor = nullptr;

			if (!Blueprint) {
				UNREALCLR_GET_ACTOR_TYPE(Type, UnrealCLR::Engine::World->SpawnActor UNREALCLR_BRACKET_LEFT, ::StaticClass() UNREALCLR_BRACKET_RIGHT, actor);
			} else {
				#if !WITH_EDITOR
					UNREALCLR_GET_ACTOR_TYPE(Type, UnrealCLR::Engine::World->SpawnActor<, >(Cast<UClass>(Blueprint)), actor);
				#else
					UNREALCLR_GET_ACTOR_TYPE(Type, UnrealCLR::Engine::World->SpawnActor<, >(Cast<UBlueprint>(Blueprint)->GeneratedClass), actor);
				#endif
			}

			if (Name) {
				FString name(ANSI_TO_TCHAR(Name));

				actor->Rename(*name);

				#if WITH_EDITOR
					actor->SetActorLabel(*name);
				#endif
			}

			return actor;
		}

		bool Destroy(AActor* Actor) {
			return UnrealCLR::Engine::World->DestroyActor(Actor);
		}

		void Rename(AActor* Actor, const char* Name) {
			FString name(ANSI_TO_TCHAR(Name));

			Actor->Rename(*name);

			#if WITH_EDITOR
				Actor->SetActorLabel(*name);
			#endif
		}

		void Hide(AActor* Actor, bool Value) {
			Actor->SetActorHiddenInGame(Value);
		}

		void AttachToActor(AActor* Actor, AActor* Parent, AttachmentTransformRule AttachmentRule, const char* SocketName) {
			FAttachmentTransformRules attachmentRules = FAttachmentTransformRules::KeepRelativeTransform;
			FName socketName;

			UNREALCLR_GET_ATTACHMENT_RULE(AttachmentRule, attachmentRules);

			if (SocketName)
				socketName = FName(ANSI_TO_TCHAR(SocketName));

			Actor->AttachToActor(Parent, attachmentRules, socketName);
		}

		bool TeleportTo(AActor* Actor, const Vector3* DestinationLocation, const Quaternion* DestinationRotation, bool IsATest, bool NoCheck) {
			return Actor->TeleportTo(*DestinationLocation, FRotator(*DestinationRotation), IsATest, NoCheck);
		}

		UActorComponent* GetComponent(AActor* Actor, const char* Name, ComponentType Type) {
			FString name;
			UActorComponent* component = nullptr;
			TSubclassOf<UActorComponent> type;

			if (Name)
				name = FString(ANSI_TO_TCHAR(Name));

			UNREALCLR_GET_COMPONENT_TYPE(Type, UNREALCLR_NONE, ::StaticClass(), type);

			for (UActorComponent* currentComponent : Actor->GetComponents()) {
				if (currentComponent && currentComponent->IsA(type) && (!Name || Name && *currentComponent->GetName() == name)) {
					component = currentComponent;
					break;
				}
			}

			return component;
		}

		USceneComponent* GetRootComponent(AActor* Actor, ComponentType Type) {
			USceneComponent* component = nullptr;
			USceneComponent* rootComponent = Actor->GetRootComponent();
			TSubclassOf<UActorComponent> type;

			UNREALCLR_GET_ATTACHABLE_COMPONENT_TYPE(Type, UNREALCLR_NONE, ::StaticClass(), type);

			if (rootComponent->IsA(type))
				component = rootComponent;

			return component;
		}

		UInputComponent* GetInputComponent(AActor* Actor) {
			return Actor->InputComponent;
		}

		bool GetBlockInput(AActor* Actor) {
			return Actor->bBlockInput;
		}

		float GetDistanceTo(AActor* Actor, AActor* Other) {
			return Actor->GetDistanceTo(Other);
		}

		void GetBounds(AActor* Actor, bool OnlyCollidingComponents, Vector3* Origin, Vector3* Extent) {
			FVector origin, extent;

			Actor->GetActorBounds(OnlyCollidingComponents, origin, extent);

			*Origin = origin;
			*Extent = extent;
		}

		bool SetRootComponent(AActor* Actor, USceneComponent* RootComponent) {
			RootComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

			return Actor->SetRootComponent(RootComponent);
		}

		void SetInputComponent(AActor* Actor, UInputComponent* InputComponent) {
			Actor->InputComponent = InputComponent;
		}

		void SetBlockInput(AActor* Actor, bool Value) {
			Actor->bBlockInput = Value;
		}

		void SetLifeSpan(AActor* Actor, float LifeSpan) {
			Actor->SetLifeSpan(LifeSpan);
		}

		void SetEnableCollision(AActor* Actor, bool Value) {
			Actor->SetActorEnableCollision(Value);
		}

		void AddTag(AActor* Actor, const char* Tag) {
			Actor->Tags.AddUnique(FName(ANSI_TO_TCHAR(Tag)));
		}

		void RemoveTag(AActor* Actor, const char* Tag) {
			Actor->Tags.Remove(FName(ANSI_TO_TCHAR(Tag)));
		}

		bool HasTag(AActor* Actor, const char* Tag) {
			return Actor->ActorHasTag(FName(ANSI_TO_TCHAR(Tag)));
		}
	}

	namespace Pawn {
		void AddControllerYawInput(APawn* Pawn, float Value) {
			Pawn->AddControllerYawInput(Value);
		}

		void AddControllerPitchInput(APawn* Pawn, float Value) {
			Pawn->AddControllerPitchInput(Value);
		}

		void AddControllerRollInput(APawn* Pawn, float Value) {
			Pawn->AddControllerRollInput(Value);
		}

		void AddMovementInput(APawn* Pawn, const Vector3* WorldDirection, float ScaleValue, bool Force) {
			Pawn->AddMovementInput(*WorldDirection, ScaleValue, Force);
		}

		void GetGravityDirection(APawn* Pawn, Vector3* Value) {
			*Value = Pawn->GetGravityDirection();
		}
	}

	namespace Controller {
		bool IsLookInputIgnored(AController* Controller) {
			return Controller->IsLookInputIgnored();
		}

		bool IsMoveInputIgnored(AController* Controller) {
			return Controller->IsMoveInputIgnored();
		}

		bool IsPlayerController(AController* Controller) {
			return Controller->IsPlayerController();
		}

		APawn* GetPawn(AController* Controller) {
			return Controller->GetPawn();
		}

		bool LineOfSightTo(AController* Controller, AActor* Actor, const Vector3* ViewPoint, bool AlternateChecks) {
			return Controller->LineOfSightTo(Actor, *ViewPoint, AlternateChecks);
		}

		void SetInitialLocationAndRotation(AController* Controller, const Vector3* NewLocation, const Quaternion* NewRotation) {
			Controller->SetInitialLocationAndRotation(*NewLocation, FRotator(*NewRotation));
		}

		void SetIgnoreLookInput(AController* Controller, bool Value) {
			Controller->SetIgnoreLookInput(Value);
		}

		void SetIgnoreMoveInput(AController* Controller, bool Value) {
			Controller->SetIgnoreMoveInput(Value);
		}

		void ResetIgnoreLookInput(AController* Controller) {
			Controller->ResetIgnoreLookInput();
		}

		void ResetIgnoreMoveInput(AController* Controller) {
			Controller->ResetIgnoreMoveInput();
		}
	}

	namespace AIController {
		void ClearFocus(AAIController* AIController, AIFocusPriority Priority) {
			AIController->ClearFocus(static_cast<EAIFocusPriority::Type>(Priority));
		}

		void GetFocalPoint(AAIController* AIController, Vector3* Value) {
			*Value = AIController->GetFocalPoint();
		}

		void SetFocalPoint(AAIController* AIController, const Vector3* NewFocus, AIFocusPriority Priority) {
			AIController->SetFocalPoint(*NewFocus, static_cast<EAIFocusPriority::Type>(Priority));
		}

		AActor* GetFocusActor(AAIController* AIController) {
			return AIController->GetFocusActor();
		}

		bool GetAllowStrafe(AAIController* AIController) {
			return AIController->bAllowStrafe;
		}

		void SetAllowStrafe(AAIController* AIController, bool Value) {
			AIController->bAllowStrafe = Value;
		}

		void SetFocus(AAIController* AIController, AActor* NewFocus, AIFocusPriority Priority) {
			AIController->SetFocus(NewFocus, static_cast<EAIFocusPriority::Type>(Priority));
		}
	}

	namespace PlayerController {
		bool IsPaused(APlayerController* PlayerController) {
			return PlayerController->IsPaused();
		}

		bool GetShowMouseCursor(APlayerController* PlayerController) {
			return PlayerController->bShowMouseCursor;
		}

		bool GetMousePosition(APlayerController* PlayerController, float* X, float* Y) {
			return PlayerController->GetMousePosition(*X, *Y);
		}

		void GetPlayerViewPoint(APlayerController* PlayerController, Vector3* Location, Quaternion* Rotation) {
			FVector location;
			FRotator rotation;

			PlayerController->GetPlayerViewPoint(location, rotation);

			*Location = location;
			*Rotation = rotation.Quaternion();
		}

		UPlayerInput* GetPlayerInput(APlayerController* PlayerController) {
			return PlayerController->PlayerInput;
		}

		void SetShowMouseCursor(APlayerController* PlayerController, bool Value) {
			PlayerController->bShowMouseCursor = Value;
		}

		void SetMousePosition(APlayerController* PlayerController, float X, float Y) {
			PlayerController->SetMouseLocation(static_cast<int32>(X), static_cast<int32>(Y));
		}

		void ConsoleCommand(APlayerController* PlayerController, const char* Command, bool WriteToLog) {
			PlayerController->ConsoleCommand(FString(ANSI_TO_TCHAR(Command)), WriteToLog);
		}

		bool SetPause(APlayerController* PlayerController, bool Value) {
			return PlayerController->SetPause(Value);
		}

		void SetViewTarget(APlayerController* PlayerController, AActor* NewViewTarget) {
			PlayerController->SetViewTarget(NewViewTarget);
		}

		void SetViewTargetWithBlend(APlayerController* PlayerController, AActor* NewViewTarget, float Time, float Exponent, BlendType Type, bool LockOutgoing) {
			EViewTargetBlendFunction type = EViewTargetBlendFunction::VTBlend_Linear;

			switch (Type) {
				case BlendType::Linear:
					break;

				case BlendType::Cubic:
					type = EViewTargetBlendFunction::VTBlend_Cubic;
					break;

				case BlendType::EaseIn:
					type = EViewTargetBlendFunction::VTBlend_EaseIn;
					break;

				case BlendType::EaseOut:
					type = EViewTargetBlendFunction::VTBlend_EaseOut;
					break;

				case BlendType::EaseInOut:
					type = EViewTargetBlendFunction::VTBlend_EaseInOut;
					break;

				default:
					break;
			}

			PlayerController->SetViewTargetWithBlend(NewViewTarget, Time, type, Exponent, LockOutgoing);
		}

		void AddYawInput(APlayerController* PlayerController, float Value) {
			PlayerController->AddYawInput(Value);
		}

		void AddPitchInput(APlayerController* PlayerController, float Value) {
			PlayerController->AddPitchInput(Value);
		}

		void AddRollInput(APlayerController* PlayerController, float Value) {
			PlayerController->AddRollInput(Value);
		}
	}

	namespace Volume {
		bool EncompassesPoint(AVolume* Volume, const Vector3* Point, float SphereRadius, float* DistanceToPoint) {
			return Volume->EncompassesPoint(*Point, SphereRadius, DistanceToPoint);
		}
	}

	namespace SoundBase {
		float GetDuration(USoundBase* SoundBase) {
			return SoundBase->Duration;
		}
	}

	namespace SoundWave {
		bool GetLoop(USoundWave* SoundWave) {
			return SoundWave->bLooping;
		}

		void SetLoop(USoundWave* SoundWave, bool Value) {
			SoundWave->bLooping = Value;
		}
	}

	namespace AnimationInstance {
		UAnimMontage* GetCurrentActiveMontage(UAnimInstance* AnimationInstance) {
			return AnimationInstance->GetCurrentActiveMontage();
		}

		float MontagePlay(UAnimInstance* AnimationInstance, UAnimMontage* Montage, float PlayRate, float TimeToStartMontageAt, bool StopAllMontages) {
			return AnimationInstance->Montage_Play(Montage, PlayRate, EMontagePlayReturnType::MontageLength, TimeToStartMontageAt, StopAllMontages);
		}

		void MontagePause(UAnimInstance* AnimationInstance, UAnimMontage* Montage) {
			AnimationInstance->Montage_Pause(Montage);
		}

		void MontageResume(UAnimInstance* AnimationInstance, UAnimMontage* Montage) {
			AnimationInstance->Montage_Resume(Montage);
		}
	}

	namespace PlayerInput {
		bool IsKeyPressed(UPlayerInput* PlayerInput, const char* Key) {
			return PlayerInput->IsPressed(FKey(ANSI_TO_TCHAR(Key)));
		}

		float GetTimeKeyPressed(UPlayerInput* PlayerInput, const char* Key) {
			return PlayerInput->GetTimeDown(FKey(ANSI_TO_TCHAR(Key)));
		}

		void GetMouseSensitivity(UPlayerInput* PlayerInput, Vector2* Value) {
			Value->X = PlayerInput->GetMouseSensitivityX();
			Value->Y = PlayerInput->GetMouseSensitivityY();
		}

		void SetMouseSensitivity(UPlayerInput* PlayerInput, const Vector2* Value) {
			PlayerInput->SetMouseSensitivity(Value->X, Value->Y);
		}
	}

	namespace ActorComponent {
		bool IsOwnerSelected(UActorComponent* ActorComponent) {
			return ActorComponent->IsOwnerSelected();
		}

		AActor* GetOwner(UActorComponent* ActorComponent) {
			return ActorComponent->GetOwner();
		}

		void Destroy(UActorComponent* ActorComponent, bool PromoteChildren) {
			ActorComponent->DestroyComponent(PromoteChildren);
		}

		void AddTag(UActorComponent* ActorComponent, const char* Tag) {
			ActorComponent->ComponentTags.AddUnique(FName(ANSI_TO_TCHAR(Tag)));
		}

		void RemoveTag(UActorComponent* ActorComponent, const char* Tag) {
			ActorComponent->ComponentTags.Remove(FName(ANSI_TO_TCHAR(Tag)));
		}

		bool HasTag(UActorComponent* ActorComponent, const char* Tag) {
			return ActorComponent->ComponentHasTag(FName(ANSI_TO_TCHAR(Tag)));
		}
	}

	namespace InputComponent {
		bool HasBindings(UInputComponent* InputComponent) {
			return InputComponent->HasBindings();
		}

		int32 GetActionBindingsNumber(UInputComponent* InputComponent) {
			return InputComponent->GetNumActionBindings();
		}

		void ClearActionBindings(UInputComponent* InputComponent) {
			InputComponent->ClearActionBindings();
		}

		void BindAction(UInputComponent* InputComponent, const char* ActionName, InputEvent KeyEvent, bool ExecutedWhenPaused, InputDelegate Function) {
			FInputActionBinding actionBinding(FName(ANSI_TO_TCHAR(ActionName)), KeyEvent);

			actionBinding.bExecuteWhenPaused = ExecutedWhenPaused;
			actionBinding.ActionDelegate.GetDelegateForManualSet().BindLambda([Function]() {
				Function();
			});

			InputComponent->AddActionBinding(actionBinding);
		}

		void BindAxis(UInputComponent* InputComponent, const char* AxisName, bool ExecutedWhenPaused, InputAxisDelegate Function) {
			FInputAxisBinding axisBinding(FName(ANSI_TO_TCHAR(AxisName)));

			axisBinding.bExecuteWhenPaused = ExecutedWhenPaused;
			axisBinding.AxisDelegate.GetDelegateForManualSet().BindLambda([Function](float AxisValue) {
				Function(AxisValue);
			});

			InputComponent->AxisBindings.Emplace(axisBinding);
		}

		void RemoveActionBinding(UInputComponent* InputComponent, const char* ActionName, InputEvent KeyEvent) {
			InputComponent->RemoveActionBinding(FName(ANSI_TO_TCHAR(ActionName)), KeyEvent);
		}

		bool GetBlockInput(UInputComponent* InputComponent) {
			return InputComponent->bBlockInput;
		}

		void SetBlockInput(UInputComponent* InputComponent, bool Value) {
			InputComponent->bBlockInput = Value;
		}

		int32 GetPriority(UInputComponent* InputComponent) {
			return InputComponent->Priority;
		}

		void SetPriority(UInputComponent* InputComponent, int32 Value) {
			InputComponent->Priority = Value;
		}
	}

	namespace SceneComponent {
		bool IsAttachedToComponent(USceneComponent* SceneComponent, USceneComponent* Component) {
			return SceneComponent->IsAttachedTo(Component);
		}

		bool IsAttachedToActor(USceneComponent* SceneComponent, AActor* Actor) {
			USceneComponent* Component = SceneComponent;

			while (Component) {
				if (Component->GetOwner() == Actor)
					return true;

				Component = Component->GetAttachParent();
			}

			return false;
		}

		USceneComponent* Create(AActor* Actor, ComponentType Type, const char* Name, bool SetAsRoot, UObject* Blueprint) {
			USceneComponent* component = nullptr;

			if (!Blueprint) {
				UNREALCLR_GET_ATTACHABLE_COMPONENT_TYPE(Type, NewObject<, >(Actor), component);
			} else {
				#if !WITH_EDITOR
					UNREALCLR_GET_ATTACHABLE_COMPONENT_TYPE(Type, NewObject<, >(Actor, Cast<UClass>(Blueprint)), component);
				#else
					UNREALCLR_GET_ATTACHABLE_COMPONENT_TYPE(Type, NewObject<, >(Actor, Cast<UBlueprint>(Blueprint)->GeneratedClass), component);
				#endif
			}

			if (component) {
				USceneComponent* rootComponent = Actor->GetRootComponent();

				if (!rootComponent || SetAsRoot)
					Actor->SetRootComponent(component);
				else
					component->AttachToComponent(rootComponent, FAttachmentTransformRules::KeepRelativeTransform);

				Actor->AddInstanceComponent(component);
				component->OnComponentCreated();
				component->RegisterComponent();

				if (Name)
					component->Rename(*FString(ANSI_TO_TCHAR(Name)));
			}

			return component;
		}

		bool AttachToComponent(USceneComponent* SceneComponent, USceneComponent* Parent, AttachmentTransformRule AttachmentRule, const char* SocketName) {
			FAttachmentTransformRules attachmentRules = FAttachmentTransformRules::KeepRelativeTransform;
			FName socketName;

			UNREALCLR_GET_ATTACHMENT_RULE(AttachmentRule, attachmentRules);

			if (SocketName)
				socketName = FName(ANSI_TO_TCHAR(SocketName));

			return SceneComponent->AttachToComponent(Parent, attachmentRules, socketName);
		}

		void UpdateToWorld(USceneComponent* SceneComponent, TeleportType Type, UpdateTransformFlags Flags) {
			ETeleportType type = ETeleportType::None;

			switch (Type) {
				case ETeleportType::None:
					break;

				case ETeleportType::TeleportPhysics:
					type = ETeleportType::TeleportPhysics;
					break;

				case ETeleportType::ResetPhysics:
					type = ETeleportType::ResetPhysics;
					break;

				default:
					break;
			}

			SceneComponent->UpdateComponentToWorld(static_cast<EUpdateTransformFlags>(Flags), type);
		}

		void AddLocalOffset(USceneComponent* SceneComponent, const Vector3* DeltaLocation) {
			SceneComponent->AddLocalOffset(*DeltaLocation);
		}

		void AddLocalRotation(USceneComponent* SceneComponent, const Quaternion* DeltaRotation) {
			SceneComponent->AddLocalRotation(*DeltaRotation);
		}

		void AddRelativeLocation(USceneComponent* SceneComponent, const Vector3* DeltaLocation) {
			SceneComponent->AddRelativeLocation(*DeltaLocation);
		}

		void AddRelativeRotation(USceneComponent* SceneComponent, const Quaternion* DeltaRotation) {
			SceneComponent->AddRelativeRotation(*DeltaRotation);
		}

		void AddLocalTransform(USceneComponent* SceneComponent, const Transform* DeltaTransform) {
			SceneComponent->AddLocalTransform(*DeltaTransform);
		}

		void AddWorldOffset(USceneComponent* SceneComponent, const Vector3* DeltaLocation) {
			SceneComponent->AddWorldOffset(*DeltaLocation);
		}

		void AddWorldRotation(USceneComponent* SceneComponent, const Quaternion* DeltaRotation) {
			SceneComponent->AddWorldRotation(*DeltaRotation);
		}

		void AddWorldTransform(USceneComponent* SceneComponent, const Transform* DeltaTransform) {
			SceneComponent->AddWorldTransform(*DeltaTransform);
		}

		void GetComponentVelocity(USceneComponent* SceneComponent, Vector3* Value) {
			*Value = SceneComponent->GetComponentVelocity();
		}

		void GetComponentLocation(USceneComponent* SceneComponent, Vector3* Value) {
			*Value = SceneComponent->GetComponentLocation();
		}

		void GetComponentRotation(USceneComponent* SceneComponent, Quaternion* Value) {
			*Value = SceneComponent->GetComponentQuat();
		}

		void GetComponentScale(USceneComponent* SceneComponent, Vector3* Value) {
			*Value = SceneComponent->GetComponentScale();
		}

		void GetComponentTransform(USceneComponent* SceneComponent, Transform* Value) {
			*Value = SceneComponent->GetComponentTransform();
		}

		void GetForwardVector(USceneComponent* SceneComponent, Vector3* Value) {
			*Value = SceneComponent->GetForwardVector();
		}

		void GetRightVector(USceneComponent* SceneComponent, Vector3* Value) {
			*Value = SceneComponent->GetRightVector();
		}

		void GetUpVector(USceneComponent* SceneComponent, Vector3* Value) {
			*Value = SceneComponent->GetUpVector();
		}

		void SetMobility(USceneComponent* SceneComponent, ComponentMobility Mobility) {
			EComponentMobility::Type mobility = EComponentMobility::Type::Movable;

			switch (Mobility) {
				case ComponentMobility::Movable:
					break;

				case ComponentMobility::Static:
					mobility = EComponentMobility::Type::Static;
					break;

				case ComponentMobility::Stationary:
					mobility = EComponentMobility::Type::Stationary;
					break;

				default:
					break;
			}

			SceneComponent->SetMobility(mobility);
		}

		void SetRelativeLocation(USceneComponent* SceneComponent, const Vector3* Location) {
			SceneComponent->SetRelativeLocation(*Location);
		}

		void SetRelativeRotation(USceneComponent* SceneComponent, const Quaternion* Rotation) {
			SceneComponent->SetRelativeRotation(*Rotation);
		}

		void SetRelativeTransform(USceneComponent* SceneComponent, const Transform* Transform) {
			SceneComponent->SetRelativeTransform(*Transform);
		}

		void SetWorldLocation(USceneComponent* SceneComponent, const Vector3* Location) {
			SceneComponent->SetWorldLocation(*Location);
		}

		void SetWorldRotation(USceneComponent* SceneComponent, const Quaternion* Rotation) {
			SceneComponent->SetWorldRotation(*Rotation);
		}

		void SetWorldTransform(USceneComponent* SceneComponent, const Transform* Transform) {
			SceneComponent->SetWorldTransform(*Transform);
		}
	}

	namespace AudioComponent {
		bool GetPaused(UAudioComponent* AudioComponent) {
			return AudioComponent->bIsPaused;
		}

		void SetSound(UAudioComponent* AudioComponent, USoundBase* Sound) {
			AudioComponent->SetSound(Sound);
		}

		void SetPaused(UAudioComponent* AudioComponent, bool Value) {
			AudioComponent->SetPaused(Value);
		}

		void Play(UAudioComponent* AudioComponent) {
			AudioComponent->Play();
		}

		void Stop(UAudioComponent* AudioComponent) {
			AudioComponent->Stop();
		}
	}

	namespace CameraComponent {
		bool GetConstrainAspectRatio(UCameraComponent* CameraComponent) {
			return CameraComponent->bConstrainAspectRatio;
		}

		float GetAspectRatio(UCameraComponent* CameraComponent) {
			return CameraComponent->AspectRatio;
		}

		float GetFieldOfView(UCameraComponent* CameraComponent) {
			return CameraComponent->FieldOfView;
		}

		float GetOrthoFarClipPlane(UCameraComponent* CameraComponent) {
			return CameraComponent->OrthoFarClipPlane;
		}

		float GetOrthoNearClipPlane(UCameraComponent* CameraComponent) {
			return CameraComponent->OrthoNearClipPlane;
		}

		float GetOrthoWidth(UCameraComponent* CameraComponent) {
			return CameraComponent->OrthoWidth;
		}

		bool GetLockToHeadMountedDisplay(UCameraComponent* CameraComponent) {
			return CameraComponent->bLockToHmd;
		}

		void SetProjectionMode(UCameraComponent* CameraComponent, CameraProjectionMode Mode) {
			CameraComponent->SetProjectionMode(Mode);
		}

		void SetConstrainAspectRatio(UCameraComponent* CameraComponent, bool Value) {
			CameraComponent->bConstrainAspectRatio = Value;
		}

		void SetAspectRatio(UCameraComponent* CameraComponent, float Value) {
			CameraComponent->AspectRatio = Value;
		}

		void SetFieldOfView(UCameraComponent* CameraComponent, float Value) {
			CameraComponent->FieldOfView = Value;
		}

		void SetOrthoFarClipPlane(UCameraComponent* CameraComponent, float Value) {
			CameraComponent->OrthoFarClipPlane = Value;
		}

		void SetOrthoNearClipPlane(UCameraComponent* CameraComponent, float Value) {
			CameraComponent->OrthoNearClipPlane = Value;
		}

		void SetOrthoWidth(UCameraComponent* CameraComponent, float Value) {
			CameraComponent->OrthoWidth = Value;
		}

		void SetLockToHeadMountedDisplay(UCameraComponent* CameraComponent, bool Value) {
			CameraComponent->bLockToHmd = Value;
		}
	}

	namespace PrimitiveComponent {
		void AddAngularImpulseInDegrees(UPrimitiveComponent* PrimitiveComponent, const Vector3* Impulse, const char* BoneName, bool VelocityChange) {
			UNREALCLR_SET_BONE_NAME(BoneName);

			PrimitiveComponent->AddAngularImpulseInDegrees(*Impulse, boneName, VelocityChange);
		}

		void AddAngularImpulseInRadians(UPrimitiveComponent* PrimitiveComponent, const Vector3* Impulse, const char* BoneName, bool VelocityChange) {
			UNREALCLR_SET_BONE_NAME(BoneName);

			PrimitiveComponent->AddAngularImpulseInRadians(*Impulse, boneName, VelocityChange);
		}

		void AddForce(UPrimitiveComponent* PrimitiveComponent, const Vector3* Force, const char* BoneName, bool AccelerationChange) {
			UNREALCLR_SET_BONE_NAME(BoneName);

			PrimitiveComponent->AddForce(*Force, boneName, AccelerationChange);
		}

		void AddForceAtLocation(UPrimitiveComponent* PrimitiveComponent, const Vector3* Force, const Vector3* Location, const char* BoneName, bool LocalSpace) {
			UNREALCLR_SET_BONE_NAME(BoneName);

			if (!LocalSpace)
				PrimitiveComponent->AddForceAtLocation(*Force, *Location, boneName);
			else
				PrimitiveComponent->AddForceAtLocationLocal(*Force, *Location, boneName);
		}

		void AddImpulse(UPrimitiveComponent* PrimitiveComponent, const Vector3* Impulse, const char* BoneName, bool VelocityChange) {
			UNREALCLR_SET_BONE_NAME(BoneName);

			PrimitiveComponent->AddImpulse(*Impulse, boneName, VelocityChange);
		}

		void AddImpulseAtLocation(UPrimitiveComponent* PrimitiveComponent, const Vector3* Impulse, const Vector3* Location, const char* BoneName) {
			UNREALCLR_SET_BONE_NAME(BoneName);

			PrimitiveComponent->AddImpulseAtLocation(*Impulse, *Location, boneName);
		}

		void AddRadialForce(UPrimitiveComponent* PrimitiveComponent, const Vector3* Origin, float Radius, float Strength, bool LinearFalloff, bool AccelerationChange) {
			PrimitiveComponent->AddRadialForce(*Origin, Radius, Strength, LinearFalloff ? ERadialImpulseFalloff::RIF_Linear : ERadialImpulseFalloff::RIF_Constant, AccelerationChange);
		}

		void AddRadialImpulse(UPrimitiveComponent* PrimitiveComponent, const Vector3* Origin, float Radius, float Strength, bool LinearFalloff, bool AccelerationChange) {
			PrimitiveComponent->AddRadialImpulse(*Origin, Radius, Strength, LinearFalloff ? ERadialImpulseFalloff::RIF_Linear : ERadialImpulseFalloff::RIF_Constant, AccelerationChange);
		}

		void AddTorqueInDegrees(UPrimitiveComponent* PrimitiveComponent, const Vector3* Torque, const char* BoneName, bool AccelerationChange) {
			UNREALCLR_SET_BONE_NAME(BoneName);

			PrimitiveComponent->AddTorqueInDegrees(*Torque, boneName, AccelerationChange);
		}

		void AddTorqueInRadians(UPrimitiveComponent* PrimitiveComponent, const Vector3* Torque, const char* BoneName, bool AccelerationChange) {
			UNREALCLR_SET_BONE_NAME(BoneName);

			PrimitiveComponent->AddTorqueInRadians(*Torque, boneName, AccelerationChange);
		}

		float GetMass(UPrimitiveComponent* PrimitiveComponent) {
			return PrimitiveComponent->GetMass();
		}

		bool GetCastShadow(UPrimitiveComponent* PrimitiveComponent) {
			return PrimitiveComponent->CastShadow;
		}

		bool GetOnlyOwnerSee(UPrimitiveComponent* PrimitiveComponent) {
			return PrimitiveComponent->bOnlyOwnerSee;
		}

		bool GetOwnerNoSee(UPrimitiveComponent* PrimitiveComponent) {
			return PrimitiveComponent->bOwnerNoSee;
		}

		UMaterialInstanceDynamic* GetMaterial(UPrimitiveComponent* PrimitiveComponent, int32 ElementIndex) {
			return Cast<UMaterialInstanceDynamic>(PrimitiveComponent->GetMaterial(ElementIndex));
		}

		int32 GetMaterialsNumber(UPrimitiveComponent* PrimitiveComponent) {
			return PrimitiveComponent->GetNumMaterials();
		}

		float GetDistanceToCollision(UPrimitiveComponent* PrimitiveComponent, const Vector3* Point, Vector3* ClosestPointOnCollision) {
			FVector closestPointOnCollision;

			float result = PrimitiveComponent->GetDistanceToCollision(*Point, closestPointOnCollision);

			*ClosestPointOnCollision = closestPointOnCollision;

			return result;
		}

		bool GetSquaredDistanceToCollision(UPrimitiveComponent* PrimitiveComponent, const Vector3* Point, float* SquaredDistance, Vector3* ClosestPointOnCollision) {
			FVector closestPointOnCollision;
			float squaredDistance = 0.0f;

			bool result = PrimitiveComponent->GetSquaredDistanceToCollision(*Point, squaredDistance, closestPointOnCollision);

			*SquaredDistance = squaredDistance;
			*ClosestPointOnCollision = closestPointOnCollision;

			return result;
		}

		void SetMass(UPrimitiveComponent* PrimitiveComponent, float Mass, const char* BoneName) {
			UNREALCLR_SET_BONE_NAME(BoneName);

			PrimitiveComponent->SetMassOverrideInKg(boneName, Mass);
		}

		void SetCastShadow(UPrimitiveComponent* PrimitiveComponent, bool Value) {
			PrimitiveComponent->CastShadow = Value;
		}

		void SetOnlyOwnerSee(UPrimitiveComponent* PrimitiveComponent, bool Value) {
			PrimitiveComponent->bOnlyOwnerSee = Value;
		}

		void SetOwnerNoSee(UPrimitiveComponent* PrimitiveComponent, bool Value) {
			PrimitiveComponent->bOwnerNoSee = Value;
		}

		void SetMaterial(UPrimitiveComponent* PrimitiveComponent, int32 ElementIndex, UMaterialInterface* Material) {
			PrimitiveComponent->SetMaterial(ElementIndex, Material);
		}

		void SetSimulatePhysics(UPrimitiveComponent* PrimitiveComponent, bool Value) {
			PrimitiveComponent->SetSimulatePhysics(Value);
		}

		UMaterialInstanceDynamic* CreateAndSetMaterialInstanceDynamic(UPrimitiveComponent* PrimitiveComponent, int32 ElementIndex) {
			return PrimitiveComponent->CreateAndSetMaterialInstanceDynamic(ElementIndex);
		}
	}

	namespace ShapeComponent {
		bool GetDynamicObstacle(UShapeComponent* ShapeComponent) {
			return ShapeComponent->bDynamicObstacle;
		}

		int32 GetShapeColor(UShapeComponent* ShapeComponent) {
			return UNREALCLR_COLOR_TO_INTEGER(ShapeComponent->ShapeColor);
		}

		void SetDynamicObstacle(UShapeComponent* ShapeComponent, bool Value) {
			ShapeComponent->bDynamicObstacle = Value;
		}

		void SetShapeColor(UShapeComponent* ShapeComponent, Color Value) {
			ShapeComponent->ShapeColor = Value;
		}
	}

	namespace BoxComponent {
		void GetScaledBoxExtent(UBoxComponent* BoxComponent, Vector3* Value) {
			*Value = BoxComponent->GetScaledBoxExtent();
		}

		void GetUnscaledBoxExtent(UBoxComponent* BoxComponent, Vector3* Value) {
			*Value = BoxComponent->GetUnscaledBoxExtent();
		}

		void SetBoxExtent(UBoxComponent* BoxComponent, const Vector3* Extent, bool UpdateOverlaps) {
			BoxComponent->SetBoxExtent(*Extent, UpdateOverlaps);
		}

		void InitBoxExtent(UBoxComponent* BoxComponent, const Vector3* Extent) {
			BoxComponent->InitBoxExtent(*Extent);
		}
	}

	namespace SphereComponent {
		float GetScaledSphereRadius(USphereComponent* SphereComponent) {
			return SphereComponent->GetScaledSphereRadius();
		}

		float GetUnscaledSphereRadius(USphereComponent* SphereComponent) {
			return SphereComponent->GetUnscaledSphereRadius();
		}

		float GetShapeScale(USphereComponent* SphereComponent) {
			return SphereComponent->GetShapeScale();
		}

		void SetSphereRadius(USphereComponent* SphereComponent, float SphereRadius, bool UpdateOverlaps) {
			SphereComponent->SetSphereRadius(SphereRadius, UpdateOverlaps);
		}

		void InitSphereRadius(USphereComponent* SphereComponent, float SphereRadius) {
			SphereComponent->InitSphereRadius(SphereRadius);
		}
	}

	namespace CapsuleComponent {
		float GetScaledCapsuleRadius(UCapsuleComponent* CapsuleComponent) {
			return CapsuleComponent->GetScaledCapsuleRadius();
		}

		float GetUnscaledCapsuleRadius(UCapsuleComponent* CapsuleComponent) {
			return CapsuleComponent->GetUnscaledCapsuleRadius();
		}

		float GetShapeScale(UCapsuleComponent* CapsuleComponent) {
			return CapsuleComponent->GetShapeScale();
		}

		void GetScaledCapsuleSize(UCapsuleComponent* CapsuleComponent, float* Radius, float* HalfHeight) {
			CapsuleComponent->GetScaledCapsuleSize(*Radius, *HalfHeight);
		}

		void GetUnscaledCapsuleSize(UCapsuleComponent* CapsuleComponent, float* Radius, float* HalfHeight) {
			CapsuleComponent->GetUnscaledCapsuleSize(*Radius, *HalfHeight);
		}

		void SetCapsuleRadius(UCapsuleComponent* CapsuleComponent, float Radius, bool UpdateOverlaps) {
			CapsuleComponent->SetCapsuleRadius(Radius, UpdateOverlaps);
		}

		void SetCapsuleSize(UCapsuleComponent* CapsuleComponent, float Radius, float HalfHeight, bool UpdateOverlaps) {
			CapsuleComponent->SetCapsuleSize(Radius, HalfHeight, UpdateOverlaps);
		}

		void InitCapsuleSize(UCapsuleComponent* CapsuleComponent, float Radius, float HalfHeight) {
			CapsuleComponent->InitCapsuleSize(Radius, HalfHeight);
		}
	}

	namespace MeshComponent {
		int32 GetMaterialIndex(UMeshComponent* MeshComponent, const char* MaterialSlotName) {
			return MeshComponent->GetMaterialIndex(FName(ANSI_TO_TCHAR(MaterialSlotName)));
		}
	}

	namespace MotionControllerComponent {
		bool IsTracked(UMotionControllerComponent* MotionControllerComponent) {
			return MotionControllerComponent->IsTracked();
		}
	}

	namespace StaticMeshComponent {
		void GetLocalBounds(UStaticMeshComponent* StaticMeshComponent, Vector3* Min, Vector3* Max) {
			FVector min, max;

			StaticMeshComponent->GetLocalBounds(min, max);

			*Min = min;
			*Max = max;
		}

		UStaticMesh* GetStaticMesh(UStaticMeshComponent* StaticMeshComponent) {
			return StaticMeshComponent->GetStaticMesh();
		}

		bool SetStaticMesh(UStaticMeshComponent* StaticMeshComponent, UStaticMesh* StaticMesh) {
			return StaticMeshComponent->SetStaticMesh(StaticMesh);
		}
	}

	namespace InstancedStaticMeshComponent {
		int32 GetInstanceCount(UInstancedStaticMeshComponent* InstancedStaticMeshComponent) {
			return InstancedStaticMeshComponent->GetInstanceCount();
		}

		int32 AddInstance(UInstancedStaticMeshComponent* InstancedStaticMeshComponent, const Transform* InstanceTransform) {
			return InstancedStaticMeshComponent->AddInstance(*InstanceTransform);
		}

		bool UpdateInstanceTransform(UInstancedStaticMeshComponent* InstancedStaticMeshComponent, int32 InstanceIndex, const Transform* InstanceTransform, bool WorldSpace, bool MarkRenderStateDirty, bool Teleport) {
			return InstancedStaticMeshComponent->UpdateInstanceTransform(InstanceIndex, *InstanceTransform, WorldSpace, MarkRenderStateDirty, Teleport);
		}

		void ClearInstances(UInstancedStaticMeshComponent* InstancedStaticMeshComponent) {
			InstancedStaticMeshComponent->ClearInstances();
		}
	}

	namespace SkinnedMeshComponent {
		void SetSkeletalMesh(USkinnedMeshComponent* SkinnedMeshComponent, USkeletalMesh* SkeletalMesh, bool ReinitializePose) {
			SkinnedMeshComponent->SetSkeletalMesh(SkeletalMesh, ReinitializePose);
		}
	}

	namespace SkeletalMeshComponent {
		UAnimInstance* GetAnimationInstance(USkeletalMeshComponent* SkeletalMeshComponent) {
			return SkeletalMeshComponent->GetAnimInstance();
		}

		void SetAnimation(USkeletalMeshComponent* SkeletalMeshComponent, UAnimationAsset* Asset) {
			SkeletalMeshComponent->SetAnimation(Asset);
		}

		void SetAnimationMode(USkeletalMeshComponent* SkeletalMeshComponent, AnimationMode Mode) {
			SkeletalMeshComponent->SetAnimationMode(Mode);
		}

		void SetAnimationBlueprint(USkeletalMeshComponent* SkeletalMeshComponent, UObject* Blueprint) {
			#if !WITH_EDITOR
				SkeletalMeshComponent->SetAnimInstanceClass(Cast<UClass>(Blueprint));
			#else
				SkeletalMeshComponent->SetAnimInstanceClass(Cast<UBlueprint>(Blueprint)->GeneratedClass);
			#endif
		}

		void Play(USkeletalMeshComponent* SkeletalMeshComponent, bool Loop) {
			SkeletalMeshComponent->Play(Loop);
		}

		void PlayAnimation(USkeletalMeshComponent* SkeletalMeshComponent, UAnimationAsset* Asset, bool Loop) {
			SkeletalMeshComponent->PlayAnimation(Asset, Loop);
		}

		void Stop(USkeletalMeshComponent* SkeletalMeshComponent) {
			SkeletalMeshComponent->Stop();
		}
	}

	namespace MaterialInterface {
		bool IsTwoSided(UMaterialInterface* MaterialInterface) {
			return MaterialInterface->IsTwoSided();
		}
	}

	namespace Material {
		bool IsDefaultMaterial(UMaterial* Material) {
			return Material->IsDefaultMaterial();
		}
	}

	namespace MaterialInstance {
		bool IsChildOf(UMaterialInstance* MaterialInstance, UMaterialInterface* Material) {
			return MaterialInstance->IsChildOf(Material);
		}
	}

	namespace MaterialInstanceDynamic {
		void ClearParameterValues(UMaterialInstanceDynamic* MaterialInstanceDynamic) {
			MaterialInstanceDynamic->ClearParameterValues();
		}

		void SetTextureParameterValue(UMaterialInstanceDynamic* MaterialInstanceDynamic, const char* ParameterName, UTexture* Value) {
			MaterialInstanceDynamic->SetTextureParameterValue(FName(ANSI_TO_TCHAR(ParameterName)), Value);
		}

		void SetVectorParameterValue(UMaterialInstanceDynamic* MaterialInstanceDynamic, const char* ParameterName, const LinearColor* Value) {
			MaterialInstanceDynamic->SetVectorParameterValue(FName(ANSI_TO_TCHAR(ParameterName)), *Value);
		}

		void SetScalarParameterValue(UMaterialInstanceDynamic* MaterialInstanceDynamic, const char* ParameterName, float Value) {
			MaterialInstanceDynamic->SetScalarParameterValue(FName(ANSI_TO_TCHAR(ParameterName)), Value);
		}
	}
}