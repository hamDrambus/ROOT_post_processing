#!/bin/sh

# Absolute path to this script, e.g. /home/user/bin/foo.sh
SCRIPT=$(readlink -f "$0")
# Absolute path this script is in, thus /home/user/bin
SCRIPTPATH=$(dirname "$SCRIPT")
PROJECTNAME=$(basename "$SCRIPTPATH")
#BUILDTYPE=Debug
BUILDTYPE=RelWithDebInfo

# Clear build directory and cd to it
cd ${SCRIPTPATH}/${BUILDTYPE}
# set -x displays cmake command. Brackets create subshell so that there is no need ot call set +x
(set -x; make)

