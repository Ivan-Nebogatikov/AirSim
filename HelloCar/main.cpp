// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "common/common_utils/StrictMode.hpp"
STRICT_MODE_OFF
#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK
#include "rpc/rpc_error.h"
STRICT_MODE_ON

#include "vehicles/car/api/CarRpcLibClient.hpp"
#include "common/common_utils/FileSystem.hpp"
#include <iostream>
#include <chrono>



int main() 
{
    using namespace msr::airlib;

    std::cout << "Make sure settings.json has \"SimMode\"=\"Car\" at root. Press Enter to continue." << std::endl; std::cin.get();

    // This assumes you are running DroneServer already on the same machine.
    // DroneServer must be running first.
    msr::airlib::CarRpcLibClient client;
    typedef VehicleCameraBase::ImageRequest ImageRequest;
    typedef VehicleCameraBase::ImageResponse ImageResponse;
    typedef VehicleCameraBase::ImageType ImageType;
    typedef common_utils::FileSystem FileSystem;
    
    try {
        client.confirmConnection();

        std::cout << "Press Enter to get FPV image" << std::endl; std::cin.get();
        vector<ImageRequest> request = { ImageRequest(0, ImageType::Scene), ImageRequest(1, ImageType::DepthMeters, true) };
        const vector<ImageResponse>& response = client.simGetImages(request);
        std::cout << "# of images recieved: " << response.size() << std::endl;

        if (response.size() > 0) {
            std::cout << "Enter path with ending separator to save images (leave empty for no save)" << std::endl; 
            std::string path;
            std::getline(std::cin, path);

            for (const ImageResponse& image_info : response) {
                std::cout << "Image uint8 size: " << image_info.image_data_uint8.size() << std::endl;
                std::cout << "Image float size: " << image_info.image_data_float.size() << std::endl;

                if (path != "") {
                    std::string file_path = FileSystem::combine(path, std::to_string(image_info.time_stamp));
                    if (image_info.pixels_as_float) {
                        Utils::writePfmFile(image_info.image_data_float.data(), image_info.width, image_info.height,
                            file_path + ".pfm");
                    }
                    else {
                        std::ofstream file(file_path + ".png", std::ios::binary);
                        file.write(reinterpret_cast<const char*>(image_info.image_data_uint8.data()), image_info.image_data_uint8.size());
                        file.close();
                    }
                }
            }
        }

        std::cout << "Press enter to drive forward" << std::endl; std::cin.get();
        client.enableApiControl(true);
        CarControllerBase::CarControls controls;
        controls.throttle = 1;
        client.setCarControls(controls);

        std::cout << "Press Enter to activate handbreak" << std::endl; std::cin.get();
        controls.handbreak = true;
        client.setCarControls(controls);

        std::cout << "Press Enter to take turn and drive backward" << std::endl; std::cin.get();
        controls.handbreak = false;
        controls.throttle = -1;
        controls.steering = 1;
        client.setCarControls(controls);

        std::cout << "Press Enter to stop" << std::endl; std::cin.get();
        client.setCarControls(CarControllerBase::CarControls());
    }
    catch (rpc::rpc_error&  e) {
        std::string msg = e.get_error().as<std::string>();
        std::cout << "Exception raised by the API, something went wrong." << std::endl << msg << std::endl; std::cin.get();
    }

    return 0;
}