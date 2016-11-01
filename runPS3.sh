#!/bin/bash

export LD_LIBRARY_PATH=/opt/od4/lib

# Launching odsupercontroller
gnome-terminal --window-with-profile=HANG_IN_THERE_BUDDY -e "/opt/od4/bin/odsupercomponent --cid=10 --verbose=1 --freq=100 --configuration=/opt/od4/configuration"

sleep 1s

# Launching the PS3 controller front-end
cd $HOME/se-research/opendlv.core/build/system-prefix/src/system-build/ps3controller
gnome-terminal --window-with-profile=HANG_IN_THERE_BUDDY -e "./opendlv-core-system-ps3controller --cid=10 --verbose=1"

# Launching the PS3 controller back-end
cd $HOME/se-research/opendlv.core/build/system-prefix/src/system-build/proxy-fh16
gnome-terminal --window-with-profile=HANG_IN_THERE_BUDDY -e "./opendlv-core-system-proxy-fh16 --cid=10 --verbose=1"


