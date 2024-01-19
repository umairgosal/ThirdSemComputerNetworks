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
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/ipv4-flow-classifier.h"
// #include "/home/gosal/Downloads/softwares/ns-allinone-3.40/ns-3.40/src/flow-monitor/model/flow-monitor.h"


using namespace ns3;

int main (int argc, char *argv[])
{

// Create p2p nodes
NS_LOG_UNCOND("Creating p2p nodes...");
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

InternetStackHelper stack;
Ipv4AddressHelper address;
YansWifiPhyHelper wifiPhy;

for (uint32_t i = 0; i < 5; ++i) {
    NodeContainer wifiApNode = p2pNodes.Get(i);

    wifiStaNodes[i].Create(17);  // Create 5 Wi-Fi STA nodes for each p2p node

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


    AodvHelper aodv;
    Ipv4ListRoutingHelper list;
    list.Add(aodv, 100); // Add AODV routing protocol with a priority of 100
//     Ptr<Node> wifiApNodes = p2pNodes.Get(i);
        stack.SetRoutingHelper(list);
        stack.Install(wifiApNode);
//     InternetStackHelper stack;
//     stack.Install(wifiApNode); // Install the internet stack for WiFi AP nodes
    stack.Install(wifiStaNodes[i]);

    switch (i) {
        case 0:
            address.SetBase("10.1.2.0", "255.255.255.0");
            address.Assign(staDevices);
            address.Assign(apDevices);
            break;
        case 1:
            address.SetBase("10.1.3.0", "255.255.255.0");
            address.Assign(staDevices);
            address.Assign(apDevices);
            break;
        case 2:
            address.SetBase("10.1.4.0", "255.255.255.0");
            address.Assign(staDevices);
            address.Assign(apDevices);
            break;
        case 3:
            address.SetBase("10.1.5.0", "255.255.255.0");
            address.Assign(staDevices);
            address.Assign(apDevices);
            break;
        case 4:
            address.SetBase("10.1.6.0", "255.255.255.0");
            address.Assign(staDevices);
            address.Assign(apDevices);
            break;
    }

    // Connect the Wi-Fi STA nodes to the corresponding p2p node
    // YansWifiPhyHelper wifiPhy;
    wifiPhy.SetChannel(channel.Create());
    WifiMacHelper wifiMac;
    NetDeviceContainer wifiStaApDevices;
    wifiMac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing", BooleanValue(false));
    wifiStaApDevices = wifi.Install(wifiPhy, wifiMac, p2pNodes.Get(i));

    // This adds the Wi-Fi STA devices to the same container as the existing p2p devices
    wifiStaDevices[i].Add(wifiStaApDevices.Get(0));
}

//     Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign(devices01);
    p2pInterfaces = address.Assign(devices02);
    p2pInterfaces = address.Assign(devices03);
    p2pInterfaces = address.Assign(devices04);
    p2pInterfaces = address.Assign(devices12);
    p2pInterfaces = address.Assign(devices23);
    p2pInterfaces = address.Assign(devices34);
    p2pInterfaces = address.Assign(devices41);

    AodvHelper aodv2;
    InternetStackHelper stack2;
    stack2.SetRoutingHelper(aodv2);
    stack2.Install(p2pNodes);


    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    Ptr<OutputStreamWrapper> stream = Create<OutputStreamWrapper> ("routes_1", std::ios::out);
    Ipv4GlobalRoutingHelper::PrintRoutingTableAt(Seconds(1.0), wifiStaNodes[4].Get(2), stream);

    Ptr<OutputStreamWrapper> stream1 = Create<OutputStreamWrapper> ("routes_2", std::ios::out);
    Ipv4GlobalRoutingHelper::PrintRoutingTableAt(Seconds(2.0), p2pNodes.Get(4), stream1);

    Ptr<OutputStreamWrapper> stream2 = Create<OutputStreamWrapper> ("routes_3", std::ios::out);
    Ipv4GlobalRoutingHelper::PrintRoutingTableAt(Seconds(2.0), p2pNodes.Get(1), stream2);


// Set up OnOff application from wifiStaNode[0] to wifiStaNode[1]
uint16_t port = 9; // Choose a port number
OnOffHelper onOff("ns3::UdpSocketFactory", Address());
onOff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
onOff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

AddressValue remoteAddress(InetSocketAddress(wifiStaNodes[1].Get(3)->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal(), port));
onOff.SetAttribute("Remote", remoteAddress);

ApplicationContainer apps = onOff.Install(wifiStaNodes[3].Get(2)); // Install the OnOff application on wifiStaNode[0]
apps.Start(Seconds(1.0)); // Start the application
apps.Stop(Seconds(100.0)); // Stop the application after a specified duration

// Set up flow monitor
    Ptr<FlowMonitor> flowMonitor;
    FlowMonitorHelper flowHelper;
    flowMonitor = flowHelper.InstallAll();


//generating xml file to feed the netanim and get visual simulation
  AnimationInterface anim ("test_7.xml"); // Create the animation object and configure output file name
  Simulator::Stop(Seconds(100.0));
  Simulator::Run ();



flowMonitor->CheckForLostPackets();
double totalDelay = 0.0;
uint64_t totalPackets = 0;
uint64_t lostPackets = 0;
FlowMonitor::FlowStatsContainer stats = flowMonitor->GetFlowStats();

    std::ofstream outfile("statistics_7.txt");

    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowHelper.GetClassifier());

    for (auto it = stats.begin(); it != stats.end(); ++it) {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(it->first);

    totalDelay = it->second.delaySum.GetSeconds();
    totalPackets = it->second.txPackets;
    lostPackets = it->second.lostPackets;
    uint64_t receivedPackets = it->second.rxPackets;

    double averageDelay = totalDelay / totalPackets;
    double pdr = (double)(totalPackets - lostPackets) / totalPackets;

    outfile << "Source Node: " << t.sourceAddress << ", Destination Node: " << t.destinationAddress << std::endl;
    outfile << "Average End-to-End Delay: " << averageDelay << " seconds\n";
    outfile << "total Packets: " << totalPackets << "\n";
    outfile << "lost Packets: " << lostPackets << "\n";
    outfile << "Received Packets: " << receivedPackets << "\n";
    outfile << "Packet Delivery Ratio (PDR): " << pdr * 100 << "%\n\n";
}

outfile.close(); // Close the output file



  Simulator::Destroy ();

  return 0;
}
