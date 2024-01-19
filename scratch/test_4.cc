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

// custom function for getting devices in range of the AS's
NetDeviceContainer GetDevicesInRange(const NetDeviceContainer& devices, uint32_t startIndex, uint32_t endIndex) {
    NetDeviceContainer selectedDevices;

    for (uint32_t i = startIndex; i <= endIndex; ++i) {
        selectedDevices.Add(devices.Get(i));
    }

    return selectedDevices;
}


int main (int argc, char *argv[])
{

//Create nodes with node class
  NodeContainer nodes;
  nodes.Create (60);

// Define the source and destination nodes
  Ptr<Node> sourceNode = nodes.Get(0);
  Ptr<Node> destinationNode = nodes.Get(59);

  // After installing the Internet stack, set AODV as the routing protocol
  InternetStackHelper stack;
  AodvHelper aodv;
//   stack.SetRoutingHelper(aodv);
  stack.Install(nodes);

  // Set up the physical layer attributes
  YansWifiPhyHelper wifiPhy;
  wifiPhy.SetErrorRateModel("ns3::NistErrorRateModel");

  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Set up the MAC layer
  WifiMacHelper wifiMac;
  wifiMac.SetType("ns3::AdhocWifiMac"/*, "Ssid", SsidValue(Ssid("ns-3-ssid"))*/); // For an ad-hoc network

  // Set up the WiFi helper
  WifiHelper wifi;
  wifi.SetStandard (WIFI_STANDARD_80211b);

  // Install the WiFi device on the nodes
//   NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);

  // Establishing p2p link between border nodes in AS
  PointToPointHelper p2p;

  // Divide nodes into 6 ASes
  NodeContainer as1, as2, as3, as4, as5, as6;

  for (uint32_t i = 0; i < 10; ++i) {
      as1.Add(nodes.Get(i)); // Add nodes 0 to 9 to AS1
  }
  for (uint32_t i = 10; i < 20; ++i) {
      as2.Add(nodes.Get(i)); // Add nodes 10 to 19 to AS2
  }
  for (uint32_t i = 20; i < 30; ++i) {
      as3.Add(nodes.Get(i)); // Add nodes 20 to 29 to AS3
  }
  for (uint32_t i = 30; i < 40; ++i) {
      as4.Add(nodes.Get(i)); // Add nodes 30 to 39 to AS4
  }
  for (uint32_t i = 40; i < 50; ++i) {
      as5.Add(nodes.Get(i)); // Add nodes 40 to 49 to AS5
  }
  for (uint32_t i = 50; i < 60; ++i) {
      as6.Add(nodes.Get(i)); // Add nodes 50 to 59 to AS6
  }

  // Designate border routers for each AS
  Ptr<Node> as1Border = as1.Get(0);
  Ptr<Node> as2Border = as2.Get(0);
  Ptr<Node> as3Border = as3.Get(0);
  Ptr<Node> as4Border = as4.Get(0);
  Ptr<Node> as5Border = as5.Get(0);
  Ptr<Node> as6Border = as6.Get(0);

// for debugging purpose
  std::cout << "AS2 Border Node: " << as2Border->GetId() << std::endl;


  // Configure interfaces and assign IP addresses for AS1 (similarly for other ASes)
  Ipv4AddressHelper address1;
  address1.SetBase("10.1.1.0", "255.255.255.0");
//    NetDeviceContainer as1Devices = GetDevicesInRange(devices, 0, 9);
  NetDeviceContainer as1Devices = wifi.Install(wifiPhy, wifiMac, as1);
   Ipv4InterfaceContainer as1Interfaces = address1.Assign(as1Devices);
// address.Assign(as1);

std::cout << "IPv4 Addresses in AS1:" << std::endl;
for (uint32_t i = 0; i < as1Interfaces.GetN(); ++i) {
    Ipv4Address ipv4Address = as1Interfaces.GetAddress(i);
    std::cout << "Device " << i << " - IPv4 Address: " << ipv4Address << std::endl;
}


   Ipv4AddressHelper address2;
  address2.SetBase("10.1.2.0", "255.255.255.0");
//    NetDeviceContainer as2Devices = GetDevicesInRange(devices, 10, 19);
  NetDeviceContainer as2Devices = wifi.Install(wifiPhy, wifiMac, as2);
   Ipv4InterfaceContainer as2Interfaces = address2.Assign(as2Devices);
//  address.Assign(as2);

std::cout << "IPv4 Addresses in AS2:" << std::endl;
for (uint32_t i = 0; i < as2Interfaces.GetN(); ++i) {
    Ipv4Address ipv4Address = as2Interfaces.GetAddress(i);
    std::cout << "Device " << i << " - IPv4 Address: " << ipv4Address << std::endl;
}

   Ipv4AddressHelper address3;
  address3.SetBase("10.1.3.0", "255.255.255.0");
//    NetDeviceContainer as3Devices = GetDevicesInRange(devices, 20, 29);
  NetDeviceContainer as3Devices = wifi.Install(wifiPhy, wifiMac, as3);
   Ipv4InterfaceContainer as3Interfaces = address3.Assign(as3Devices);
//  address.Assign(as3);

std::cout << "IPv4 Addresses in AS3:" << std::endl;
for (uint32_t i = 0; i < as3Interfaces.GetN(); ++i) {
    Ipv4Address ipv4Address = as3Interfaces.GetAddress(i);
    std::cout << "Device " << i << " - IPv4 Address: " << ipv4Address << std::endl;
}
   Ipv4AddressHelper address4;
  address4.SetBase("10.1.4.0", "255.255.255.0");
//    NetDeviceContainer as4Devices = GetDevicesInRange(devices, 30, 39);
  NetDeviceContainer as4Devices = wifi.Install(wifiPhy, wifiMac, as4);
   Ipv4InterfaceContainer as4Interfaces = address4.Assign(as4Devices);
//  address.Assign(as4);

std::cout << "IPv4 Addresses in AS4:" << std::endl;
for (uint32_t i = 0; i < as4Interfaces.GetN(); ++i) {
    Ipv4Address ipv4Address = as4Interfaces.GetAddress(i);
    std::cout << "Device " << i << " - IPv4 Address: " << ipv4Address << std::endl;
}

   Ipv4AddressHelper address5;
  address5.SetBase("10.1.5.0", "255.255.255.0");
//    NetDeviceContainer as5Devices = GetDevicesInRange(devices, 40, 49);
  NetDeviceContainer as5Devices = wifi.Install(wifiPhy, wifiMac, as5);
   Ipv4InterfaceContainer as5Interfaces = address5.Assign(as5Devices);
//  address.Assign(as5);

std::cout << "IPv4 Addresses in AS5:" << std::endl;
for (uint32_t i = 0; i < as5Interfaces.GetN(); ++i) {
    Ipv4Address ipv4Address = as5Interfaces.GetAddress(i);
    std::cout << "Device " << i << " - IPv4 Address: " << ipv4Address << std::endl;
}

   Ipv4AddressHelper address6;
  address6.SetBase("10.1.6.0", "255.255.255.0");
//    NetDeviceContainer as6Devices = GetDevicesInRange(devices, 50, 59);
  NetDeviceContainer as6Devices = wifi.Install(wifiPhy, wifiMac, as6);
   Ipv4InterfaceContainer as6Interfaces = address6.Assign(as6Devices);
//  address.Assign(as6);

std::cout << "IPv4 Addresses in AS6:" << std::endl;
for (uint32_t i = 0; i < as6Interfaces.GetN(); ++i) {
    Ipv4Address ipv4Address = as6Interfaces.GetAddress(i);
    std::cout << "Device " << i << " - IPv4 Address: " << ipv4Address << std::endl;
}


  // Set up links between ASes
  Ipv4AddressHelper AS1AS2linkAddress;
  AS1AS2linkAddress.SetBase("10.1.10.0", "255.255.255.0");
  // AS1 to AS2 link
  NetDeviceContainer devicesAS1AS2 = p2p.Install(as1Border, as2Border);
  Ipv4InterfaceContainer interfacesAS1AS2 = AS1AS2linkAddress.Assign(devicesAS1AS2);

  std::cout << "AS1-AS2 Link Addresses:" << std::endl;
for (uint32_t i = 0; i < interfacesAS1AS2.GetN(); ++i) {
    std::cout << "Device " << i << " - Address: ";
    std::cout << interfacesAS1AS2.GetAddress(i) << std::endl;
}

  Ipv4AddressHelper AS2AS3linkAddress;
  AS2AS3linkAddress.SetBase("10.1.11.0", "255.255.255.0");
  // AS2 to AS3 link
  NetDeviceContainer devicesAS2AS3 = p2p.Install(as2Border, as3Border);
  Ipv4InterfaceContainer interfacesAS2AS3 = AS2AS3linkAddress.Assign(devicesAS2AS3);

  std::cout << "AS2-AS3 Link Addresses:" << std::endl;
for (uint32_t i = 0; i < interfacesAS2AS3.GetN(); ++i) {
    std::cout << "Device " << i << " - Address: ";
    std::cout << interfacesAS2AS3.GetAddress(i) << std::endl;
}

  Ipv4AddressHelper AS3AS4linkAddress;
  AS3AS4linkAddress.SetBase("10.1.12.0", "255.255.255.0");
  // AS3 to AS4 link
  NetDeviceContainer devicesAS3AS4 = p2p.Install(as3Border, as4Border);
  Ipv4InterfaceContainer interfacesAS3AS4 = AS3AS4linkAddress.Assign(devicesAS3AS4);

  std::cout << "AS3-AS4 Link Addresses:" << std::endl;
for (uint32_t i = 0; i < interfacesAS3AS4.GetN(); ++i) {
    std::cout << "Device " << i << " - Address: ";
    std::cout << interfacesAS3AS4.GetAddress(i) << std::endl;
}

  Ipv4AddressHelper AS4AS5linkAddress;
  AS4AS5linkAddress.SetBase("10.1.13.0", "255.255.255.0");
  // AS4 to AS5 link
  NetDeviceContainer devicesAS4AS5 = p2p.Install(as4Border, as5Border);
  Ipv4InterfaceContainer interfacesAS4AS5 = AS4AS5linkAddress.Assign(devicesAS4AS5);

  std::cout << "AS4-AS5 Link Addresses:" << std::endl;
for (uint32_t i = 0; i < interfacesAS4AS5.GetN(); ++i) {
    std::cout << "Device " << i << " - Address: ";
    std::cout << interfacesAS4AS5.GetAddress(i) << std::endl;
}

  Ipv4AddressHelper AS5AS6linkAddress;
  AS5AS6linkAddress.SetBase("10.1.14.0", "255.255.255.0");
  // AS5 to AS6 link
  NetDeviceContainer devicesAS5AS6 = p2p.Install(as5Border, as6Border);
  Ipv4InterfaceContainer interfacesAS5AS6 = AS5AS6linkAddress.Assign(devicesAS5AS6);

  std::cout << "AS5-AS6 Link Addresses:" << std::endl;
for (uint32_t i = 0; i < interfacesAS5AS6.GetN(); ++i) {
    std::cout << "Device " << i << " - Address: ";
    std::cout << interfacesAS5AS6.GetAddress(i) << std::endl;
}


//    Ipv4GlobalRoutingHelper::PopulateRoutingTables();




//defining mobility criteria and installing mobility in the nodes
  MobilityHelper mobilityAS1;
mobilityAS1.SetPositionAllocator("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue(0.0),
                                 "MinY", DoubleValue(0.0),
                                 "DeltaX", DoubleValue(30.0),
                                 "DeltaY", DoubleValue(30.0),
                                 "GridWidth", UintegerValue(5),
                                 "LayoutType", StringValue("RowFirst"));

Ptr<UniformRandomVariable> speedAS1 = CreateObject<UniformRandomVariable> ();
speedAS1->SetAttribute ("Min", DoubleValue (10));
speedAS1->SetAttribute ("Max", DoubleValue (20));
mobilityAS1.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                              "Speed", PointerValue (speedAS1),
                              "Bounds", RectangleValue (Rectangle (-300, 300, -300, 300)));
mobilityAS1.Install (as1); // Install mobility model for AS1 nodes;
/////////////////////////////////////////////////////////////////////////////////////////////////
MobilityHelper mobilityAS2;
mobilityAS2.SetPositionAllocator("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue(0.0),
                                 "MinY", DoubleValue(60.0),  // Offset AS2 vertically
                                 "DeltaX", DoubleValue(30.0),
                                 "DeltaY", DoubleValue(30.0),
                                 "GridWidth", UintegerValue(5),
                                 "LayoutType", StringValue("RowFirst"));

Ptr<UniformRandomVariable> speedAS2 = CreateObject<UniformRandomVariable> ();
speedAS2->SetAttribute ("Min", DoubleValue (10));
speedAS2->SetAttribute ("Max", DoubleValue (20));
mobilityAS2.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                              "Speed", PointerValue (speedAS2),
                              "Bounds", RectangleValue (Rectangle (-300, 300, -300, 300)));
mobilityAS2.Install (as2); // Install mobility model for AS2 nodes
///////////////////////////////////////////////////////////////////////////////////////////////////
MobilityHelper mobilityAS3;
mobilityAS3.SetPositionAllocator("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue(0.0),
                                 "MinY", DoubleValue(120.0),  // Offset AS3 vertically
                                 "DeltaX", DoubleValue(30.0),
                                 "DeltaY", DoubleValue(30.0),
                                 "GridWidth", UintegerValue(5),
                                 "LayoutType", StringValue("RowFirst"));


Ptr<UniformRandomVariable> speedAS3 = CreateObject<UniformRandomVariable> ();
speedAS3->SetAttribute ("Min", DoubleValue (10));
speedAS3->SetAttribute ("Max", DoubleValue (20));
mobilityAS3.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                              "Speed", PointerValue (speedAS3),
                              "Bounds", RectangleValue (Rectangle (-300, 300, -300, 300)));
mobilityAS3.Install (as3); // Install mobility model for AS3 nodes
//////////////////////////////////////////////////////////////////////////////////////////////
MobilityHelper mobilityAS4;
mobilityAS4.SetPositionAllocator("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue(60.0),  // Offset AS4 horizontally
                                 "MinY", DoubleValue(0.0),
                                 "DeltaX", DoubleValue(30.0),
                                 "DeltaY", DoubleValue(30.0),
                                 "GridWidth", UintegerValue(5),
                                 "LayoutType", StringValue("RowFirst"));

Ptr<UniformRandomVariable> speedAS4 = CreateObject<UniformRandomVariable> ();
speedAS4->SetAttribute ("Min", DoubleValue (10));
speedAS4->SetAttribute ("Max", DoubleValue (20));
mobilityAS4.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                              "Speed", PointerValue (speedAS4),
                              "Bounds", RectangleValue (Rectangle (-300, 300, -300, 300)));
mobilityAS4.Install (as4); // Install mobility model for AS4 nodes
//////////////////////////////////////////////////////////////////////////////////////////////
MobilityHelper mobilityAS5;
mobilityAS5.SetPositionAllocator("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue(120.0),  // Offset AS5 horizontally
                                 "MinY", DoubleValue(0.0),
                                 "DeltaX", DoubleValue(30.0),
                                 "DeltaY", DoubleValue(30.0),
                                 "GridWidth", UintegerValue(5),
                                 "LayoutType", StringValue("RowFirst"));

Ptr<UniformRandomVariable> speedAS5 = CreateObject<UniformRandomVariable> ();
speedAS5->SetAttribute ("Min", DoubleValue (10));
speedAS5->SetAttribute ("Max", DoubleValue (20));
mobilityAS5.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                              "Speed", PointerValue (speedAS5),
                              "Bounds", RectangleValue (Rectangle (-300, 300, -300, 300)));
mobilityAS5.Install (as5); // Install mobility model for AS5 nodes
///////////////////////////////////////////////////////////////////////////////////////////////
MobilityHelper mobilityAS6;
mobilityAS6.SetPositionAllocator("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue(180.0),  // Offset AS6 horizontally
                                 "MinY", DoubleValue(0.0),
                                 "DeltaX", DoubleValue(30.0),
                                 "DeltaY", DoubleValue(30.0),
                                 "GridWidth", UintegerValue(5),
                                 "LayoutType", StringValue("RowFirst"));

Ptr<UniformRandomVariable> speedAS6 = CreateObject<UniformRandomVariable> ();
speedAS1->SetAttribute ("Min", DoubleValue (10));
speedAS1->SetAttribute ("Max", DoubleValue (20));
mobilityAS6.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                              "Speed", PointerValue (speedAS6),
                              "Bounds", RectangleValue (Rectangle (-300, 300, -300, 300)));
mobilityAS6.Install (as6); // Install mobility model for AS6 nodes
////////////////////////////////////////////////////////////////////////////////////////////////


// Populate routing tables for each AS
for (int i = 0; i < 6; ++i) {
    stack.SetRoutingHelper(aodv); // Install AODV on this AS
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
}

// Application setup (OnOff application between AS6 and AS1 nodes)
  uint16_t port = 9;
  OnOffHelper onoff("ns3::UdpSocketFactory", InetSocketAddress(as1Interfaces.GetAddress(0), port));
  onoff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
  onoff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

  // Install OnOff applications on specific nodes in AS6
  ApplicationContainer apps = onoff.Install(nodes.Get(52)); // Installing on AS6 node
  apps.Start(Seconds(1.0));
  apps.Stop(Seconds(3.0));

  // PacketSink application setup in AS1
  PacketSinkHelper sinkHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port));
  // Install PacketSink on a node in AS1
  apps = sinkHelper.Install(nodes.Get(5)); // Installing on AS1 node
  apps.Start(Seconds(0.0));
  apps.Stop(Seconds(3.0));



//generating xml file to feed the netanim and get visual simulation
  AnimationInterface anim ("test_4.xml"); // Create the animation object and configure output file name

  Simulator::Stop(Seconds(10.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
