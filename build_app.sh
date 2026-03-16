rm -rf build
cmake -S . -B build -DARM_NONE_EABI_TOOLCHAIN_PATH=/usr -DNRF5_SDK_PATH=/home/finneym/Documents/code/Pinetime/nRF5_SDK_15.3.0_59ac345 -DCMAKE_BUILD_TYPE=Release -DBUILD_DFU=1 -DBUILD_RESOURCES=1 -DTARGET_DEVICE=PINETIME
cd build
make pinetime-mcuboot-app -j4
