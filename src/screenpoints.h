#ifndef SCREEN_POINTS_H
#define SCREEN_POINTS_H

#include <vector>
#include <windows.h>

class ScreenPoint {
    private:
    int x;
    int y;
    COLORREF color;

    public:
    ScreenPoint(int x, int y, COLORREF color);
    int getX();
    int getY();
    COLORREF getColor();
};

std::vector<ScreenPoint *> get_points_for_resolution(UINT32 width, UINT32 height);
void init_screen_points();

#endif