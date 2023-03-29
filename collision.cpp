#include <iostream>
#include <cmath>
#include <algorithm>
#include "collision.h"
#include <cstdio> //printf

bool CollisionCircleAABB(float cx, float cy, float cr, float min_x, float min_y, float max_x, float max_y, bool *hasCollision, int *floor, float speedX, float speedY) {
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
    // 1 = |O max_x -- 2 = O| min_x -- 3 = ō max_y -- 4 = ⍜ min_y
    if (*hasCollision) {
        //int xCheck, yCheck, x1, x2, y1, y2;
        //bool lineCollision;
        //if (speedX > 0) { xCheck = max_x; }
        //else if (speedX < 0) { xCheck = min_x; }
        //if (speedY > 0) { yCheck = min_y; }
        //else if (speedY < 0) { yCheck = max_y; }
        //if (abs(speedX) > abs(speedY)) {
        //    if (cr * 4 >= abs(cx - xCheck)) {
        //        if (xCheck = max_x) {
        //            *floor = 1;
        //        }
        //        else if (xCheck = min_x) {
        //            *floor = 2;
        //        }
        //    }
        //    else if (cr * 4 >= abs(cy - yCheck)) {
        //        if (yCheck = min_y) {
        //            *floor = 4;
        //        }
        //        else if (yCheck = max_y) {
        //            *floor = 3;
        //        }
        //    }
        //}
        //else if (abs(speedY) > abs(speedX)) {
        //    if (cr * 4 >= abs(cy - yCheck)) {
        //        if (yCheck = min_y) {
        //            *floor = 4;
        //        }
        //        else if (yCheck = max_y) {
        //            *floor = 3;
        //        }
        //    }
        //    else if (cr * 4 >= abs(cx - xCheck)) {
        //        if (xCheck = max_x) {
        //            *floor = 1;
        //        }
        //        else if (xCheck = min_x) {
        //            *floor = 2;
        //        }
        //    }
        //}
        float closest_x = std::clamp(cx, min_x, max_x);
        float closest_y = std::clamp(cy, min_y, max_y);
        // 1 = |O max_x -- 2 = O| min_x -- 3 = ō max_y -- 4 = ⍜ min_y

        if (closest_x == min_x) {
            *floor = 2; 
        }
        else if (closest_x == max_x) {
            *floor = 1; 
        }
        
        else if (closest_y == min_y) {
            *floor = 4; 
        }
        else if (closest_y == max_y) {
            *floor = 3; 
        }
    //if (cy >= min_y && cy <= max_y) { *floor =  1; }
    //else { *floor =  2; }
    return 0;  
    }
}

bool circleLineCollision(int cx, int cy, int cr, int x1, int y1, int x2, int y2) {
    // Calculate the distance between the circle center and the line
    int dx = x2 - x1;
    int dy = y2 - y1;
    int d = abs(dx * (cy - y1) - dy * (cx - x1)) / sqrt(dx * dx + dy * dy);

    // Check if the distance is less than or equal to the radius of the circle
    if (d <= cr) {
        // Check if the point of intersection is within the line segment
        int t = ((cx - x1) * dx + (cy - y1) * dy) / (dx * dx + dy * dy);
        if (t >= 0 && t <= 1) {
            return true; // Collision
        }
    }

    return false; // No collision
}