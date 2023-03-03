// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "libs/SDK.h"
#include "libs/SDK/BasicTypes_Package.cpp"
#include "libs/SDK/CoreUObject_Package.cpp"
#include "libs/SDK/Engine_Package.cpp"
#include "libs/ZeroGUI/ZeroGUI.h";
#include "libs/ZeroGUI/ZeroInput.h"

using namespace CG;

inline int PostRenderIndex = 0;
typedef void(__thiscall* post_render_type)(UGameViewportClient*, UCanvas*);
post_render_type original_post_render = nullptr;
void Hook(UGameViewportClient* vp_client, UCanvas* canvas)
{
    try
    {
        //canvas->K2_DrawText(get_font(), FString(L"[Name]\nDefault Menu Key: INSERT"), FVector2D(10, 15), FVector2D(0.5f, 0.5f), rgb2rgbfl(252, 232, 3), 1.0f, FLinearColor(0, 0, 0, 1), FVector2D(), false, true, true, FLinearColor(0, 0, 0, 1));
        ZeroGUI::SetupCanvas(canvas);
        Cheats();
        UWorld** _UWorld = reinterpret_cast<UWorld**>(UWorld::GWorld);
        Nullcheck(_UWorld);
        Nullcheck(*_UWorld);
        UGameInstance* OwningGameInstance = (*_UWorld)->OwningGameInstance;
        Nullcheck(OwningGameInstance);
        auto GameState = (*_UWorld)->GameState;
        Nullcheck(GameState);
        UWorld* _GWorld = UWorld::GWorld[0];
        Nullcheck(_GWorld);
        ULocalPlayer* LocalPlayer = _GWorld->OwningGameInstance->LocalPlayers[0];
        Nullcheck(LocalPlayer);
    }
    catch (std::exception& e)
    {
        return original_post_render(vp_client, canvas);
    }
    return original_post_render(vp_client, canvas);
}

void Initialize() {
    InitSdk();
    UWorld* _UWorld = *UWorld::GWorld;
    Nullcheck(_UWorld);

    auto OwningGameInstance = _UWorld->OwningGameInstance;
    Nullcheck(OwningGameInstance);

    ULocalPlayer* LocalPlayer = OwningGameInstance->LocalPlayers[0];
    Nullcheck(LocalPlayer);

    auto GameViewportClient = LocalPlayer->ViewportClient;
    void** GameViewportClient_VTable = *reinterpret_cast<void***>(GameViewportClient);

    DWORD OldProtection;

    VirtualProtect(&GameViewportClient_VTable[PostRenderIndex], 8, PAGE_EXECUTE_READWRITE, &OldProtection);
    original_post_render = decltype(original_post_render)(GameViewportClient_VTable[PostRenderIndex]);
    GameViewportClient_VTable[PostRenderIndex] = Hook;
    VirtualProtect(&GameViewportClient_VTable[PostRenderIndex], 8, OldProtection, &OldProtection);

    return;
}

BOOL APIENTRY DllMain( HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        Initialize();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

