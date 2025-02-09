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
   cd build

else 
   echo " + verbose is off"
   echo " + build folder exists"
   cd build
fi
echo " + running"
if [ $IS_VERBOSE == 1 ]; then
   make
   cd ..
   ./build/app 
else
   make >> /dev/null
   cd ..
   ./build/app >> /dev/null
fi

