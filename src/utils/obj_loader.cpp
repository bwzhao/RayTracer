#include "utils/obj_loader.h"

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
// Optional. define TINYOBJLOADER_USE_MAPBOX_EARCUT gives robust trinagulation. Requires C++11
#define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"

ObjectList load_obj(std::string obj_file_path, double scale_x, double scale_y, double scale_z,
                    std::shared_ptr<Material> material) {
    ObjectList boxes1;

    if (material == nullptr) {
        auto texture = make_shared<ImageTexture>("../textures/viking_room.png");
        material = make_shared<Lambertian>(texture);
    }

    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto red   = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    std::string inputfile = obj_file_path;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str())) {
        throw std::runtime_error(warn + err);
    }

    for (size_t s = 0; s < shapes.size(); s++) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            std::array<Vec3, 3> vertex_corr, normal_corr, texture_corr;
            bool flag_normal = true;
            bool flag_texture = true;

            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                vertex_corr[v] = Vec3(vx * scale_x, vy * scale_y, vz * scale_z);

                if (idx.normal_index >= 0) {
                    tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                    tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                    tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];

                    normal_corr[v] = unit_vector(Vec3(nx * scale_x, ny * scale_y, nz * scale_z));
                }
                else{
                    flag_normal = false;
                }

                if (idx.texcoord_index >= 0) {
                    tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                    tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];

                    texture_corr[v] = Vec3(tx, ty, 0);
                }
                else{
                    flag_texture = false;
                }

// Optional: vertex colors
// tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
// tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
// tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
            }
            index_offset += fv;

// per-face material
//            shapes[s].mesh.material_ids[f];

            if (!flag_normal) {
                normal_corr[0] = unit_vector(cross(vertex_corr[1] - vertex_corr[0], vertex_corr[2] - vertex_corr[0]));
                normal_corr[1] = unit_vector(cross(vertex_corr[1] - vertex_corr[0], vertex_corr[2] - vertex_corr[0]));
                normal_corr[2] = unit_vector(cross(vertex_corr[1] - vertex_corr[0], vertex_corr[2] - vertex_corr[0]));
            }
            if (!flag_texture) {
                texture_corr[0] = Vec3(0, 0, 0);
                texture_corr[0] = Vec3(0, 1, 0);
                texture_corr[0] = Vec3(1, 1, 0);
            }

            boxes1.add(make_shared<Triangle>(vertex_corr[0],
                                              vertex_corr[1],
                                              vertex_corr[2], material,
                                              normal_corr[0],
                                              normal_corr[1],
                                              normal_corr[2],
                                              texture_corr[0],
                                              texture_corr[1],
                                              texture_corr[2]));
        }
    }

    return boxes1;
}
