#ifndef LOADER_H
#define LOADER_H
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct FaceElement
{
    int vertexIndex{-1};
    int textureIndex{-1};
    int normalIndex{-1};
};

class ObjLoader
{
public:
    ObjLoader();
    void loadObj(const std::string& path);

    std::vector<float> getVertices() const;
    std::vector<float> getNormals() const;
    std::vector<float> getTexCoords() const;
    std::vector<FaceElement> getElements() const;
private:
    float parseFloat(const std::string& str) const;
    int parseInt(const std::string& str) const;
    FaceElement parseFaceElement(const std::string& str) const;

    std::vector<float> vertices {};
    std::vector<float> normals {};
    std::vector<float> texCoords {};
    std::vector<FaceElement> elements {};
};

#endif
