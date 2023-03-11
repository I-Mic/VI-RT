//
//  Scene.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include "scene.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "primitive.hpp"
#include "mesh.hpp"
#include "Phong.hpp"

#include <iostream>
#include <set>
#include <vector>

using namespace tinyobj;

static void PrintInfo (const ObjReader myObj) {
    const tinyobj::attrib_t attrib = myObj.GetAttrib();
    const std::vector<tinyobj::shape_t> shapes = myObj.GetShapes();
    const std::vector<tinyobj::material_t> materials = myObj.GetMaterials();
    std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
    std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
    std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
              << std::endl;

    std::cout << "# of shapes    : " << shapes.size() << std::endl;
    std::cout << "# of materials : " << materials.size() << std::endl;
    
    // Iterate shapes
    auto it_shape = shapes.begin();
    for ( ; it_shape != shapes.end() ; it_shape++) {
        // assume each face has 3 vertices
        std::cout << "Processing shape " << it_shape->name << std::endl;
        // iterate faces
        // assume each face has 3 vertices
        auto it_vertex = it_shape->mesh.indices.begin();
        for ( ; it_vertex != it_shape->mesh.indices.end() ; ) {
            // process 3 vertices
            for (int v=0 ; v<3 ; v++) {
                std::cout << it_vertex->vertex_index;
                it_vertex++;
            }
            std::cout << std::endl;
        }
        
        printf("There are %lu material indexes\n", it_shape->mesh.material_ids.size());
        std::cout << std::endl;

    }
    
}

void PrintPoint(Point p) {
    printf("%f,%f,%f", p.X, p.Y, p.Z);
}

/*
 Use tiny load to load .obj scene descriptions
 https://github.com/tinyobjloader/tinyobjloader
 */

bool Scene::Load (const std::string &fname) {
    ObjReader myObjReader;

    if (!myObjReader.ParseFromFile(fname)) {
        return false;
    }
    
    PrintInfo(myObjReader);

    // convert loader's representation to my representation
    const tinyobj::attrib_t attrib = myObjReader.GetAttrib();

    const std::vector<material_t> materials = myObjReader.GetMaterials();
    std::vector<float> vertices = attrib.vertices;
    std::vector<float> normals = attrib.normals;
    const std::vector<shape_t> shps = myObjReader.GetShapes();
    //std::vector<Primitive> primitives;

    // iterate over shapes
    for (auto shp = shps.begin(); shp != shps.end(); shp++) {
        // iterate over this shape’s vertices
        std::vector<Face> faces;
        std::vector<Point> mvertices;
        std::vector<Vector> mnormals;
        for (auto indices = shp->mesh.indices.begin(); indices != shp->mesh.indices.end(); ) {
            // process 3 vertices
            Face face;
            Point pontos[3];
            Vector myNormal[3];
            face.hasShadingNormals = false;
            for (int v = 0; v < 3; v++) {
                face.vert_ndx[v] = indices->vertex_index;
                pontos[v].X = vertices[indices->vertex_index * 3];
                pontos[v].Y = vertices[indices->vertex_index * 3 + 1];
                pontos[v].Z = vertices[indices->vertex_index * 3 + 2];
                if (normals.size() > 0) {
                    const int objNdx = indices->normal_index;
                    myNormal[v].X = normals[objNdx * 3];
                    myNormal[v].Y = normals[objNdx * 3 + 1];
                    myNormal[v].Z = normals[objNdx * 3 + 2];
                    mnormals.push_back(myNormal[v]);
                    face.vert_normals_ndx[v] = objNdx;
                    face.hasShadingNormals = true;
                }
                indices++;
                mvertices.push_back(pontos[v]);
                face.bb.update(pontos[v]);
            }
            Vector v1 = pontos[0].vec2point(pontos[1]);
            Vector v2 = pontos[0].vec2point(pontos[2]);
            Vector geonormal = v1.cross(v2);
            face.geoNormal = geonormal;

            faces.push_back(face);
        }
        Geometry* mesh = new Mesh{ std::move(faces),std::move(mvertices),std::move(mnormals) };
        Primitive primitive{ mesh,shp->mesh.material_ids[0]};

        this->prims.push_back(std::move(primitive));
    }

    // your code here
    return true;
}



bool Scene::trace (Ray r, Intersection *isect) {
    Intersection curr_isect;
    bool intersection = false;    
    
    if (numPrimitives==0) return false;
    
    // iterate over all primitives
    for (auto prim_itr = prims.begin() ; prim_itr != prims.end() ; prim_itr++) {
        if (prim_itr->g->intersect(r, &curr_isect)) {
            if (!intersection) { // first intersection
                intersection = true;
                *isect = curr_isect;
                isect->f = &BRDFs[prim_itr->material_ndx];
            }
            else if (curr_isect.depth < isect->depth) {
                *isect = curr_isect;
            }
        }
    }
    return intersection;
}



