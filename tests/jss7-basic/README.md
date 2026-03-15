# OpenSS7 <-> jSS7 Basic Interop Lab

This lab creates two isolated Linux network namespaces on one host:

- `ss7-a`: OpenSS7 side
- `ss7-b`: jSS7 side

Use this first for end-to-end SCTP and M3UA bring-up before moving to two real
hosts. It avoids Docker-specific networking surprises while still giving you a
real two-node topology.

## Recommended Roles

For the first pass, keep the roles simple:

- OpenSS7 in `ss7-a` acts as the `SGP/STP` side
- jSS7 in `ss7-b` acts as the `ASP` side

That matches common jSS7 deployments and keeps the peer configuration smaller.

## Why Namespaces Instead of Docker

A network namespace is a separate Linux network stack with its own interfaces,
addresses, routes, and sockets. It behaves like a lightweight node without a
VM.

This is preferred over Docker for initial OpenSS7 validation because:

- OpenSS7 uses the host kernel and modules directly
- SCTP multihoming is easier to reason about
- packet capture and routing diagnostics are simpler
- there is less abstraction between your code and the network path

Docker is acceptable for `jSS7 only` if you want packaging convenience, but not
for the OpenSS7 side.

## Topology

- `ss7-a`
  - `eth0`: `10.23.0.1/24`
  - `eth1`: `10.23.1.1/24`
- `ss7-b`
  - `eth0`: `10.23.0.2/24`
  - `eth1`: `10.23.1.2/24`

Use `eth0` first. `eth1` is there for later SCTP multihoming tests.

Default protocol values for the lab:

- SCTP port: `2905`
- OpenSS7 point code: `1-1-1`
- jSS7 point code: `2-2-2`
- Routing Context: `101`
- Traffic mode: `loadshare`
- Network Appearance: unset

## Host Prerequisites

Install at least:

```bash
sudo apt install iproute2 lksctp-tools tcpdump openjdk-17-jre-headless
```

You also need your built OpenSS7 tree and a runnable jSS7 application.

## Setup

Create the namespaces:

```bash
sudo ./tests/jss7-basic/setup-netns.sh
```

Verify IP connectivity:

```bash
sudo ip netns exec ss7-a ping -c 2 10.23.0.2
sudo ip netns exec ss7-b ping -c 2 10.23.0.1
```

## Step 1: SCTP Smoke Test

Run the built-in smoke helper:

```bash
sudo ./tests/jss7-basic/sctp-smoke.sh
```

This validates that the namespaces and kernel SCTP path are working before you
debug M3UA.

## Step 2: Configure the Peer Roles

### jSS7 Side

Use [jss7-peer-template.md](/root/SIGTRAN/openss7/tests/jss7-basic/jss7-peer-template.md) as the
parameter map for your local jSS7 version. The exact XML or Java bootstrap
format differs across jSS7 distributions, but the important values are:

- local host: `10.23.0.2`
- remote host: `10.23.0.1`
- local port: `2905`
- remote port: `2905`
- local point code: `2-2-2`
- remote point code: `1-1-1`
- routing context: `101`
- mode: `ASP`

Run jSS7 inside `ss7-b`:

```bash
sudo ip netns exec ss7-b java -jar /path/to/jss7-app.jar
```

### OpenSS7 Side

Configure the OpenSS7 endpoint in `ss7-a` as the `SGP/STP` side using the same
SCTP tuple and the opposite point codes:

- local host: `10.23.0.1`
- remote host: `10.23.0.2`
- local point code: `1-1-1`
- remote point code: `2-2-2`
- routing context: `101`
- mode: `SGP`

Use the OpenSS7 tools and config flow you already use for your local build.

## Step 3: Bring-Up Order

1. Start the OpenSS7 side in `ss7-a`
2. Start the jSS7 side in `ss7-b`
3. Confirm SCTP association establishment
4. Confirm `ASPUP` / `ASPUP ACK`
5. Confirm `ASPAC` / `ASPAC ACK`
6. Confirm data transfer

## Useful Diagnostics

Capture traffic on the first path:

```bash
sudo ip netns exec ss7-a tcpdump -ni eth0 -vv proto sctp
sudo ip netns exec ss7-b tcpdump -ni eth0 -vv proto sctp
```

Check sockets:

```bash
sudo ip netns exec ss7-a ss -neOap | grep sctp
sudo ip netns exec ss7-b ss -neOap | grep sctp
```

## Test Matrix

Run these in order:

- `SCTP connect`: association comes up on `10.23.0.1:2905 <-> 10.23.0.2:2905`
- `ASPUP`: jSS7 transitions from down to up
- `ASPAC`: traffic becomes active
- `Heartbeat`: idle association stays healthy
- `jSS7 restart`: OpenSS7 detects loss and accepts reconnect
- `OpenSS7 restart`: jSS7 reconnects cleanly
- `Multihoming`: bind both `10.23.0.x` and `10.23.1.x`
- `Path failover`: administratively drop one path and confirm association survives

## Docker Guidance

If you insist on Docker, only containerize jSS7. The safest pattern is still:

- create `ss7-b` namespace with `setup-netns.sh`
- run the jSS7 container with host networking
- move it into `ss7-b`, or launch it from inside `ss7-b`

Do not use Docker bridge networking for the first OpenSS7 interop pass.

## Cleanup

```bash
sudo ./tests/jss7-basic/cleanup-netns.sh
```
