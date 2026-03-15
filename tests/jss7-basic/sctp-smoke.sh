#!/usr/bin/env bash
set -euo pipefail

for cmd in ip sctp_darn timeout; do
    command -v "$cmd" >/dev/null 2>&1 || {
        echo "missing command: $cmd" >&2
        exit 1
    }
done

cleanup() {
    if [[ -n "${server_pid:-}" ]]; then
        kill "${server_pid}" 2>/dev/null || true
        wait "${server_pid}" 2>/dev/null || true
    fi
}
trap cleanup EXIT

ip netns exec ss7-b sh -c "sctp_darn -H 10.23.0.2 -P 2905 -l >/tmp/jss7-basic-sctp-server.log 2>&1" &
server_pid=$!

sleep 1

timeout 5s ip netns exec ss7-a sctp_darn -H 10.23.0.1 -P 2905 -h 10.23.0.2 -p 2905 -s >/tmp/jss7-basic-sctp-client.log 2>&1 || true

if ip netns exec ss7-a ss -neOap | grep -q sctp; then
    echo "SCTP smoke check passed."
    echo "Client log: /tmp/jss7-basic-sctp-client.log"
    echo "Server log: /tmp/jss7-basic-sctp-server.log"
    exit 0
fi

echo "SCTP smoke check failed." >&2
echo "Client log: /tmp/jss7-basic-sctp-client.log" >&2
echo "Server log: /tmp/jss7-basic-sctp-server.log" >&2
exit 1
