#!/bin/bash

# Simulation parameters
num_nodes=(20 40 70 100)
packets_per_second=(100 200 300 400)
node_speeds=(5 10 15 20)

# path
path="manet-routing-compare.cc"
aodv="AODV"
raodv="RAODV"


# CSV header


# Run simulations
output_file="aodv_simulation_results_nodes.csv"
echo "Protocol,Num Nodes,Packets Per Second,Node Speed (m/s),Throughput (kbps),End-to-End Delay (s),Packet Delivery Ratio (%),Packet Drop Ratio (%)" > $output_file
for n in "${num_nodes[@]}"; do
    pps="${packets_per_second[1]}"
    speed="${node_speeds[1]}"
    echo "Running simulation: $n nodes, $pps packets/s, $speed m/s"
    ./ns3 run "$path --nNodes=$n --pps=$pps --nodeSpeed=$speed --dataFile=$output_file --protocol=$aodv "
    ./ns3 run "$path --nNodes=$n --pps=$pps --nodeSpeed=$speed --dataFile=$output_file --protocol=$raodv "
done

output_file="aodv_simulation_results_pps.csv"
echo "Protocol,Num Nodes,Packets Per Second,Node Speed (m/s),Throughput (kbps),End-to-End Delay (s),Packet Delivery Ratio (%),Packet Drop Ratio (%)" > $output_file
for pps in "${packets_per_second[@]}"; do
    n="${num_nodes[1]}"
    speed="${node_speeds[1]}"
    echo "Running simulation: $n nodes, $pps packets/s, $speed m/s"
    ./ns3 run "$path --nNodes=$n --pps=$pps --nodeSpeed=$speed --dataFile=$output_file --protocol=$aodv "
    ./ns3 run "$path --nNodes=$n --pps=$pps --nodeSpeed=$speed --dataFile=$output_file --protocol=$raodv "
done

output_file="aodv_simulation_results_speed.csv"
echo "Protocol,Num Nodes,Packets Per Second,Node Speed (m/s),Throughput (kbps),End-to-End Delay (s),Packet Delivery Ratio (%),Packet Drop Ratio (%)" > $output_file
for speed in "${node_speeds[@]}"; do
    pps="${packets_per_second[1]}"
    n="${num_nodes[1]}"
    echo "Running simulation: $n nodes, $pps packets/s, $speed m/s"
    ./ns3 run "$path --nNodes=$n --pps=$pps --nodeSpeed=$speed --dataFile=$output_file --protocol=$aodv "
    ./ns3 run "$path --nNodes=$n --pps=$pps --nodeSpeed=$speed --dataFile=$output_file --protocol=$raodv "
done