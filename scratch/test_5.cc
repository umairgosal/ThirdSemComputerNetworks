#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/wifi-module.h"
#include "ns3/on-off-helper.h" // Include the OnOffHelper
#include "ns3/packet-sink-helper.h" // Include the PacketSinkHelper
#include "ns3/aodv-module.h" // Include the AODV module
#include "ns3/aodv-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/applications-module.h"

using namespace ns3;

int main (int argc, char *argv[])
{

// Create p2p nodes
NodeContainer p2pNodes;
p2pNodes.Create(5); // Creating 5 nodes for the point-to-point setup

PointToPointHelper pointToPoint;
pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

// Connect nodes 0 and 1
NetDeviceContainer devices01 = pointToPoint.Install(p2pNodes.Get(0), p2pNodes.Get(1));

// Connect nodes 0 and 2
NetDeviceContainer devices02 = pointToPoint.Install(p2pNodes.Get(0), p2pNodes.Get(2));

// Connect nodes 0 and 3
NetDeviceContainer devices03 = pointToPoint.Install(p2pNodes.Get(0), p2pNodes.Get(3));

// Connect nodes 0 and 4
NetDeviceContainer devices04 = pointToPoint.Install(p2pNodes.Get(0), p2pNodes.Get(4));

// Connect nodes 1 and 2
NetDeviceContainer devices12 = pointToPoint.Install(p2pNodes.Get(1), p2pNodes.Get(2));

// Connect nodes 2 and 3
NetDeviceContainer devices23 = pointToPoint.Install(p2pNodes.Get(2), p2pNodes.Get(3));

// Connect nodes 3 and 4
NetDeviceContainer devices34 = pointToPoint.Install(p2pNodes.Get(3), p2pNodes.Get(4));

// Connect nodes 4 and 1
NetDeviceContainer devices41 = pointToPoint.Install(p2pNodes.Get(4), p2pNodes.Get(1));


// Manually set positions for nodes around Node 0 in a cross shape
Ptr<ConstantPositionMobilityModel> mobilityModels[5];
for (uint32_t i = 0; i < 5; ++i) {
    mobilityModels[i] = CreateObject<ConstantPositionMobilityModel>();
    p2pNodes.Get(i)->AggregateObject(mobilityModels[i]);

    double xPos = 0.0, yPos = 0.0;
    switch (i) {
        case 0: // Node 0 at the center
            xPos = 25.0;
            yPos = 25.0;
            break;
        case 1: // Node 1 to the left of Node 0
            xPos = 0.0;
            yPos = 25.0;
            break;
        case 2: // Node 2 above Node 0
            xPos = 25.0;
            yPos = 50.0;
            break;
        case 3: // Node 3 to the right of Node 0
            xPos = 50.0;
            yPos = 25.0;
            break;
        case 4: // Node 4 below Node 0
            xPos = 25.0;
            yPos = 0.0;
            break;
    }
    Vector pos = Vector(xPos, yPos, 0.0);
    mobilityModels[i]->SetPosition(pos);
}




NodeContainer wifiStaNodes[5];  // Array to hold Wi-Fi STA nodes for each p2p node
NetDeviceContainer wifiStaDevices[5];  // Array to hold Wi-Fi STA devices for each p2p node

for (uint32_t i = 0; i < 5; ++i) {
    NodeContainer wifiApNode = p2pNodes.Get(i);

    wifiStaNodes[i].Create(9);  // Create 4 Wi-Fi STA nodes for each p2p node

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy;
    phy.SetChannel(channel.Create());

    WifiMacHelper mac;
    Ssid ssid = Ssid("ns-3-ssid");

    WifiHelper wifi;

    NetDeviceContainer staDevices;
    mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));
    staDevices = wifi.Install(phy, mac, wifiStaNodes[i]);

    NetDeviceContainer apDevices;
    mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
    apDevices = wifi.Install(phy, mac, wifiApNode);


    MobilityHelper mobility;
    switch (i) {
        case 0: // Node 0 at the center
                mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue(27.0),
                                  "MinY", DoubleValue(27.0),
                                  "DeltaX", DoubleValue(2.0),  // Adjust the size of the area as needed
                                  "DeltaY", DoubleValue(2.0),
                                  "GridWidth", UintegerValue(3),
                                  "LayoutType", StringValue("RowFirst"));

    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                              "Bounds", RectangleValue(Rectangle(25, 35, 25, 35)));  // Adjust the bounds as needed

//      mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
     mobility.Install(wifiStaNodes[i]);
            break;
        case 1: // Node 1 to the left of Node 0
                mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue(2.0),
                                  "MinY", DoubleValue(27.0),
                                  "DeltaX", DoubleValue(2.0),  // Adjust the size of the area as needed
                                  "DeltaY", DoubleValue(2.0),
                                  "GridWidth", UintegerValue(3),
                                  "LayoutType", StringValue("RowFirst"));

    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                              "Bounds", RectangleValue(Rectangle(0, 10, 25, 35)));  // Adjust the bounds as needed

//      mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
     mobility.Install(wifiStaNodes[i]);
            break;
        case 2: // Node 2 above Node 0
                mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue(25.0),
                                  "MinY", DoubleValue(40.0),
                                  "DeltaX", DoubleValue(2.0),  // Adjust the size of the area as needed
                                  "DeltaY", DoubleValue(2.0),
                                  "GridWidth", UintegerValue(3),
                                  "LayoutType", StringValue("RowFirst"));

    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                              "Bounds", RectangleValue(Rectangle(23, 33, 38, 48)));  // Adjust the bounds as needed

//      mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
     mobility.Install(wifiStaNodes[i]);
            break;
        case 3: // Node 3 to the right of Node 0
                mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue(40.0),
                                  "MinY", DoubleValue(25.0),
                                  "DeltaX", DoubleValue(2.0),  // Adjust the size of the area as needed
                                  "DeltaY", DoubleValue(2.0),
                                  "GridWidth", UintegerValue(3),
                                  "LayoutType", StringValue("RowFirst"));

    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                              "Bounds", RectangleValue(Rectangle(38, 48, 23, 33)));  // Adjust the bounds as needed

//      mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
     mobility.Install(wifiStaNodes[i]);
            break;
        case 4: // Node 4 below Node 0
                mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue(27.0),
                                  "MinY", DoubleValue(2.0),
                                  "DeltaX", DoubleValue(2.0),  // Adjust the size of the area as needed
                                  "DeltaY", DoubleValue(2.0),
                                  "GridWidth", UintegerValue(3),
                                  "LayoutType", StringValue("RowFirst"));

    mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                              "Bounds", RectangleValue(Rectangle(25, 35, 0, 10)));  // Adjust the bounds as needed

//      mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
     mobility.Install(wifiStaNodes[i]);
            break;
    }
//     mobility.SetPositionAllocator("ns3::GridPositionAllocator",
//                                   "MinX", DoubleValue(0.0),
//                                   "MinY", DoubleValue(0.0),
//                                   "DeltaX", DoubleValue(2.0),  // Adjust the size of the area as needed
//                                   "DeltaY", DoubleValue(2.0),
//                                   "GridWidth", UintegerValue(2),
//                                   "LayoutType", StringValue("RowFirst"));
// //
// //     mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
// //                               "Bounds", RectangleValue(Rectangle(0, 10, 0, 10)));  // Adjust the bounds as needed
//
//      mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
//      mobility.Install(wifiStaNodes[i]);
//
// //     mobility.Install(wifiStaNodes[i]);
//     mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
//     mobility.Install(wifiApNode);


    InternetStackHelper stack;
    stack.Install(wifiApNode); // Install the internet stack for WiFi AP nodes

    wifiStaDevices[i] = staDevices;  // Store devices in an array for further use if needed

    // Connect the Wi-Fi STA nodes to the corresponding p2p node
    YansWifiPhyHelper wifiPhy;
    wifiPhy.SetChannel(channel.Create());
    WifiMacHelper wifiMac;
    NetDeviceContainer wifiStaApDevices;
    wifiMac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));
    wifiStaApDevices = wifi.Install(wifiPhy, wifiMac, p2pNodes.Get(i));

    // This adds the Wi-Fi STA devices to the same container as the existing p2p devices
    wifiStaDevices[i].Add(wifiStaApDevices.Get(0));
}


//generating xml file to feed the netanim and get visual simulation
  AnimationInterface anim ("test_5.xml"); // Create the animation object and configure output file name
  Simulator::Stop(Seconds(10.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
