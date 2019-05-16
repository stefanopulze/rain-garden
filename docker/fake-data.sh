#!/usr/bin/env bash

DB=garden
curl -i -XPOST 'http://localhost:8086/write?db='$DB --data-binary $'temperature,node=node01 value=31.0\npressure,node=node01 value=1000.02'