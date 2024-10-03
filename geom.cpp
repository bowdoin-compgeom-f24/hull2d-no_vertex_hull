#include "geom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>      // For atan2 and M_PI
#include <vector>
#include <algorithm>
#include <limits>     // For std::numeric_limits

using namespace std; 

/* **************************************** */
/* returns the signed area of triangle abc. The area is positive if c
   is to the left of ab, and negative if c is to the right of ab
 */
int signed_area2D(point2d a, point2d b, point2d c) {
  int ax = b.x - a.x;
  int ay = b.y - a.y;
  int bx = c.x - a.x;
  int by = c.y - a.y;

  return (ax * by) - (ay * bx); 
}



/* **************************************** */
/* return 1 if p,q,r collinear, and 0 otherwise */
int collinear(point2d p, point2d q, point2d r) {
  return signed_area2D(p, q, r) == 0; 
}

/* **************************************** */
/* return 1 if c is  strictly left of ab; 0 otherwise */
int left_strictly(point2d a, point2d b, point2d c) {
  return signed_area2D(a, b, c) > 0; 
}


/* return 1 if c is left of ab or on ab; 0 otherwise */
int left_on(point2d a, point2d b, point2d c) {
  return signed_area2D(a, b, c) >= 0; 
}

using namespace std;

/* Compute the radial angle between point p and reference point c */
double compute_radial_angle(const point2d& p, const point2d& c) {
    double delta_y = p.y - c.y;
    double delta_x = p.x - c.x;

    if (delta_x == 0.0 && delta_y == 0.0) {
        return std::numeric_limits<double>::quiet_NaN(); // Undefined angle
    }

    double angle = atan2(delta_y, delta_x);
    return angle;
}

//sorts points in radial order
vector<point2d> radial_sort(const vector<point2d>& points, const point2d& c) {
    vector<point2d> sorted_points = points;

    //comparator for comparing points such that they are in ASCENDING order
    auto angle_cmp = [c](const point2d& p1, const point2d& p2) -> bool {
        double angle_1 = compute_radial_angle(p1, c);
        double angle_2 = compute_radial_angle(p2, c);

        //we will return nan if and only if the point is equal to c
        bool angle_1_is_nan = std::isnan(angle_1);
        bool angle_2_is_nan = std::isnan(angle_2);

        //handle the situation such that if the vector is equal to c then we want it in a earlier position in the array
        if (angle_1_is_nan && angle_2_is_nan) {
            return false;
        }
        if (angle_1_is_nan) {
            return true;
        }
        if (angle_2_is_nan) {
            return false;
        }

        if (angle_1 != angle_2) {
            return angle_1 < angle_2;
        } else {
            // extra feature (?) we want to reach the closest point first
            // thus if the angles are equal, we will tiebreak based on distance from c
            double dx1 = p1.x - c.x;
            double dy1 = p1.y - c.y;
            double dist_sq1 = dx1 * dx1 + dy1 * dy1;

            double dx2 = p2.x - c.x;
            double dy2 = p2.y - c.y;
            double dist_sq2 = dx2 * dx2 + dy2 * dy2;

            return dist_sq1 < dist_sq2;
        }
    };

    std::sort(sorted_points.begin(), sorted_points.end(), angle_cmp);

    return sorted_points;
}

// compute the convex hull of pts, and store the points on the hull in hull
void graham_scan(vector<point2d>& pts, vector<point2d>& hull) {
    printf("hull2d (graham scan): start\n");
    hull.clear(); 

    //make sure points are not empty
    assert(!pts.empty());

    //assume the bottom_right is the first point
    point2d bottom_right = pts[0];
    //basically just a loop to find the bottom_right most point.
    for (size_t i = 1; i < pts.size(); i++) {
        if (pts[i].y < bottom_right.y) {
            bottom_right = pts[i];
        } else if (pts[i].y == bottom_right.y && pts[i].x > bottom_right.x) {
            bottom_right = pts[i];
        }
    }

    //sort the points using our found bottom_right
    vector<point2d> sorted_points = radial_sort(pts, bottom_right);

    //iterate through points
    for (size_t i = 0; i < sorted_points.size(); i++) {
        //by definition, the first two points are extreme and thus on the convex hull
        if (i == 0 || i == 1) {
            hull.push_back(sorted_points[i]);
        } else {

            point2d next_vertex = sorted_points[i];
            // Pop elements while the turn is not left
            // while loop condition also automatically gets the last two elements on hull
            while (hull.size() >= 2 && !left_strictly(hull[hull.size() - 2], hull.back(), next_vertex)) {
                hull.pop_back();
            }
            // when done popping we push the next vertex
            hull.push_back(next_vertex);
        }
    }
    printf("hull2d (graham scan): end\n");
    return;
}
