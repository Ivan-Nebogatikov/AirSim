// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

//in header only mode, control library is not available
#ifndef AIRLIB_HEADER_ONLY
//RPC code requires C++14. If build system like Unreal doesn't support it then use compiled binaries
#ifndef AIRLIB_NO_RPC
//if using Unreal Build system then include precompiled header file first

#include "vehicles/car/api/CarRpcLibClient.hpp"

#include "common/Common.hpp"
#include "common/ClockFactory.hpp"
#include <thread>
STRICT_MODE_OFF
#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK
#undef check
#include "rpc/client.h"
#include "vehicles/car/api/CarRpcLibAdapators.hpp"
STRICT_MODE_ON
#ifdef _MSC_VER
__pragma(warning( disable : 4239))
#endif	



namespace msr { namespace airlib {


typedef msr::airlib_rpclib::CarRpcLibAdapators CarRpcLibAdapators;

CarRpcLibClient::CarRpcLibClient(const string&  ip_address, uint16_t port, uint timeout_ms)
    : RpcLibClientBase(ip_address, port, timeout_ms)
{
}

CarRpcLibClient::~CarRpcLibClient()
{}

void CarRpcLibClient::setCarControls(const CarApiBase::CarControls& controls)
{
    static_cast<rpc::client*>(getClient())->
        call("setCarControls", CarRpcLibAdapators::CarControls(controls));
}

void CarRpcLibClient::reset()
{
    static_cast<rpc::client*>(getClient())->call("reset");
}

CarApiBase::CarState CarRpcLibClient::getCarState()
{
    return static_cast<rpc::client*>(getClient())->
        call("getCarState").as<CarRpcLibAdapators::CarState>().to();

}


}} //namespace

#endif
#endif
