BUILDDIR=build
IS_VERBOSE=0

while getopts "vc" opt; do
 case $opt in
 v)
   echo " + verbose is on"
   IS_VERBOSE=1
   ;;
 esac
 case $opt in
    c)
       echo "cleaning..."
       rm -rf build
       exit
       ;;
 esac
done

if [ ! -d "$BUILDDIR" ]; then
   echo " + creating build folder..."
   mkdir -p build
   cd build
   if [ $IS_VERBOSE == 1 ]; then
      cmake ..
   else 
      echo " + verbose is off"
      cmake .. >> /dev/null
   fi

else 
   echo " + build folder exists"
   cd build
fi
if [ $IS_VERBOSE == 1 ]; then
   echo " + compilation..."
   make
   cd ..
   echo " + running"
   ./build/app 
else
   echo " + compilation..."
   make >> /dev/null
   cd ..
   echo " + running"
   ./build/app >> /dev/null
fi

