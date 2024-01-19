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
//Create nodes with node class
  NodeContainer nodes;
  nodes.Create (50);

// Define the source and destination nodes
  Ptr<Node> sourceNode = nodes.Get(0);
  Ptr<Node> destinationNode = nodes.Get(49);

//installing network stack on nodes
  InternetStackHelper stack;
  stack.Install(nodes);

// Create point-to-point link attributes
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));


// creating the ip gen class here to make sure the for loop gets the address attributes
  Ipv4AddressHelper address;
  address.SetBase("10.1.1.0", "255.255.255.0");

// Install TCP/IP stack on the nodes
  Ipv4InterfaceContainer interfaces;
  NetDeviceContainer devices;
  for (uint32_t i = 0; i < nodes.GetN(); ++i) {
      for (uint32_t j = 1; j <= 5; ++j) { // Connect each node to the next five nodes
          uint32_t neighborIndex = (i + j) % nodes.GetN(); // Ensures circular connectivity

          if (neighborIndex == i || neighborIndex == 0 || neighborIndex == 49) // Exclude source and destination nodes
              continue;

          NodeContainer nodePair = NodeContainer(nodes.Get(i), nodes.Get(neighborIndex));
          NetDeviceContainer link = pointToPoint.Install(nodePair);

          // Assign IP addresses
          Ipv4InterfaceContainer interface = address.Assign(link);
          devices.Add(link);

          for (uint32_t k = 0; k < interface.GetN(); ++k) {
              interfaces.Add(interface.Get(k));
          }

          // Increment the network address
          address.NewNetwork();
      }
  }

  //assigning IP's to the nodes
//   for (uint32_t i = 0; i < devices.GetN(); ++i) {
//         address.Assign(devices.Get(i));
//     }

// // Set up the physical layer attributes
//   YansWifiPhyHelper wifiPhy;
//   wifiPhy.SetErrorRateModel("ns3::NistErrorRateModel");
//
//   YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
//   wifiPhy.SetChannel (wifiChannel.Create ());
//
// // Set up the MAC layer
//   WifiMacHelper wifiMac;
//   wifiMac.SetType("ns3::AdhocWifiMac", "Ssid", SsidValue(Ssid("ns-3-ssid"))); // For an ad-hoc network
//
// // Set up the WiFi helper
//   WifiHelper wifi;
//   wifi.SetStandard (WIFI_STANDARD_80211b);
//
// // Install the WiFi device on the nodes
//   NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, nodes);

//defining mobility criteria and installing mobility in the nodes
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (30.0),
                                 "DeltaY", DoubleValue (30.0),
                                 "GridWidth", UintegerValue (5),
                                 "LayoutType", StringValue ("RowFirst"));

  Ptr<UniformRandomVariable> speed = CreateObject<UniformRandomVariable> ();
  speed->SetAttribute ("Min", DoubleValue (10));
  speed->SetAttribute ("Max", DoubleValue (20));
  mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                           "Speed", PointerValue (speed), //applies the speed to the nodes as defined above
                           "Bounds", RectangleValue (Rectangle (-500, 500, -500, 500))); //sets the boundary area for the nodes
  mobility.Install (nodes);



//After installing the Internet stack, set AODV as the routing protocol
  AodvHelper aodv;
  // Configure AODV settings
  aodv.Set("EnableHello", BooleanValue(true));
  aodv.Set("HelloInterval", TimeValue(Seconds(1)));
//   aodv.EnableAodvLogs();
  stack.SetRoutingHelper(aodv); // Use AODV protocol for routing



// for (uint32_t i = 0; i < nodes.GetN(); ++i)
// {
//     for (uint32_t j = i + 1; j < std::min(nodes.GetN(), i + 6); ++j) // Each node has 5 interfaces
//     {
//         if ((i == 0 && j == 49) || (i == 49 && j == 0)) // Exclude source and destination nodes
//             continue;
//
//         NodeContainer nodePair = NodeContainer(nodes.Get(i), nodes.Get(j));
//
//         // Create point-to-point link between the nodes
//         NetDeviceContainer devices;
//         devices = pointToPoint.Install(nodePair);
//
//         // Assign IP addresses
//         address.Assign(devices);
//
//         // Increment the network address
//         address.NewNetwork();
//     }
// }

//Set up the application layer
  uint16_t port = 9; // Discard port (RFC 863)
//   OnOffHelper onoff ("ns3::UdpSocketFactory", InetSocketAddress (destinationNode->GetObject<Ipv4>()->GetAddress(1,0).GetLocal(), port));
//   onoff.SetConstantRate (DataRate ("200kb/s"));
//   ApplicationContainer apps = onoff.Install (sourceNode);
//   apps.Start (Seconds (5.0));
//   apps.Stop (Seconds (20.0));
//
//   PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
//   apps = sink.Install (destinationNode);
//   apps.Start (Seconds (8.0));


//   // Create a socket at the source node
//   Ptr<Socket> source = Socket::CreateSocket (sourceNode, UdpSocketFactory::GetTypeId ());
//
// // Create a socket at the destination node
//   Ptr<Socket> sinkSocket = Socket::CreateSocket (destinationNode, UdpSocketFactory::GetTypeId ());
//
//
// // Bind the sockets to the respective nodes
//   source->Bind ();
//   sinkSocket->Bind (InetSocketAddress (Ipv4Address::GetAny (), port));
//
//
// // Connect the source socket to the destination socket
//   source->Connect (InetSocketAddress (destinationNode->GetObject<Ipv4>()->GetAddress (1, 0).GetLocal (), port));
//
// // Create a packet and send it from the source node to the destination node
//   Ptr<Packet> packet = Create<Packet> (1024); // 1024 bytes
//   source->Send (packet);
//
//
//   // Add your network setup and application code here
//   Simulator::Stop(Seconds(20.0));


// Install TCP/IP stack on the nodes
//     Ipv4InterfaceContainer interfaces;
//     interfaces = address.Assign(devices);

    // Set up the routing tables
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // Set up OnOff application on the sender node
    OnOffHelper onoff("ns3::UdpSocketFactory", InetSocketAddress(interfaces.GetAddress(49), port));
    onoff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onoff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    ApplicationContainer apps = onoff.Install(sourceNode);
    apps.Start(Seconds(1.0));
    apps.Stop(Seconds(3.0));

    // Set up PacketSink application on the receiver node
    PacketSinkHelper sinkHelper("ns3::UdpSocketFactory", InetSocketAddress(interfaces.GetAddress(49), port));
    apps = sinkHelper.Install(destinationNode);
    apps.Start(Seconds(0.0));
    apps.Stop(Seconds(3.0));


//generating xml file to feed the netanim and get visual simulation
  AnimationInterface anim ("test_3.xml"); // Create the animation object and configure output file name

  Simulator::Stop(Seconds(10.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
