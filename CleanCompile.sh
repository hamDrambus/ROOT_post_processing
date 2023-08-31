#!/bin/sh

# Necessary input for CMake
BOOSTPATH=${HOME}/Software/boost_1_67_0/

# Absolute path to this script, e.g. /home/user/bin/foo.sh
SCRIPT=$(readlink -f "$0")
# Absolute path this script is in, thus /home/user/bin
SCRIPTPATH=$(dirname "$SCRIPT")
PROJECTNAME=$(basename "$SCRIPTPATH")
BUILDTYPE=Debug
#BUILDTYPE=RelWithDebInfo

# Clear build directory and cd to it
rm -rf ${SCRIPTPATH}/${BUILDTYPE}
mkdir ${SCRIPTPATH}/${BUILDTYPE}
cd ${SCRIPTPATH}/${BUILDTYPE}
# set -x displays cmake command. Brackets create subshell so that there is no need ot call set +x
(set -x; cmake -DCMAKE_BUILD_TYPE=${BUILDTYPE} -DBOOST_ROOT=${BOOSTPATH} ../)
(set -x; make)

