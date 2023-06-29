//
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#include <iostream>
#include <limits>
#include <optional>
#include <vector>
#include <memory>

#include "scene/scene.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

scene_t::scene_t(std::vector<std::unique_ptr<light_t>> lights) noexcept :
    success{false}, prims{}, lights{std::move(lights)}, materials{} {}

scene_t::scene_t(std::string const& fn, std::vector<std::unique_ptr<light_t>> lights):
    success{false}, prims{}, lights{std::move(lights)}, materials{}
{
    this->load(fn);
}


static void print_info(tinyobj::ObjReader const& obj_reader){

    tinyobj::attrib_t const& attrib {obj_reader.GetAttrib()};
    std::vector<tinyobj::shape_t> const& shapes {obj_reader.GetShapes()};
    std::vector<tinyobj::material_t> const& materials {obj_reader.GetMaterials()};

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


void scene_t::print_summary() const {
    std::cout << "#primitives = " << this->prims.size()  << " ; "
              << "#lights = "     << this->lights.size() << " ; "
              << "#materials = "  << this->materials.size()  << '\n';
}

/*
 Use tiny load to load .obj scene descriptions
 https://github.com/tinyobjloader/tinyobjloader
 */

void scene_t::load(std::string const& fn){

    tinyobj::ObjReader obj_reader {};

    if(!obj_reader.ParseFromFile(fn)){
        this->success = false;
        return;
    }

    print_info(obj_reader);

    // convert loader's representation to my representation
    tinyobj::attrib_t const& attrib {obj_reader.GetAttrib()};

    std::vector<tinyobj::material_t> const& materials {obj_reader.GetMaterials()};
    std::vector<tinyobj::shape_t>    const& shapes    {obj_reader.GetShapes()};
    std::vector<float> const& obj_vertices {attrib.vertices};
    std::vector<float> const& obj_normals  {attrib.normals};

    // iterate over shapes
    for(tinyobj::shape_t const& shape : shapes){

        std::vector<face_t> faces {};
        std::unordered_map<size_t, vec3_t> mesh_vertices {};
        std::unordered_map<size_t, vec3_t> mesh_normals  {};
        bb_t mesh_bb {};

        using indices_iter_t = std::vector<tinyobj::index_t>::const_iterator;
        for(
            indices_iter_t indices_iter {shape.mesh.indices.begin()};
            indices_iter != shape.mesh.indices.end();
        ){

            face_t face {};
            if(indices_iter->normal_index > -1)
                face.normals_indices = std::make_optional<std::array<size_t, 3>>();

            // process 3 vertices
            for(size_t v {0}; v < 3; ++v){

                face.vert_indices[v] = static_cast<size_t>(indices_iter->vertex_index);

                vec3_t const vertice {
                    obj_vertices.at(static_cast<size_t>(indices_iter->vertex_index) * 3),
                    obj_vertices.at(static_cast<size_t>(indices_iter->vertex_index) * 3 + 1),
                    obj_vertices.at(static_cast<size_t>(indices_iter->vertex_index) * 3 + 2)
                };

                mesh_vertices.insert_or_assign(
                    face.vert_indices[v],
                    vertice
                );

                if(face.has_shading_normals()){
                    face.normals_indices.value()[v] =
                        static_cast<size_t>(indices_iter->normal_index);

                    vec3_t const normal {
                        obj_normals.at(static_cast<size_t>(indices_iter->normal_index) * 3),
                        obj_normals.at(static_cast<size_t>(indices_iter->normal_index) * 3 + 1),
                        obj_normals.at(static_cast<size_t>(indices_iter->normal_index) * 3 + 2)
                    };

                    if(!mesh_normals.contains(face.normals_indices.value()[v]))
                        mesh_normals[face.normals_indices.value()[v]] = normal;
                }

                ++indices_iter;
            }


            face.geo_normal = vec3_t::surface_normal(
                mesh_vertices.at(face.vert_indices[0]),
                mesh_vertices.at(face.vert_indices[1]),
                mesh_vertices.at(face.vert_indices[2])
            );

            face.bb = {
                mesh_vertices.at(face.vert_indices[0]),
                mesh_vertices.at(face.vert_indices[1]),
                mesh_vertices.at(face.vert_indices[2])
            };

            face.edge1 = {
                mesh_vertices.at(face.vert_indices[1]) -
                mesh_vertices.at(face.vert_indices[0])
            };
            face.edge2 = {
                mesh_vertices.at(face.vert_indices[2]) -
                mesh_vertices.at(face.vert_indices[0])
            };

            faces.push_back(face);
            mesh_bb = bb_t::from_union_of(face.bb, mesh_bb);
        }

        std::unique_ptr<geometry_t> mesh {
            std::make_unique<mesh_t>(
                std::move(faces),
                std::move(mesh_vertices),
                std::move(mesh_normals),
                mesh_bb
            )
        };

        // assumes all faces in a mesh use the same material!
        int const material_index {shape.mesh.material_ids.at(0)};
        this->prims.push_back(
            {
                std::move(mesh),
                material_index < 0 ?
                    std::nullopt :
                    std::make_optional(static_cast<size_t>(material_index))
            }
        );
    }


    for(tinyobj::material_t const& mat : materials){

        rgb_t const ka {mat.ambient[0],       mat.ambient[1],       mat.ambient[2]};
        rgb_t const kd {mat.diffuse[0],       mat.diffuse[1],       mat.diffuse[2]};
        rgb_t const ks {mat.specular[0],      mat.specular[1],      mat.specular[2]};
        rgb_t const kt {mat.transmittance[0], mat.transmittance[1], mat.transmittance[2]};
        float const ns {mat.shininess};
        // very bad workaround
        float const metalness {mat.name.starts_with("metal_") ? 1.f : 0.f};

        this->materials.push_back({ka, kd, ks, kt, ns, metalness});
    }

    this->success = true;
}

bool scene_t::is_loaded() const noexcept {
    return this->success;
}

std::optional<intersection_t> scene_t::trace(ray_t const& r) const noexcept {

    bool intersects {false};
    intersection_t min_isect {};
    min_isect.depth = std::numeric_limits<float>::max();

    // iterate over all area lights
    for(std::unique_ptr<light_t> const& l : this->lights){

        if(l->type == light_type_t::AREA_LIGHT){

            light_properties_t const lprops {l->get_properties()};

            std::optional<intersection_t> const inter {lprops.light_geom->intersect(r)};
            if(!inter.has_value())
                continue;

            intersects = true;
            if(inter.value().depth < min_isect.depth){
                min_isect = inter.value();
                min_isect.le = std::make_optional(lprops.power.value());
            }
        }
    }

    // iterate over all primitives
    for(primitive_t const& prim : this->prims){

        std::optional<intersection_t> const inter {prim.geo->intersect(r)};
        if(!inter.has_value())
            continue;

        intersects = true;
        if(inter.value().depth < min_isect.depth){
            min_isect = inter.value();
            min_isect.material_index = prim.material_index;
        }
    }

    return intersects ? std::make_optional(min_isect) : std::nullopt;
}


std::pair<scene_t::lights_citer_t, scene_t::lights_citer_t>
scene_t::get_lights_iterator() const noexcept {
    return std::make_pair(this->lights.begin(), this->lights.end());
}

material_t const* scene_t::material_at(size_t const index) const noexcept {
    if(index >= this->materials.size())
        return nullptr;
    return &*(this->materials.cbegin() + static_cast<long>(index));
}

bool scene_t::is_visible(ray_t const& r, float const max_l) const noexcept {

    // iterate over all primitives while visible
    for(primitive_t const& prim : this->prims){
        std::optional<intersection_t> const inter {prim.geo->intersect(r)};
        if(inter.has_value() && inter.value().depth < max_l)
            return false;
    }

    return true;
}
