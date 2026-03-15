#!/usr/bin/env bash
set -euo pipefail

for cmd in ip sysctl; do
    command -v "$cmd" >/dev/null 2>&1 || {
        echo "missing command: $cmd" >&2
        exit 1
    }
done

create_ns() {
    local ns="$1"
    ip netns add "$ns" 2>/dev/null || true
    ip -n "$ns" link set lo up
}

create_veth_pair() {
    local host_if="$1"
    local ns_if="$2"
    local ns="$3"

    ip link add "$host_if" type veth peer name "$ns_if" 2>/dev/null || true
    ip link set "$ns_if" netns "$ns" 2>/dev/null || true
}

addr_once() {
    local ns="$1"
    local addr="$2"
    local dev="$3"

    ip -n "$ns" addr add "$addr" dev "$dev" 2>/dev/null || true
}

create_ns ss7-a
create_ns ss7-b

create_veth_pair v-ss7a-0 eth0 ss7-a
create_veth_pair v-ss7a-1 eth1 ss7-a
create_veth_pair v-ss7b-0 eth0 ss7-b
create_veth_pair v-ss7b-1 eth1 ss7-b

ip link set v-ss7a-0 up
ip link set v-ss7a-1 up
ip link set v-ss7b-0 up
ip link set v-ss7b-1 up

ip -n ss7-a link set eth0 up
ip -n ss7-a link set eth1 up
ip -n ss7-b link set eth0 up
ip -n ss7-b link set eth1 up

addr_once ss7-a 10.23.0.1/24 eth0
addr_once ss7-a 10.23.1.1/24 eth1
addr_once ss7-b 10.23.0.2/24 eth0
addr_once ss7-b 10.23.1.2/24 eth1

sysctl -w net.ipv4.ip_forward=1 >/dev/null

echo "Namespaces created:"
echo "  ss7-a: eth0=10.23.0.1 eth1=10.23.1.1"
echo "  ss7-b: eth0=10.23.0.2 eth1=10.23.1.2"
echo
echo "Example:"
echo "  ip netns exec ss7-a ping -c 2 10.23.0.2"
echo "  ip netns exec ss7-b ping -c 2 10.23.0.1"
