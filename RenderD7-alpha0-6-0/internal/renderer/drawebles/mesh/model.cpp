#include "model.hpp"
#include "OBJ_Loader.h"

namespace d7gfx {
        bool Model::loadFromFile(const std::string& t_filename) {
            objl::Loader loader;

            if(!loader.LoadFile(t_filename)) {
                printf("Failed!\n");
                return false;
            }

            for (unsigned int i = 0; i < loader.LoadedMeshes.size(); i++) {
                printf("\x1b[3;1HLoading Model: %i/%i\x1b[K", i, (int)loader.LoadedMeshes.size());
                objl::Mesh curMesh = loader.LoadedMeshes[i];
                
                for (unsigned int j = 0; j < curMesh.Vertices.size() - 2; j += 3) {
                    printf("\x1b[4;1HLoading Model Vertices: %i/%i\x1b[K", j, (int)curMesh.Vertices.size() - 2);
                    addPolygon(d7gfx::Mesh::Polygon(
                        { {
                            curMesh.Vertices[j].Position.X,
                            curMesh.Vertices[j].Position.Y,
                            curMesh.Vertices[j].Position.Z
                        }, {
                            curMesh.Vertices[j].TextureCoordinate.X,
                            curMesh.Vertices[j].TextureCoordinate.Y
                        }, {
                            curMesh.Vertices[j].Normal.X,
                            curMesh.Vertices[j].Normal.Y,
                            curMesh.Vertices[j].Normal.Z
                        } },

                        { {
                            curMesh.Vertices[j + 1].Position.X,
                            curMesh.Vertices[j + 1].Position.Y,
                            curMesh.Vertices[j + 1].Position.Z
                        }, {
                            curMesh.Vertices[j + 1].TextureCoordinate.X,
                            curMesh.Vertices[j + 1].TextureCoordinate.Y
                        }, {
                            curMesh.Vertices[j + 1].Normal.X,
                            curMesh.Vertices[j + 1].Normal.Y,
                            curMesh.Vertices[j + 1].Normal.Z
                        } },

                        { {
                            curMesh.Vertices[j + 2].Position.X,
                            curMesh.Vertices[j + 2].Position.Y,
                            curMesh.Vertices[j + 2].Position.Z
                        }, {
                            curMesh.Vertices[j + 2].TextureCoordinate.X,
                            curMesh.Vertices[j + 2].TextureCoordinate.Y
                        }, {
                            curMesh.Vertices[j + 2].Normal.X,
                            curMesh.Vertices[j + 2].Normal.Y,
                            curMesh.Vertices[j + 2].Normal.Z
                        } }
                    ));
                }
            }

            updateVBO();

            return true;
        }
} /* d7gfx */
