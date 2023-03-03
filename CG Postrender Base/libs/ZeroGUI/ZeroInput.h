#pragma once
#include <Windows.h>

inline namespace ZeroGUI
{
    namespace Input
    {
        inline bool mouseDown[5];
        inline bool mouseDownAlready[256];

        inline bool keysDown[256];
        inline bool keysDownAlready[256];

        inline bool IsAnyMouseDown()
        {
            if (mouseDown[0]) return true;
            if (mouseDown[1]) return true;
            if (mouseDown[2]) return true;
            if (mouseDown[3]) return true;
            if (mouseDown[4]) return true;

            return false;
        }

        inline bool IsMouseClicked(int button, int element_id, bool repeat)
        {
            if (mouseDown[button])
            {
                if (!mouseDownAlready[element_id])
                {
                    mouseDownAlready[element_id] = true;
                    return true;
                }
                if (repeat)
                    return true;
            }
            else
            {
                mouseDownAlready[element_id] = false;
            }
            return false;
        }
        inline bool IsKeyPressed(int key, bool repeat)
        {
            if (keysDown[key])
            {
                if (!keysDownAlready[key])
                {
                    keysDownAlready[key] = true;
                    return true;
                }
                if (repeat)
                    return true;
            }
            else
            {
                keysDownAlready[key] = false;
            }
            return false;
        }

        inline void Handle()
        {
            if (GetAsyncKeyState(0x01))
                mouseDown[0] = true;
            else
                mouseDown[0] = false;
        }
    }
}