# We want to build support for all ARM ABIs
# This can be overriden by build/tools/rebuild-stlport.sh though
APP_STL := system
APP_ABI := armeabi armeabi-v7a
APP_PLATFORM := android-8
APP_CFLAGS := -fvisibility=hidden -ffunction-sections -fdata-sections -Os 
APP_CPPFLAGS := -fvisibility=hidden -ffunction-sections -fdata-sections -Os
APP_LDFLAGS := -Wl,--gc-sections -llog -lgcc -ldl -lm

APP_CFLAGS += -Werror=return-type  -Werror=format -Werror=nonnull -Werror=implicit -Werror=missing-include-dirs -Werror=uninitialized  -Werror=array-bounds  -Werror=missing-parameter-type -Wno-error=maybe-uninitialized

APP_CPPFLAGS += -Werror=return-type  -Werror=format -Werror=nonnull -Werror=implicit -Werror=missing-include-dirs -Werror=uninitialized  -Werror=array-bounds -Werror=missing-parameter-type -Wno-error=maybe-uninitialized

# -Werror=missing-field-initializers  -Werror=write-strings  -Werror=parentheses  -Werror=switch-default -Werror=maybe-uninitialized -Werror=free-nonheap-object

APP_PIE := false
#NDK_TOOLCHAIN_VERSION=4.8
#NDK_TOOLCHAIN_VERSION=clang
APP_OPTIOM := release
APP_SHORT_COMMANDS := true
