#include <iostream>
#include <cmath>
#include <algorithm>
#include "collision.h"
#include <cstdio> //printf

bool CollisionCircleAABB(float cx, float cy, float cr, float min_x, float min_y, float max_x, float max_y) {
    // Find the closest point on the AABB to the center of the circle
    float closest_x = std::clamp(cx, min_x, max_x);
    float closest_y = std::clamp(cy, min_y, max_y);
    // Check if the closest point is inside the rectangle
    if (closest_x < min_x || closest_x > max_x || closest_y < min_y || closest_y > max_y) {
        return true;
    }

    // Check if the distance between the closest point and the center of the circle is less than the radius
    float distance_x = cx - closest_x;
    float distance_y = cy - closest_y;
    float distance_squared = distance_x * distance_x + distance_y * distance_y;
    return distance_squared < cr* cr;
}