#include <iostream>
#include <cmath>
#include <algorithm>
#include "collision.h"
#include <cstdio> //printf

bool CollisionCircleAABB(float cx, float cy, float cr, float min_x, float min_y, float max_x, float max_y, bool *hasCollision, int *floor) {
    if (cx > min_x && cx < max_x && cy > min_y && cy < max_y) {
        *hasCollision = true;
    }
    float closest_x = std::clamp(cx, min_x, max_x);
    float closest_y = std::clamp(cy, min_y, max_y);
    float dx = closest_x - cx;
    float dy = closest_y - cy;
    float distenceCheck = dx * dx + dy * dy;
    if (distenceCheck < cr * cr) {*hasCollision = true;}
    else {*hasCollision = false;}
    // Add wall collision detection
    // 1 = O| -- 2 = |O -- 3 = ō -- 4 = ⍜
    if (*hasCollision) {
        float closest_x = std::clamp(cx, min_x, max_x);
        float closest_y = std::clamp(cy, min_y, max_y);

        if (closest_x == min_x) {
            *floor = 2; // left
        }
        else if (closest_x == max_x) {
            *floor = 1; // right
        }

        if (closest_y == min_y) {
            *floor = 3; // top
        }
        else if (closest_y == max_y) {
            *floor = 4; // bottom
        }
    //if (cy >= min_y && cy <= max_y) { *floor =  1; }
    //else { *floor =  2; }
    return 0;  
    }
}

bool CheckSide(float cx, float cy, float cr, float min_x, float min_y, float max_x, float max_y) {
    return true;
}