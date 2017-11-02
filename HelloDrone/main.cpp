// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "common/common_utils/StrictMode.hpp"
STRICT_MODE_OFF
#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK
#include "rpc/rpc_error.h"
STRICT_MODE_ON

#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
#include "common/common_utils/FileSystem.hpp"
#include <iostream>
#include <chrono>

using namespace msr::airlib;

typedef VehicleCameraBase::ImageRequest ImageRequest;
typedef VehicleCameraBase::ImageResponse ImageResponse;
typedef VehicleCameraBase::ImageType ImageType;
typedef common_utils::FileSystem FileSystem;

class __declspec(dllexport) ClientClass
{
	private:
		MultirotorRpcLibClient *client;
	public:
		ClientClass()
		{
			client = new MultirotorRpcLibClient();
		};
	
		~ClientClass()
		{
			delete client;
		}

		MultirotorRpcLibClient *GetClient()
		{
			return client;
		}
};

extern "C" __declspec(dllexport) ClientClass* createClient()
{
	return new ClientClass();
}

extern "C" __declspec(dllexport) void disposeClient(ClientClass* pObject)
{
	if (pObject != NULL)
	{
		delete pObject;
		pObject = NULL;
	}
}

extern "C" __declspec(dllexport) void confirmConnection(ClientClass* pObject)
{
	pObject->GetClient()->confirmConnection();
}

extern "C" __declspec(dllexport) void enableApiControl(ClientClass* pObject)
{
	pObject->GetClient()->enableApiControl(true);
}

extern "C" __declspec(dllexport) void armDisarm(ClientClass* pObject, bool isArm)
{
	pObject->GetClient()->armDisarm(isArm);
}

extern "C" __declspec(dllexport) void takeoff(ClientClass* pObject, float timeout)
{
	pObject->GetClient()->takeoff(timeout);
}

extern "C" __declspec(dllexport) void sleepClient(ClientClass* pObject, float time)
{
	std::this_thread::sleep_for(std::chrono::duration<double>(time));
}

extern "C" __declspec(dllexport) void hover(ClientClass* pObject)
{
	pObject->GetClient()->hover();
}

extern "C" __declspec(dllexport) void moveByVelocityZ(ClientClass* pObject, float speed)
{
	auto position = pObject->GetClient()->getPosition();
	float z = position.z(); // current position (NED coordinate system).  
	const float size = 10.0f;
	const float duration = size / speed;
	DrivetrainType driveTrain = DrivetrainType::ForwardOnly;
	YawMode yaw_mode(true, 0);
	std::cout << "moveByVelocityZ(" << speed << ", 0, " << z << "," << duration << ")" << std::endl;
	pObject->GetClient()->moveByVelocityZ(speed, 0, z, duration, driveTrain, yaw_mode);
}

extern "C" __declspec(dllexport) void land(ClientClass* pObject)
{
	pObject->GetClient()->land();
}


int main() 
{
    return 0;
}