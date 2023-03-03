#include "Functions.h"
#include "../pch.h"
#include "libs/SDK.h"
#include "libs/SDK/BasicTypes_Package.cpp"
#include "libs/SDK/CoreUObject_Package.cpp"
#include "libs/SDK/Engine_Package.cpp"


using namespace CG;

auto GetBoneMatrixOffset = 0x0000000;

inline void getBoneLocationByIndex() {
    typedef FMatrix* (__thiscall* _GetBoneMatrix)(USkeletalMeshComponent* mesh, FMatrix* result, int index);
    FVector get_bone_location_by_index(USkeletalMeshComponent * mesh, int index)
    {
        if (!mesh)
            return { 0.f, 0.f, 0.f };

        FMatrix matrix{};

        _GetBoneMatrix fGetBoneMatrix = reinterpret_cast<_GetBoneMatrix>((uint64_t)GetModuleHandleA(NULL) + GetBoneMatrixOffset);
        fGetBoneMatrix(mesh, &matrix, index);

        return matrix.WPlane;
    }
}