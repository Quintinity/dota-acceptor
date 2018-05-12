#include "screenpoints.h"
#include <unordered_map>

static std::unordered_map<UINT64, std::vector<ScreenPoint *>> points_map;
static const std::vector<ScreenPoint *> NO_POINTS;

ScreenPoint::ScreenPoint(int x, int y, COLORREF color) {
    this->x = x;
    this->y = y;
    this->color = color;
}

UINT64 make_key(UINT32 w, UINT32 h) {
    return (static_cast<UINT64>(w) << 32) | h;
}

std::vector<ScreenPoint *> get_points_for_resolution(UINT32 width, UINT32 height) {
    auto search = points_map.find(make_key(width, height));
    if  (search != points_map.end()) {
        return search->second;
    }
    return NO_POINTS;
}

int ScreenPoint::getX() {
    return x;
}

int ScreenPoint::getY() {
    return y;
}

COLORREF ScreenPoint::getColor() {
    return color;
}

void init_screen_points() {
    // 1920x1080
    std::vector<ScreenPoint *> _1920x1080;
    _1920x1080.push_back(new ScreenPoint(800, 495, RGB(46, 96, 68)));  //top-left
    _1920x1080.push_back(new ScreenPoint(1119, 495, RGB(25, 36, 35)));  //top-right
    _1920x1080.push_back(new ScreenPoint(800, 558, RGB(25, 36, 35)));  //bottom-left
    _1920x1080.push_back(new ScreenPoint(1119, 558, RGB(22, 33, 33)));  //bottom-right
    points_map[make_key(1920, 1080)] = _1920x1080;
}