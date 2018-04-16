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

extern "C" __declspec(dllexport) ClientClass* CreateClientCPP()
{
	return new ClientClass();
}

extern "C" __declspec(dllexport) void DisposeClientCPP(ClientClass* pObject)
{
	if (pObject != NULL)
	{
		delete pObject;
		pObject = NULL;
	}
}

extern "C" __declspec(dllexport) void ConfirmConnectionCPP(ClientClass* pObject)
{
	pObject->GetClient()->confirmConnection();
}

extern "C" __declspec(dllexport) void EnableApiControlCPP(ClientClass* pObject)
{
	pObject->GetClient()->enableApiControl(true);
}

extern "C" __declspec(dllexport) void ArmDisarmCPP(ClientClass* pObject, bool isArm)
{
	pObject->GetClient()->armDisarm(isArm);
}

extern "C" __declspec(dllexport) void TakeoffCPP(ClientClass* pObject, float timeout)
{
	pObject->GetClient()->takeoff(timeout);
}

extern "C" __declspec(dllexport) void SleepClientCPP(ClientClass* pObject, float time)
{
	std::this_thread::sleep_for(std::chrono::duration<double>(time));
}

extern "C" __declspec(dllexport) void HoverCPP(ClientClass* pObject)
{
	pObject->GetClient()->hover();
}

extern "C" __declspec(dllexport) void MoveByVelocityZCPP(ClientClass* pObject, float speed, float duration)
{
	auto position = pObject->GetClient()->getPosition();
	float z = position.z(); // current position (NED coordinate system).  
	const float size = 10.0f;
	DrivetrainType driveTrain = DrivetrainType::ForwardOnly;
	YawMode yaw_mode(true, 0);
	std::cout << "moveByVelocityZ(" << speed << ", 0, " << z << "," << duration << ")" << std::endl;
	pObject->GetClient()->moveByVelocityZ(speed, 0, z, duration, driveTrain, yaw_mode);
}

extern "C" __declspec(dllexport) void LandCPP(ClientClass* pObject)
{
	pObject->GetClient()->land();
}

extern "C" __declspec(dllexport) int GetDistanceImageCPP(ClientClass* pObject, float ar[])
{

	vector<ImageRequest> request = { ImageRequest(0, ImageType::Scene), ImageRequest(1, ImageType::DepthVis, true) };
	vector<ImageResponse> response = pObject->GetClient()->simGetImages(request);
	vector<float> x = response[1].image_data_float;
	int sz = (int)x.size();
	for (int i = 0; i < sz; ++i)
	{
		ar[i] = x[i];
	}
	ar[sz] = (float) response[1].width;
	ar[sz + 1] = (float) response[1].height;
	return sz;
}

extern "C" __declspec(dllexport) float GetDistanceCPP(ClientClass* pObject)
{
	vector<ImageRequest> request = { ImageRequest(1, ImageType::DepthVis, true) };
	vector<ImageResponse> response = pObject->GetClient()->simGetImages(request);
	vector<float> x = response[0].image_data_float;
	return x[(int)(x.size() / 2) + response[0].width / 2];
}

extern "C" __declspec(dllexport) void GetPosCPP(ClientClass* pObject, float coord[])
{
	Vector3r v = pObject->GetClient()->getPosition();
	coord[0] = v.x();
	coord[1] = v.y();
	coord[2] = v.z();
}

extern "C" __declspec(dllexport) int GetImageCPP(ClientClass* pObject, int ar[])
{
	for (int i = 0; i < 20; ++i)
	{
		ar[i] = i;
	}
	return 20;
}

int main() 
{
    return 0;
}