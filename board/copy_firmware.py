Import("env")
import shutil
import os
import datetime

def copy_firmware(source, target, env):
    build_dir = env.subst("$BUILD_DIR")
    source_path = os.path.join(build_dir, "firmware.bin")
    dest_path = "firmware/LEEF_Firmware.bin"

    if not os.path.exists(source_path):
        print(f"Error: {source_path} does not exist.")
        return

    os.makedirs(os.path.dirname(dest_path), exist_ok=True)

    if os.path.exists(dest_path):
        timestamp = datetime.datetime.now().strftime("%Y%m%d%H%M%S")
        backup_path = f"{dest_path}.{timestamp}.bak"
        try:
            shutil.move(dest_path, backup_path)
            print(f"Existing file backed up as {backup_path}")
        except Exception as e:
            print(f"Error backing up file: {e}")

    try:
        shutil.copy(source_path, dest_path)
        print(f"Successfully copied {source_path} to {dest_path}")
    except Exception as e:
        print(f"Error copying file: {e}")

env.AddPostAction("buildprog", copy_firmware)
env.AddPostAction("upload", copy_firmware)
