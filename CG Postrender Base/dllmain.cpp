/*
    Thanks for using this! As I am new to all of this I am hoping this will also help some people that had the same struggles I did!
    Some common issue's I ran into while learning that may help others are
     * Never use return in a loop, use continue (took me 3 days to figure out why my ESP wasnt working)
     * Most errors can be fixed with a simple 2-3 minute google search
     * Make sure you have it set to release, not debug lol.
*/

#include "pch.h"
#include "libs/SDK.h"
#include "libs/SDK/BasicTypes_Package.cpp"
#include "libs/SDK/CoreUObject_Package.cpp"
#include "libs/SDK/Engine_Package.cpp"
#include "libs/ZeroGUI/ZeroGUI.h"
#include "libs/ZeroGUI/ZeroInput.h"

#define KeyPressed( k ) ( GetAsyncKeyState(k) & 0x8000 )
#define Nullcheck(x) if (x == NULL || x == nullptr || !x) {return;}
<<<<<<< HEAD
#define NullcheckC(x) if (x == NULL || x == nullptr || !x) {continue;}s

using namespace CG;
int MenuKey = 45; // V Key index. Checkout https://boostrobotics.eu/windows-key-codes/ for all the key indexes!
int PostRenderIndex = 0;
FVector2D pos = FVector2D{ 100.0f, 100.0f };
FVector2D WindowSize = FVector2D{ 500.f, 300.f };

inline bool TestBool = false;
inline float TestFloat = 0.f;

void Cheats() {
     ZeroGUI::Input::Handle();

    static bool menu_open = false;
    if (GetAsyncKeyState(MenuKey) & 1) menu_open = !menu_open;

    if (ZeroGUI::Window((char*)"Title", &pos, WindowSize, menu_open)) {
        static int tab = 0;
        if (ZeroGUI::ButtonTab((char*)"NULL", FVector2D{ 100, 15 }, tab == 0)) tab = 0;
        ZeroGUI::SameLine();
        if (ZeroGUI::ButtonTab((char*)"NULL", FVector2D{ 100, 15 }, tab == 1)) tab = 1;
        ZeroGUI::SameLine();
        if (ZeroGUI::ButtonTab((char*)"NULL", FVector2D{ 100, 15 }, tab == 2)) tab = 2;
        ZeroGUI::SameLine();
        if (ZeroGUI::ButtonTab((char*)"NULL", FVector2D{ 100, 15 }, tab == 3)) tab = 3;

        if (tab == 0) {
            ZeroGUI::Text((char*)"NULL");
        }

        if (tab == 1) {
            ZeroGUI::Checkbox((char*)"NULL", &TestBool);
        }

        if (tab == 2) {
            ZeroGUI::SliderFloat((char*)"NULL", &TestFloat, 0.f, 1000.f, (char*)"%1.0f");
        }

        if (tab == 3) {
            if (ZeroGUI::Button((char*)"NULL", FVector2D{ 100, 25 })) {}
        }

        UWorld** _UWorld = reinterpret_cast<UWorld**>(UWorld::GWorld);
        Nullcheck(_UWorld);
        Nullcheck(*_UWorld);
        UGameInstance* OwningGameInstance = (*_UWorld)->OwningGameInstance;
        Nullcheck(OwningGameInstance);
        auto GameState = (*_UWorld)->GameState;
        Nullcheck(GameState);
        auto PersistentLevel = (*_UWorld)->PersistentLevel;
        Nullcheck(PersistentLevel);
        UWorld* _GWorld = UWorld::GWorld[0];
        Nullcheck(_GWorld);
        ULocalPlayer* LocalPlayer = _GWorld->OwningGameInstance->LocalPlayers[0];
        Nullcheck(LocalPlayer);
        auto PlayerController = LocalPlayer->PlayerController;
        Nullcheck(PlayerController);
    }
=======
#define NullcheckC(x) if (x == NULL || x == nullptr || !x) {continue;}

using namespace CG;
int MenuKey = 45;
int PostRenderIndex = 98;

void Cheats() {
  // Cheats Here (Drawing related stuff goes in 'void Hook')   
>>>>>>> 3389fcf688d2d149543bfdf5330877cc25a9177d
}

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

