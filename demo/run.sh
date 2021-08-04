#!/bin/bash

upload_map() {
    [ -z "${INORBIT_API_URL}" ] && echo "Need to set INORBIT_API_URL" && exit 1;
    [ -z "${INORBIT_APP_KEY}" ] && echo "Need to set INORBIT_APP_KEY" && exit 1;
    [ -z "${INORBIT_ROBOT_ID}" ] && echo "Need to set INORBIT_ROBOT_ID" && exit 1;

    echo "Uploading map using InOrbit API"
    curl --location --request POST "${INORBIT_API_URL}/robots/${INORBIT_ROBOT_ID}/maps" \
        --header "x-auth-inorbit-app-key: ${INORBIT_APP_KEY}" \
        --form 'metadata="{\"mapId\":\"map\", \"label\": \"map\", \"resolution\": 0.1, \"x\": 0, \"y\": 0}"' \
        --form 'image=@"./map.png"'
}

build() {
    echo "Building InOrbit SDK demo"
    g++ demo.cpp -o demo.out
}

run() {
    echo "Running InOrbit SDK demo"
    ./demo.out
}

upload_map
build
run
