import datetime
import os
import json
Import("env")

FILENAME_BUILDNO = 'versioning'
FILENAME_VERSION_H = 'include/version.h'
FILENAME_VERSION_JSON = 'data/settings/version.json'

VERSION = "1.1."

tm = datetime.datetime.today()
date = str(tm.year)[-4:]+'-'+('0'+str(tm.month))[-2:]+'-'+('0'+str(tm.day))[-2:]

def get_and_update_build_number(increment_firmware_build_no, increment_fs_build_no):
    firmware_build_no = increment_firmware_build_no
    fs_build_no = increment_fs_build_no
    try:
        with open(FILENAME_BUILDNO, 'r') as f:
            cnt = f.readlines()
            firmware_build_no = int(cnt[0].strip()) + increment_firmware_build_no
            fs_build_no = int(cnt[1].strip()) + increment_fs_build_no
    except FileNotFoundError:
        print('Starting build numbers from 1..')

    with open(FILENAME_BUILDNO, 'w') as f:
        f.write(str(firmware_build_no) + '\n')
        f.write(str(fs_build_no) + '\n')
    return firmware_build_no, fs_build_no

def before_build(source, target, env):  
  firmware_build_no, fs_build_no = get_and_update_build_number(1, 0)

  hf = """
  #pragma once
  #define VERSION "{}"
  extern const char* version;
  #define COMPILE_DATE "{}"
  extern const char* compile_date;
  """.format(VERSION + str(firmware_build_no), date)
  
  with open(FILENAME_VERSION_H, 'w+') as f:
      f.write(hf)
      

def before_fsbuild(source, target, env):
    print("Before filesystem build")
    firmware_build_no, fs_build_no = get_and_update_build_number(0, 1)
    jf = """
    {{
        "version": "{}",
        "date": "{}"
    }}
    """.format(VERSION+str(fs_build_no), date)
    with open(FILENAME_VERSION_JSON, 'w+') as f:
        f.write(jf)


def after_build(source, target, env):
    print("After build")
    firmware_build_no, fs_build_no = get_and_update_build_number(0, 0)
    builddir = env.subst("$BUILD_DIR")
    os.rename(os.path.join(builddir, "firmware.bin"), os.path.join(builddir, "firmware_v" + VERSION + str(firmware_build_no) + ".bin"))

def after_fsbuild(source, target, env):
    print("After filesystem build")
    firmware_build_no, fs_build_no = get_and_update_build_number(0, 0)
    builddir = env.subst("$BUILD_DIR")
    os.rename(os.path.join(builddir, "littlefs.bin"), os.path.join(builddir, "filesystem_v" + VERSION +  str(fs_build_no) + ".bin"))

env.AddPreAction("$BUILD_DIR/src/version.cpp.o", before_build)
env.AddPreAction("$BUILD_DIR/${ESP32_FS_IMAGE_NAME}.bin", before_fsbuild)

env.AddPostAction("buildprog", after_build)
env.AddPostAction("$BUILD_DIR/${ESP32_FS_IMAGE_NAME}.bin", after_fsbuild)