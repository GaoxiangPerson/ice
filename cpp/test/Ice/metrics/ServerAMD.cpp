// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <TestCommon.h>
#include <TestAMDI.h>

DEFINE_TEST("serveramd")

using namespace std;

int
run(int, char**, const Ice::CommunicatorPtr& communicator)
{
    communicator->getProperties()->setProperty("TestAdapter.Endpoints", "default -p 12010");
    Ice::ObjectAdapterPtr adapter = communicator->createObjectAdapter("TestAdapter");
    adapter->add(ICE_MAKE_SHARED(MetricsI), communicator->stringToIdentity("metrics"));
    adapter->activate();

    communicator->getProperties()->setProperty("ControllerAdapter.Endpoints", "default -p 12011");
    Ice::ObjectAdapterPtr controllerAdapter = communicator->createObjectAdapter("ControllerAdapter");
    controllerAdapter->add(ICE_MAKE_SHARED(ControllerI, adapter), communicator->stringToIdentity("controller"));
    controllerAdapter->activate();

    TEST_READY
    communicator->waitForShutdown();

    return EXIT_SUCCESS;
}

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif
    Ice::CommunicatorPtr communicator;
    try
    {
        Ice::InitializationData initData;
        initData.properties = Ice::createProperties(argc, argv);
        //initData.properties->setProperty("Ice.ThreadPool.Server.Size", "1");
        //initData.properties->setProperty("Ice.ThreadPool.Server.SizeMax", "1");
        initData.properties->setProperty("Ice.Admin.Endpoints", "tcp");
        initData.properties->setProperty("Ice.Admin.InstanceName", "server");
        initData.properties->setProperty("Ice.Warn.Connections", "0");
        initData.properties->setProperty("Ice.Warn.Dispatch", "0");
        initData.properties->setProperty("Ice.MessageSizeMax", "50000");
        initData.properties->setProperty("Ice.Default.Host", "127.0.0.1");
        communicator = Ice::initialize(argc, argv, initData);
        return run(argc, argv, communicator);
    }
    catch(const Ice::Exception& ex)
    {
        cerr << ex << endl;
        return EXIT_FAILURE;
    }
}
