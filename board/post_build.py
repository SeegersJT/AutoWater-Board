import shutil
import os

def copy_build_file(source_dir, dest_dir, filename_prefix):
    if not os.path.exists(dest_dir):
        os.makedirs(dest_dir)

    build_files = [f for f in os.listdir(source_dir) if f.endswith('.bin')]
    
    if not build_files:
        print("No build files found.")
        return

    build_file = max(build_files, key=lambda f: os.path.getmtime(os.path.join(source_dir, f)))
    build_file_path = os.path.join(source_dir, build_file)
    
    dest_file = os.path.join(dest_dir, f"{filename_prefix}.bin")

    shutil.copy(build_file_path, dest_file)
    print(f"Build file copied to {dest_file}")

source_directory = ".pio/build/esp32dev"
destination_directory = "firmware"
filename_prefix = "LEEF_Firmware"

# Run the copy function
copy_build_file(source_directory, destination_directory, filename_prefix)
