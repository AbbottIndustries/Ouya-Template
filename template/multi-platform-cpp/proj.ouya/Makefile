all:
	./build_native.sh
	ant -Dsdk.dir=/Users/medwards/Development/Android/adt-bundle-mac-x86_64-20130917/sdk debug

run:
	adb -d uninstall org.cocos2dx.hellocpp
	adb -d install bin/HelloCpp-debug.apk
	adb -d logcat | grep cocos2d

clean:
	rm -rf libs/
	rm -rf obj/
	rm -rf bin/*
	rm -rf gen/
	rm -rf assets/*

ccclean:
	rm -rf ../../cocos2dx/platform/ouya/java/bin/*
	rm -rf ../../cocos2dx/platform/ouya/java/gen/

tests:
	./build_native.sh -t
	ant -Dsdk.dir=/Users/medwards/Development/Android/adt-bundle-mac-x86_64-20130917/sdk debug
