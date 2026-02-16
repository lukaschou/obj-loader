#include <cstdlib>
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <sstream>

#include "loader.h"

ObjLoader::ObjLoader(){}

float ObjLoader::parseFloat(const std::string& str) const {
    std::string::size_type size;
    float f = std::stof(str, &size);
    if (size < str.length()) {
        throw std::invalid_argument{"parseFloat"};
    }

    return f;
}

int ObjLoader::parseInt(const std::string& str) const {
    std::string::size_type size;
    int i = std::stoul(str, &size);
    if (size < str.length()) {
        throw std::invalid_argument{"parseInt"};
    }

    return i;
}

FaceElement ObjLoader::parseFaceElement(const std::string& str) const {
    FaceElement element{};
    
    int startPos = 0;
    std::string::size_type endPos = 0;
    endPos = str.find("/");
    // There will always at least be a vertex index
    element.vertexIndex = parseInt(str.substr(startPos, endPos));
    if (endPos == std::string::npos) {
        return element;
    }
    
    startPos = endPos + 1;
    endPos = str.find("/", startPos);
    std::string vt = str.substr(startPos, 
            endPos == std::string::npos ? endPos : endPos - startPos);
    if (!vt.empty()) {
        element.textureIndex = parseInt(vt);
    }
    if (endPos == std::string::npos) {
        return element;
    }
    
    
    startPos = endPos + 1;
    endPos = str.find("/", startPos);
    // If there is another reference number, the face element is invalid
    if (endPos != std::string::npos) {
        throw "Invalid element";
    }
    std::string vn = str.substr(startPos, endPos - startPos);
    if (!vn.empty()) {
        element.normalIndex = parseInt(str.substr(startPos));
    }

    return element;
}

void ObjLoader::loadObj(const std::string& path) {
    std::ifstream inf{ path };

    if (!inf) {
        throw std::runtime_error("Failed to open file " + std::string(path));
    }

    std::string strInput{};
    int lineNum = 1;
    while (std::getline(inf, strInput)) {
        // Remove all commments before processing a line
        std::string line = strInput.substr(0, strInput.find("#"));
        std::stringstream ss{line};

        std::string keyword{};
        ss >> keyword;
        if (keyword == "v") {
            // TODO: Optional weight values??
            std::string sx{}, sy{}, sz{};
            ss >> sx >> sy >> sz;
            
            try {
                for (const auto& strComponent : {sx, sy, sz}) {
                    float component = parseFloat(strComponent);
                    vertices.push_back(component);
                }
            } catch (const std::invalid_argument& ia) {
                std::cerr << "Error: Failed to parse vertex component\n  Line" 
                    << ' ' << lineNum << ": " << strInput << '\n';
                exit(EXIT_FAILURE);
            } 
        } else if (keyword == "vn") {
            std::string si{}, sj{}, sk{};
            ss >> si >> sj >> sk;

            try {
                for (const auto& strComponent : {si, sj, sk}) {
                    float component = parseFloat(strComponent);
                    normals.push_back(component);
                }
            } catch (const std::invalid_argument& ia) {
                std::cerr << "Error: Failed to parse vertex normal component\n  Line" 
                    << ' ' << lineNum << ": " << strInput << '\n';
                exit(EXIT_FAILURE);
            }
        } else if (keyword == "vt") {
            std::string su{}, sv{}, st{};
            ss >> su >> sv >> st;
            
            try {
                texCoords.push_back(parseFloat(su));

                for (const auto& strComponent : {sv, st}) {
                    if (strComponent.empty()) {
                        texCoords.push_back(0);
                    } else {
                        texCoords.push_back(parseFloat(strComponent));
                    }
                }
                
            } catch (const std::invalid_argument& ia) {
                std::cerr << "Error: Failed to parse vertex texture component\n  Line" 
                    << ' ' << lineNum << ": " << strInput << '\n';
                exit(EXIT_FAILURE);
            }
        } else if (keyword == "f") {
            // TODO: Negative references and variable x-sided faces
            std::string f1{}, f2{}, f3{};
            ss >> f1 >> f2 >> f3;
            try {
                FaceElement firstFaceElement = parseFaceElement(f1);
                elements.push_back(firstFaceElement);

                for (const auto& strFace : {f2, f3}) {
                    FaceElement faceElement = parseFaceElement(strFace);
                    if ((faceElement.textureIndex == -1 && elements.back().textureIndex > 0) ||
                            (faceElement.normalIndex == -1 && elements.back().normalIndex > 0)) {
                        throw std::invalid_argument{"loadObj"}; 
                    }
    
                }
            } catch (const std::invalid_argument& ia) {
                std::cerr << "Error: Failed to parse face element component\n  Line" 
                    << ' ' << lineNum << ": " << strInput << '\n';
                exit(EXIT_FAILURE);
            }
        } else {
            std::cerr << "Error: Unknown keyword '" << keyword << "'\n  Line"
                << ' ' << lineNum << ": " << strInput << '\n';
            exit(EXIT_FAILURE);
        } 
        lineNum++;
    }
    return;
}

std::vector<float> ObjLoader::getVertices() const {
    return vertices;
}

std::vector<float> ObjLoader::getNormals() const {
    return normals;
}

std::vector<float> ObjLoader::getTexCoords() const {
    return texCoords;
}
std::vector<FaceElement> ObjLoader::getElements() const {
    return elements;
}
