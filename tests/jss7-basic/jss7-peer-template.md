# jSS7 Peer Template

Use this as a parameter sheet for your local jSS7 build or XML/bootstrap file.
The exact element names depend on the jSS7 distribution, but these values are
the ones that matter for the first OpenSS7 interoperability pass.

## Role

- jSS7 side: `ASP`
- OpenSS7 side: `SGP`

## SCTP Association

- Local IP: `10.23.0.2`
- Remote IP: `10.23.0.1`
- Local port: `2905`
- Remote port: `2905`
- Extra local multihoming IP: `10.23.1.2`
- Extra remote multihoming IP: `10.23.1.1`

## M3UA Parameters

- ASP name: `jss7-asp1`
- AS name: `as-101`
- Routing Context: `101`
- Traffic mode: `loadshare`
- Local point code: `2-2-2`
- Remote point code: `1-1-1`
- Network Appearance: unset
- Service Indicator: `SCCP`

## Expected State Flow

1. SCTP association established
2. ASP sends `ASPUP`
3. OpenSS7 replies `ASPUP ACK`
4. ASP sends `ASPAC`
5. OpenSS7 replies `ASPAC ACK`
6. Data transfer begins

## If You Use a Java Bootstrap Instead of XML

Your bootstrap should create:

- one SCTP management instance
- one association from `10.23.0.2:2905` to `10.23.0.1:2905`
- one ASP factory bound to that association
- one AS with routing context `101`
- one route toward point code `1-1-1`

Keep heartbeat enabled. For the first pass, do not enable IPSP mode.
