NJOBS="${EUPSPKG_NJOBS:-1}"

# Compile, test and install C++ library
meson compile -C build-release -j $NJOBS && MESON_NUM_PROCESSES=$NJOBS meson test -C build-release && MESON_NUM_PROCESSES=$NJOBS meson install -C build-release
