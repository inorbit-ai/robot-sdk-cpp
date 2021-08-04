# C++ Robot SDK demo

This is a synthetic demo scenario to illustrate how the C++ SDK can be used
to connect a robot to InOrbit.

## How to run

The following parameters are required to run the demo:

```bash
export INORBIT_API_URL="http://api.inorbit.ai"
export INORBIT_APP_KEY="abcd1234abcd1234"
export INORBIT_ROBOT_ID="123456789"
```

Then execute `run.sh` to upload `map.png`, build and execute the demo scenario. On a different terminal, create the `/tmp/start` file in order to trigger the demo scenario:

```bash
touch /tmp/start
```
