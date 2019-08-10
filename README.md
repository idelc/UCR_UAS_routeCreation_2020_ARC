# UCR_UAS_RouteCreationTools

This repository will contain the programs that fetch, create and upload the route the uav will fly.

These tools will work from the command line and represent routes and waypoints as simple text files.
This will make it simpler to break the tasks up into small, easy-to-test segments.

## Overview

* Route file format: text file containing a full route as used in [MavProxy](https://github.com/ArduPilot/MAVProxy/blob/master/MAVProxy/modules/mavproxy_wp.py). It's not documented in any one place so it is necessary to RE the format from the code.

* Obstacle file format: text file storing the obstacles that must be avoided. Defined in obstacles.md
         
* `combine ./in0.txt ./in1.txt ./out.txt`: Take in two routes and merges them into one valid route.

* `safeRoute ./Obst.txt ./route.txt ./out.txt`: Takes a list of obstacles and a route. It creates from these a route that still hits all waypoints in `route.txt` but no longer collides with any obstical in `Obst.txt`.

* `scanRoute [Options] ./out.txt`: based on `[Options]`, generates a route to scan the ADLC search area. Options would include things such as scan height, scan speed, etc.

* Upload: MavProxy will be used to upload the finished route file to the UAV.
