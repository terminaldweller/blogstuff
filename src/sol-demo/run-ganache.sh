#!/bin/bash
cd $(dirname $0)

function check_node_version() {
  MIN_VERSION="7.6.0"
  if [[ "$MIN_VERSION" == "$1" ]]; then return 0;fi
  local IFS=.
  VERSION="${1:1}"
  local i ver1=($MIN_VERSION) ver2=($VERSION) 
  for ((i=0; i < ${#ver1[@]}; i++)); do
    if [[ 10#${ver1[i]} > 10#${ver2[i]} ]]; then return 1;fi
    if [[ 10#${ver1[i]} < 10#${ver2[i]} ]]; then return 0;fi
  done
}

NODE_VERSION=$(node --version)
check_node_version $NODE_VERSION
if [[ $? == 1 ]]; then echo "node version is too old. please use v.7.6.0 or newer." && exit 1; fi
echo "node version is new enough!"

ganache-cli -p 22222 -d /dev/null 2>&1 &
G_PID=$!
echo "ganache-cli pid "$G_PID
sleep 4

"solc" --overwrite --abi --optimize --bin --ast --asm ./contracts/*.sol -o ./bin

"node" test.js
"kill" -9 $G_PID
