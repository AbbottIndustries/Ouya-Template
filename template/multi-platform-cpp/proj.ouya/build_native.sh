APPNAME="HelloCpp"

# options

buildexternalsfromsource=

usage(){
cat << EOF
usage: $0 [options]

Build C/C++ code for $APPNAME using Android NDK

OPTIONS:
-s	Build externals from source
-h	this help
EOF
}

while getopts "sh" OPTION; do
case "$OPTION" in
s)
buildexternalsfromsource=1
;;
h)
usage
exit 0
;;
esac
done

# paths

if [ -z "${NDK_ROOT+aaa}" ];then
echo "please define NDK_ROOT"
exit 1
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
# ... use paths relative to current directory
COCOS2DX_ROOT="$DIR/../../.."
APP_ROOT="$DIR/.."
APP_OUYA_ROOT="$DIR"

echo "NDK_ROOT = $NDK_ROOT"
echo "COCOS2DX_ROOT = $COCOS2DX_ROOT"
echo "APP_ROOT = $APP_ROOT"
echo "APP_OUYA_ROOT = $APP_OUYA_ROOT"

# make sure assets is exist
if [ -d "$APP_OUYA_ROOT"/assets ]; then
    rm -rf "$APP_OUYA_ROOT"/assets
fi

mkdir "$APP_OUYA_ROOT"/assets

# copy resources
for file in "$APP_ROOT"/Resources/*
do
if [ -d "$file" ]; then
    cp -rf "$file" "$APP_OUYA_ROOT"/assets
fi

if [ -f "$file" ]; then
    cp "$file" "$APP_OUYA_ROOT"/assets
fi
done

# run ndk-build
if [[ "$buildexternalsfromsource" ]]; then
    echo "Building external dependencies from source"
    "$NDK_ROOT"/ndk-build -C "$APP_OUYA_ROOT" $* \
        "NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/source"
else
    echo "Using prebuilt externals"
    "$NDK_ROOT"/ndk-build -C "$APP_OUYA_ROOT" $* \
        "NDK_MODULE_PATH=${COCOS2DX_ROOT}:${COCOS2DX_ROOT}/cocos2dx/platform/third_party/android/prebuilt"
fi
