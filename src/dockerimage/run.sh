#!/usr/bin/env sh
docker run --network="host" -it --entrypoint zsh $1
