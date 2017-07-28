 #             __________               __   ___.
#   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
#   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
#   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
#   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
#                     \/            \/     \/    \/            \/
# $Id$
#

PACKAGE=com.gaana
PACKAGE_PATH=com/gaana
BINLIB_DIR=$(BUILDDIR)/libs/$(ANDROID_ARCH)
ANDROID_DIR=$(ROOTDIR)/android

CUSTOMJNI_SRC=$(ROOTDIR)/firmware/target/hosted/android/jni
CUSTOMJNI_PATH=$(ROOTDIR)/firmware/target/hosted/android/jni/power-manager.c
CUSTOMJNIOBJS_PATH=$(BUILDDIR)/firmware/target/hosted/android/jni
CUSTOMJNIOBJS = $(CUSTOMJNIOBJS_PATH)/power-manager.o

# this is a glibc compatibility hack to provide a get_nprocs() replacement.
# The NDK ships cpu-features.c which has a compatible function android_getCpuCount()
CPUFEAT = $(ANDROID_NDK_PATH)/sources/android/cpufeatures
CPUFEAT_BUILD = $(BUILDDIR)
INCLUDES += -I$(CPUFEAT)
OTHER_SRC += $(CPUFEAT)/cpu-features.c
CLEANOBJS += $(CPUFEAT_BUILD)/cpu-features.o
$(CPUFEAT_BUILD)/cpu-features.o: $(CPUFEAT)/cpu-features.c
	$(SILENT)mkdir -p $(dir $@)
	$(call PRINTS,CC $(subst $(CPUFEAT)/,,$<))$(CC) -o $@ -c $^ $(GCCOPTS) -Wno-unused
$(CUSTOMJNIOBJS_PATH)/%.o: $(CUSTOMJNI_SRC)/%.c
	$(SILENT)mkdir -p $(dir $@)
	$(call PRINTS,CC $(subst $(CUSTOMJNI_SRC)/,,$<))$(CC) -o $@ -c $^ $(INCLUDES) $(GCCOPTS) -Wno-unused

.SECONDEXPANSION: # $$(JAVA_OBJ) is not populated until after this
.SECONDEXPANSION: # $$(OBJ) is not populated until after this
.PHONY: apk classes clean dex dirs libs jar

# API version
ANDROID_PLATFORM_VERSION=23
ANDROID_PLATFORM=$(ANDROID_SDK_PATH)/platforms/android-$(ANDROID_PLATFORM_VERSION)

# android tools
BUILD_TOOLS_VERSION=19.0.1
AIDL=$(ANDROID_SDK_PATH)/build-tools/$(BUILD_TOOLS_VERSION)/aidl
AAPT=$(ANDROID_SDK_PATH)/build-tools/$(BUILD_TOOLS_VERSION)/aapt
DX=$(ANDROID_SDK_PATH)/build-tools/$(BUILD_TOOLS_VERSION)/dx
ZIPALIGN=$(ANDROID_SDK_PATH)/build-tools/$(BUILD_TOOLS_VERSION)/zipalign
KEYSTORE=$(HOME)/.android/rockbox.keystore
ADB=$(ANDROID_SDK_PATH)/platform-tools/adb
BUILDAPK=$(ANDROID_DIR)/buildapk.sh

CLASSPATH   := $(BUILDDIR)/bin/classes

MANIFEST	:= $(BUILDDIR)/bin/AndroidManifest.xml
MANIFEST_SRC	:= $(ANDROID_DIR)/AndroidManifest.xml

R_JAVA		:= $(BUILDDIR)/gen/$(PACKAGE_PATH)/R.java
R_JAVA		+= $(BUILDDIR)/gen/com.android.internal.telephony/ITelephony.java
R_OBJ		:= $(CLASSPATH)/$(PACKAGE_PATH)/R.class

JAVA_SRC	:= $(wildcard $(ANDROID_DIR)/src/$(PACKAGE_PATH)/*.java)
JAVA_SRC	+= $(wildcard $(ANDROID_DIR)/src/$(PACKAGE_PATH)/Helper/*.java)
JAVA_SRC	+= $(wildcard $(ANDROID_DIR)/src/$(PACKAGE_PATH)/widgets/*.java)
JAVA_SRC	+= $(wildcard $(ANDROID_DIR)/src/$(PACKAGE_PATH)/monitors/*.java)
JAVA_SRC	+= $(wildcard $(ANDROID_DIR)/src/$(PACKAGE_PATH)/preference/*.java)
JAVA_SRC	+= $(wildcard $(ANDROID_DIR)/src/$(PACKAGE_PATH)/jni/*.java)

AIDL_SRC	:= $(wildcard $(ANDROID_DIR)/src)

java2class = $(addsuffix .class,$(basename $(subst $(ANDROID_DIR)/src,$(CLASSPATH),$(1))))

JAVA_OBJ	:= $(call java2class,$(JAVA_SRC))


LIBS		:= $(BINLIB_DIR)/$(BINARY) $(BINLIB_DIR)/libmisc.so
LIBS 		+= $(addprefix $(BINLIB_DIR)/lib,$(patsubst %.codec,%.so,$(notdir $(CODECS))))

TEMP_APK	:= $(BUILDDIR)/bin/_rockbox.apk
TEMP_APK2	:= $(BUILDDIR)/bin/__rockbox.apk
DEX			:= $(BUILDDIR)/bin/classes.dex
JAR			:= $(BUILDDIR)/bin/classes.jar
AP_			:= $(BUILDDIR)/bin/resources.ap_
APK			:= $(BUILDDIR)/rockbox.apk

_DIRS		:= $(BUILDDIR)/___/$(PACKAGE_PATH)
DIRS		+= $(subst ___,gen,$(_DIRS))
DIRS		+= $(subst ___,data,$(_DIRS))
DIRS		+= $(BUILDDIR)/libs/$(ANDROID_ARCH)
DIRS		+= $(CPUFEAT_BUILD)
DIRS		+= $(CLASSPATH)

RES		:= $(wildcard $(ANDROID_DIR)/res/*/*)

#UMENGSDK
UMENGSDKPATH		:= $(ROOTDIR)/android/UMENG_SDK
UMENGSDKFILE		:= $(UMENGSDKPATH)/umeng_sdk.jar

CLEANOBJS += bin gen libs data

JAVAC_OPTS += -source 1.6 -target 1.6 -implicit:none -classpath $(ANDROID_PLATFORM)/android.jar:$(UMENGSDKFILE):$(UMENGSDKPATH)/annotations.jar:$(CLASSPATH)

.PHONY:
$(MANIFEST): $(MANIFEST_SRC) $(DIRS)
	$(call PRINTS,MANIFEST $(@F))sed -e 's/versionName="1.0"/versionName="$(SVNVERSION)"/;s/screenOrientation="portrait"/screenOrientation="$(LCDORIENTATION)"/' $(MANIFEST_SRC) > $(MANIFEST)

$(R_JAVA) $(AP_): $(MANIFEST) $(RES) | $(DIRS)
	$(call PRINTS,AIDL ITelephony.aidl)$(AIDL) \
		-p$(ANDROID_PLATFORM)/framework.aidl \
		-I$(AIDL_SRC) \
		$(AIDL_SRC)/com/android/internal/telephony/ITelephony.aidl \
		$(BUILDDIR)/gen/com.android.internal.telephony/ITelephony.java
	$(call PRINTS,AAPT resources.ap_)$(AAPT) package -f -m \
		-J $(BUILDDIR)/gen -M $(MANIFEST) -S $(ANDROID_DIR)/res \
		-I $(ANDROID_PLATFORM)/android.jar -F $(AP_) \
		-I $(UMENGSDKFILE) \
		-I $(UMENGSDKPATH)/annotations.jar #注释@override

$(CLASSPATH)/$(PACKAGE_PATH)/R.class: $(R_JAVA)
	$(call PRINTS,JAVAC $(subst $(ROOTDIR)/,,$<))javac -d $(BUILDDIR)/bin \
		$(JAVAC_OPTS) -sourcepath $(ANDROID_DIR)/gen $<

$(CLASSPATH)/$(PACKAGE_PATH)/%.class: $(ANDROID_DIR)/src/$(PACKAGE_PATH)/%.java $(CLASSPATH)/$(PACKAGE_PATH)/R.class
	$(call PRINTS,JAVAC $(subst $(ROOTDIR)/,,$<))javac -d $(CLASSPATH) \
		$(JAVAC_OPTS) -sourcepath $(ANDROID_DIR)/src $<

$(JAR): $(JAVA_SRC) $(R_JAVA)
	$(call PRINTS,JAVAC $(subst $(ROOTDIR)/,,$?))javac -d $(CLASSPATH) \
		$(JAVAC_OPTS) -sourcepath $(ANDROID_DIR)/src:$(ANDROID_DIR)/gen $?
	$(call PRINTS,JAR $(subst $(BUILDDIR)/,,$@))jar cf $(JAR) -C $(CLASSPATH) com
#代码混淆用	$(SILENT)$(ANDROID_SDK_PATH)/tools/proguard/bin/proguard.sh -injars $(TMPJAR) -outjars $(JAR) -libraryjars $(ANDROID_PLATFORM)/android.jar -libraryjars $(UMENGSDKFILE) @$(ANDROID_DIR)/proguard.cfg

jar: $(JAR)

$(DEX): $(JAR)
	@echo "Checking for deleted class files" && $(foreach obj,$(JAVA_OBJ) $(R_OBJ), \
		(test -f $(obj) || (echo "$(obj) is missing. Run 'make classes' to fix." && false)) && ) true
	$(call PRINTS,DX $(subst $(BUILDDIR)/,,$@))$(DX) --dex --no-optimize --output=$@ $(UMENGSDKFILE) $(UMENGSDKPATH)/annotations.jar $<

dex: $(DEX)

classes: $(R_OBJ) $(JAVA_OBJ)


$(BUILDDIR)/$(BINARY): $$(OBJ) $(FIRMLIB) $(VOICESPEEXLIB) $(CORE_LIBS) $(CPUFEAT_BUILD)/cpu-features.o \
		$(CUSTOMJNIOBJS)
	$(call PRINTS,LD $(BINARY))$(CC) -o $@ $^ $(LDOPTS) $(GLOBAL_LDOPTS) -Wl,-Map,$(BUILDDIR)/rockbox.map
	$(call PRINTS,OC $(@F))$(call objcopy,$@,$@)

$(BINLIB_DIR)/$(BINARY): $(BUILDDIR)/$(BINARY)
	$(call PRINTS,CP $(BINARY))cp $^ $@

$(BINLIB_DIR)/libmisc.so: $(BUILDDIR)/rockbox.zip
	$(call PRINTS,CP rockbox.zip)cp $^ $@

$(BINLIB_DIR)/lib%.so: $(RBCODEC_BLD)/codecs/%.codec
	$(call PRINTS,CP $(@F))cp $^ $@

libs: $(DIRS) $(LIBS)

$(TEMP_APK): $(AP_) $(LIBS) $(DEX) | $(DIRS)
	$(call PRINTS,APK $(subst $(BUILDDIR)/,,$@))$(BUILDAPK) $(BUILDDIR) $(notdir $@) $(BUILD_TOOLS_VERSION)

$(KEYSTORE):
	$(SILENT)mkdir -p $(HOME)/.android
	$(call PRINTS,KEYTOOL rockbox.keystore)keytool -genkey \
		-alias rockboxkey -keystore $@ \
		-storepass rbtheme.5d6d.net -keypass rbtheme.5d6d.net -validity 40000 \
		-sigalg MD5withRSA -keyalg RSA -keysize 1024 \
		-dname "CN=zhkailing,O=zhkailing-PC,C=CN"

ifdef NODEPS
$(APK): $(TEMP_APK) $(KEYSTORE)
else
$(APK): $(TEMP_APK) $(BUILDDIR)/rockbox.zip $(KEYSTORE)
endif
	$(SILENT)rm -f $@
	$(SILENT)echo "SIGN rockbox-rcc-`git rev-parse --verify --short HEAD`-$(LCD_WIDTH)x$(LCD_HEIGHT).apk"
	$(SILENT)jarsigner \
		-keystore "$(KEYSTORE)" -storepass "rbtheme.5d6d.net" -keypass "rbtheme.5d6d.net" \
		-signedjar $(TEMP_APK2) $(TEMP_APK) "rockboxkey" \
		-sigalg MD5withRSA -digestalg SHA1
	$(SILENT)echo "ZIPALIGN rockbox-rcc-`git rev-parse --verify --short HEAD`-$(LCD_WIDTH)x$(LCD_HEIGHT).apk"
	$(SILENT)$(ZIPALIGN) -v 4 $(TEMP_APK2) $@ > /dev/null

$(DIRS):
	$(SILENT)mkdir -p $@

dirs: $(DIRS)

apk: $(APK)
	$(SILENT)mv ./rockbox.apk ./rockbox-rcc-`git rev-parse --verify --short HEAD`\
	-$(LCD_WIDTH)x$(LCD_HEIGHT).apk


install: apk
	$(SILENT)$(ADB) install -r rockbox-rcc-`git rev-parse --verify --short HEAD`\
	-$(LCD_WIDTH)x$(LCD_HEIGHT).apk

launch:
	$(SILENT)echo 'am start -a android.intent.action.MAIN -n com.gaana/.RockboxActivity; exit' | $(ADB) shell >/dev/null
