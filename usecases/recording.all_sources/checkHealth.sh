#!/bin/bash
#
# Copyright (C) 2016 Christian Berger
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
# USA.

# System configuration:
LOCALHOST=127.0.0.1
MULTICAST_ROUTE=225.0.0.0
DOCKER_VOLUME_PREFIX=/from.host
DEV_VIDEO0=${DOCKER_VOLUME_PREFIX}/dev/video0
DEV_VIDEO1=${DOCKER_VOLUME_PREFIX}/dev/video1
DEV_VIDEO2=${DOCKER_VOLUME_PREFIX}/dev/video2
DEV_VIDEO3=${DOCKER_VOLUME_PREFIX}/dev/video3
DEV_USB0=${DOCKER_VOLUME_PREFIX}/dev/ttyUSB0
# Reachability tests:
HAS_MULTICAST_ROUTE=$(ip route | grep "$MULTICAST_ROUTE" 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
PING_LOCALHOST=$(ping -W1 -c1 $LOCALHOST 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")

# Devnode tests:
HAS_DEV_VIDEO0=$(test -e $DEV_VIDEO0 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
HAS_DEV_VIDEO1=$(test -e $DEV_VIDEO1 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
HAS_DEV_VIDEO2=$(test -e $DEV_VIDEO2 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
HAS_DEV_VIDEO3=$(test -e $DEV_VIDEO3 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")
HAS_DEV_USB0=$(test -e $DEV_USB0 2>&1 >/dev/null && echo "PASSED" || echo "FAILED")

# System:
KERNEL=$(cat /proc/version | cut -f3 -d" ")
LOADAVERAGE=$(cat /proc/loadavg | cut -f1-3 -d" "|tr -s " " "/")
UPTIME=$(echo "`cat /proc/uptime |cut -f1 -d" "`/60"|bc)

# Disk space:
DISKSPACE_USED=$(df -h | grep "/$"|tr -s " " " "|cut -f3 -d" ")
DISKSPACE_LEFT=$(df -h | grep "/$"|tr -s " " " "|cut -f4 -d" ")

echo "health.host=$(hostname)"
echo "$(hostname).system.kernel=$KERNEL"
echo "$(hostname).system.loadaverage=$LOADAVERAGE"
echo "$(hostname).system.uptime=${UPTIME}min"
echo "$(hostname).has.${DEV_VIDEO0}=$HAS_DEV_VIDEO0"
echo "$(hostname).has.multicast_route=$HAS_MULTICAST_ROUTE"
echo "$(hostname).ping.localhost=$PING_LOCALHOST"
echo "$(hostname).diskspace.used=$DISKSPACE_USED"
echo "$(hostname).diskspace.available=$DISKSPACE_LEFT"

