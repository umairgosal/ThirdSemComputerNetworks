#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h" // For visualization
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-routing-table-entry.h"
#include "ns3/node-container.h"



// Create 50 mobile nodes
NodeContainer mobileNodes;
mobileNodes.Create(50);

// Create 2 base stations
NodeContainer baseStations;
baseStations.Create(2);

// Install mobility models on mobile nodes
MobilityHelper mobility;
mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                              "MinX", DoubleValue(0.0),
                              "MinY", DoubleValue(0.0),
                              "DeltaX", DoubleValue(5.0),
                              "DeltaY", DoubleValue(10.0),
                              "GridWidth", UintegerValue(10),
                              "LayoutType", StringValue("RowFirst"));
mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel",
                          "Bounds", RectangleValue(Rectangle(-50, 50, -50, 50)));
mobility.Install(mobileNodes);

// Keep base stations static
mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
mobility.Install(baseStations);

// Install Wi-Fi devices on all nodes
WifiHelper wifi;
wifi.SetStandard(WIFI_STANDARD_80211n_5GHZ);
// ... (configure Wi-Fi parameters as needed)



YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
// ... (configure propagation delay model parameters)



// Calculate link costs based on distance and propagation delay
for (uint32_t i = 0; i < mobileNodes.GetN(); i++) {
  Ptr<Node> node = mobileNodes.Get(i);
  Ptr<Ipv4StaticRouting> staticRouting = Ipv4RoutingHelper::GetRouting<Ipv4StaticRouting>(node);
  Ipv4RoutingTableEntry::MetricType metric = 0;

  // Calculate metric based on distance and propagation delay
  for (uint32_t j = 0; j < node->GetNDevices(); j++) {
    Ptr<NetDevice> device = node->GetDevice(j);
    Ptr<Channel> channel = device->GetChannel();
    // Access propagation delay and other channel attributes to calculate metric
    // ... (adjust calculation based on your model and requirements)
    metric += calculatedLinkCost; // Add the calculated cost for this link
  }

  // Set default route with calculated metric
  Ipv4Address defaultRoute = Ipv4Address("10.0.0.1"); // Replace with appropriate default gateway address
  staticRouting->SetDefaultRoute(defaultRoute, metric);
}


// Create a mesh topology with 5 interfaces per mobile node
NetDeviceContainer meshDevices = wifi.Install(wifiPhy, wifiMac, mobileNodes);

// Connect mobile nodes to base stations using point-to-point links
PointToPointHelper p2p;
p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
p2p.SetChannelAttribute("Delay", StringValue("2ms"));
NetDeviceContainer p2pDevices = p2p.Install(baseStations.Get(0), mobileNodes);
p2pDevices.Add(p2p.Install(baseStations.Get(1), mobileNodes));



Ipv4ListRoutingHelper listRouting;
listRouting.Add(Ipv4StaticRoutingHelper::Default()); // Use static routing for simplicity
Ipv4GlobalRoutingHelper::PopulateRoutingTables();

// Calculate link costs based on distance and propagation delay
// ... (implement logic to calculate link costs dynamically)

// Set link costs in the routing tables
// ... (update routing tables with calculated link costs)
