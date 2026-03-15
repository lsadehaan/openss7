#!/usr/bin/env bash
set -euo pipefail

ip netns del ss7-a 2>/dev/null || true
ip netns del ss7-b 2>/dev/null || true

for dev in v-ss7a-0 v-ss7a-1 v-ss7b-0 v-ss7b-1; do
    ip link del "$dev" 2>/dev/null || true
done

echo "Removed ss7-a and ss7-b namespaces."
