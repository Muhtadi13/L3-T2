#!/bin/bash

path="tcp-variants-comparison.cc"
name1="TcpHybla"
name2="TcpLp"
flow='7'
bandwidth='3Mbps'
trace="true"
error="1e-6"
./ns3 run "$path --transport_prot=$name1 --tracing=$trace --num_flows=$flow --bandwidth=$bandwidth --prefix_name=$name1 " 
# ./ns3 run "$path --transport_prot=$name2 --tracing=$trace --num_flows=$flow --bandwidth=$bandwidth --prefix_name=$name2"
