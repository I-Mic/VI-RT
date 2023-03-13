//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#include <optional>
#include <vector>
#include <memory>

#include "primitive/bb.hpp"
#include "primitive/brdf/brdf.hpp"
#include "primitive/geometry/geometry.hpp"
#include "rays/intersection.hpp"
#include "scene/scene.hpp"
#include "primitive/primitive.hpp"
#include "primitive/geometry/mesh.hpp"
#include "primitive/brdf/phong.hpp"
#include "utils/rgb.hpp"
#include "utils/vector.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


namespace scene {

scene_t::scene_t() noexcept :
    prims{},
    lights{},
    brdfs{} {}

static void print_info(tinyobj::ObjReader const& obj_reader){

    tinyobj::attrib_t const attrib { obj_reader.GetAttrib() };
    std::vector<tinyobj::shape_t> const shapes { obj_reader.GetShapes() };
    std::vector<tinyobj::material_t> const materials { obj_reader.GetMaterials() };

    std::cout << "# of vertices  : " << (attrib.vertices.size() / 3)  << '\n'
              << "# of normals   : " << (attrib.normals.size() / 3)   << '\n'
              << "# of texcoords : " << (attrib.texcoords.size() / 2) << '\n'
              << "# of shapes    : " << shapes.size()                 << '\n'
              << "# of materials : " << materials.size()              << '\n';

    // Iterate shapes
    for(tinyobj::shape_t const& s : shapes) {
        // assume each face has 3 vertices
        std::cout << "Processing shape " << s.name << '\n';
        // iterate faces
        // assume each face has 3 vertices

        using index_t_iter = std::vector<tinyobj::index_t>::const_iterator;
        for(index_t_iter i {s.mesh.indices.begin()}; i != s.mesh.indices.end(); i += 3){
            // process 3 vertices
            for(index_t_iter v {i}; v != i + 3; ++v)
                std::cout << v->vertex_index << ' ';

            std::cout << '\n';
        }

        std::cout << "There are " << s.mesh.material_ids.size() << " material indexes\n\n";
    }
}

bool scene_t::set_lights() noexcept {
    return true;
}

void scene_t::print_summary() const {
    std::cout << "#primitives = " << this->prims.size()  << " ; "
              << "#lights = "     << this->lights.size() << " ; "
              << "#materials = "  << this->brdfs.size()  << " ;\n";
}

/*
 Use tiny load to load .obj scene descriptions
 https://github.com/tinyobjloader/tinyobjloader
 */

bool scene_t::load(std::string const& fname){

    tinyobj::ObjReader obj_reader {};

    if(!obj_reader.ParseFromFile(fname))
        return false;

    print_info(obj_reader);

    // convert loader's representation to my representation
    tinyobj::attrib_t const& attrib {obj_reader.GetAttrib()};

    std::vector<tinyobj::material_t> const& materials {obj_reader.GetMaterials()};
    std::vector<tinyobj::shape_t>    const& shapes    {obj_reader.GetShapes()};
    std::vector<float> const& obj_vertices {attrib.vertices};
    std::vector<float> const& obj_normals  {attrib.normals};

    // iterate over shapes
    for(tinyobj::shape_t const& shape : shapes){

        std::vector<prim::geo::face_t> faces {};
        std::vector<vec::vec3_t> mesh_vertices {};
        std::vector<vec::vec3_t> mesh_normals  {};
        prim::bb_t mesh_bb {};

        //needed to compute geometric normal only
        std::array<vec::vec3_t, 3> face_vertices {};

        using indices_iter_t = std::vector<tinyobj::index_t>::const_iterator;
        for(indices_iter_t indices_iter {shape.mesh.indices.begin()}; indices_iter != shape.mesh.indices.end();){

            prim::geo::face_t face {};
            if(obj_normals.size() > 0)
                face.vert_normals_indices = std::make_optional(std::array<size_t, 3>{});

            // process 3 vertices
            for(size_t v {0}; v < 3; ++v){

                face.vert_indices[v] = static_cast<size_t>(indices_iter->vertex_index);
                vec::vec3_t const vertice {
                    obj_vertices[static_cast<size_t>(indices_iter->vertex_index * 3)],
                    obj_vertices[static_cast<size_t>(indices_iter->vertex_index * 3 + 1)],
                    obj_vertices[static_cast<size_t>(indices_iter->vertex_index * 3 + 2)]
                };
                mesh_vertices.push_back(vertice);
                face_vertices[v] = vertice;

                if(face.has_shading_normals()){

                    face.vert_normals_indices.value()[v] = static_cast<size_t>(indices_iter->normal_index);
                    vec::vec3_t const normal {
                        obj_normals[static_cast<size_t>(indices_iter->normal_index * 3)],
                        obj_normals[static_cast<size_t>(indices_iter->normal_index * 3 + 1)],
                        obj_normals[static_cast<size_t>(indices_iter->normal_index * 3 + 2)]
                    };
                    mesh_normals.push_back(normal);
                }

                ++indices_iter;
            }

            vec::vec3_t const v1 {face_vertices[1] - face_vertices[0]};
            vec::vec3_t const v2 {face_vertices[2] - face_vertices[0]};
            vec::vec3_t const geo_normal {v1.cross_product(v2)};
            face.geo_normal = geo_normal;

            prim::bb_t const face_bb {face_vertices[0], face_vertices[1], face_vertices[2]}; 
            face.bb = face_bb;

            faces.push_back(face);

            mesh_bb = prim::bb_t::from_union_of(face_bb, mesh_bb);
        }

        std::unique_ptr<prim::geo::geometry_t> mesh {
            std::make_unique<prim::geo::mesh_t>(
                std::move(faces),
                std::move(mesh_vertices),
                std::move(mesh_normals),
                mesh_bb
            )
        };

        // assumes all faces in a mesh use the same material!
        this->prims.push_back({std::move(mesh), static_cast<size_t>(shape.mesh.material_ids[0])});
    }


    for(tinyobj::material_t const& mat : materials){

        rgb::rgb_t const ka {mat.ambient[0],       mat.ambient[1],       mat.ambient[2]};
        rgb::rgb_t const kd {mat.diffuse[0],       mat.diffuse[1],       mat.diffuse[2]};
        rgb::rgb_t const ks {mat.specular[0],      mat.specular[1],      mat.specular[2]};
        rgb::rgb_t const kt {mat.transmittance[0], mat.transmittance[1], mat.transmittance[2]};

        std::unique_ptr<prim::brdf::brdf_t> brdf {
            std::make_unique<prim::brdf::phong_t>(ka, kd, ks, kt)
        };

        this->brdfs.push_back(std::move(brdf));
    }

    return true;
}

std::optional<ray::intersection_t> scene_t::trace(ray::ray_t const& r) const {

    if(this->prims.size() == 0)
        return std::nullopt;

    ray::intersection_t curr_isect {};
    bool intersects {false};

    // iterate over all primitives
    for(prim::primitive_t const& prim : this->prims){

        std::optional<ray::intersection_t> const inter = prim.g->intersect(r);

        if(inter.has_value()){

            if(!intersects){ // first intersection
                intersects = true;
                curr_isect = inter.value();
                curr_isect.f = this->brdfs[prim.material_index].get();
            }
            else if (inter.value().depth < curr_isect.depth) {
                curr_isect = inter.value();
            }
        }
    }

    return intersects ? std::make_optional(curr_isect) : std::nullopt;
}

};
