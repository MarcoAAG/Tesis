#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/marco/Tesis/ROS/src/rosserial/rosserial_client"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/marco/Tesis/ROS/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/marco/Tesis/ROS/install/lib/python2.7/dist-packages:/home/marco/Tesis/ROS/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/marco/Tesis/ROS/build" \
    "/usr/bin/python2" \
    "/home/marco/Tesis/ROS/src/rosserial/rosserial_client/setup.py" \
    build --build-base "/home/marco/Tesis/ROS/build/rosserial/rosserial_client" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/marco/Tesis/ROS/install" --install-scripts="/home/marco/Tesis/ROS/install/bin"
