#include <iostream>
#include <cmath>
#include <algorithm>
#include "collision.h"
#include <cstdio> //printf

bool CollisionCircleAABB(float cx, float cy, float cr, float min_x, float min_y, float max_x, float max_y) {
    // Find the closest point on the AABB to the center of the circle
    
    // Check if the closest point is inside the rectangle
    if (cx > min_x && cx < max_x && cy > min_y && cy < max_y) {
        return true;
    }

    float closest_x = std::clamp(cx, min_x, max_x);
    float closest_y = std::clamp(cy, min_y, max_y);
    printf("%i\n", closest_x);
    float dx = closest_x - cx;
    float dy = closest_y - cy;
    float distenceCheck = std::sqrt(dx * dx + dy * dy);
    return distenceCheck < cr;
}