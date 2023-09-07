module;

#include <vector>
#include <string>

export module CameraDevices;

export std::vector<std::string> GetListOfCameraDevices() {
    std::vector<std::string> cameras;
    cameras.emplace_back("First Camera");
    return cameras;
}