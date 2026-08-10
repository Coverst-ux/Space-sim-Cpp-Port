// loader.cpp
#include "json_loader.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::vector<Body> load_bodies(std::string filepath) {
std::ifstream file(filepath);
json data = json::parse(file);

std::vector<Body> bodies;

for (auto& b : data["bodies"]) {
    std::string name = b["name"];
    float mass = b["mass"];
    double radius = b["radius"];
    float px = b["position"][0];
    float py = b["position"][1];
    float pz = b["position"][2];
    float vx = b["velocity"][0];
    float vy = b["velocity"][1];
    float vz = b["velocity"][2];

    vector3d position(px, py, pz);
    vector3d velocity(vx, vy, vz);
    Body newBody(name, mass, position, velocity, radius);
    bodies.push_back(newBody);
}

return bodies;
}