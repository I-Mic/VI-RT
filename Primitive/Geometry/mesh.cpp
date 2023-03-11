//
//  mesh.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 05/02/2023.
//

#include "mesh.hpp"
#define MAXFLOAT 3.402823E+38

Mesh::Mesh(std::vector<Face> faces, std::vector<Point> vertices, std::vector<Vector> normals): faces(std::move(faces)),vertices(std::move(vertices)),normals(std::move(normals)){}

// see pbrt book (3rd ed.), sec 3.6.2, pag 157
bool Mesh::TriangleIntersect (Ray r, Face f, Intersection *isect) {


    return false;
}

bool Mesh::intersect (Ray r, Intersection *isect) {
    bool intersect = true, intersect_this_face;
    Intersection min_isect, curr_isect;
    float min_depth=MAXFLOAT;
    // intersect the ray with the mesh BB
    
    if (!intersect) return false;
    
    // If it intersects then loop through the faces
    intersect = false;
    for (auto face_it=faces.begin() ; face_it != faces.end() ; face_it++) {
        intersect_this_face = TriangleIntersect(r, *face_it, &curr_isect);
        if (!intersect_this_face) continue;
        
        intersect = true;
        if (curr_isect.depth < min_depth) {  // this is closer
            min_depth = curr_isect.depth;
            min_isect = curr_isect;
        }
    }
    
    return intersect;
}
