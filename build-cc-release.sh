NJOBS="${EUPSPKG_NJOBS:-1}"

# Compile, test and install C++ library
meson compile -C build-release -j $NJOBS && meson test -C build-release && meson install -C build-release
