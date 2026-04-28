#!/usr/bin/env python3
"""
package_firmware.py
Creates a distributable zip containing all compiled firmware files.
Run after 'pio run -e esp32-s3'.
"""

import os
import shutil
import json
from datetime import datetime

# ── Configuration ──
BUILD_DIR = ".pio/build/esp32-s3"
OUTPUT_DIR = "firmware-package"
PROJECT_NAME = "esp32s3-firmware"

def get_firmware_info():
    """Gather metadata about the build."""
    firmware_bin = os.path.join(BUILD_DIR, "firmware.bin")
    firmware_elf = os.path.join(BUILD_DIR, "firmware.elf")
    
    info = {
        "project": PROJECT_NAME,
        "board": "esp32-s3-devkitc-1",
        "framework": "arduino",
        "build_date": datetime.utcnow().isoformat() + "Z",
        "files": {}
    }
    
    if os.path.exists(firmware_bin):
        info["files"]["firmware.bin"] = {
            "size_bytes": os.path.getsize(firmware_bin),
            "description": "Main firmware binary"
        }
    
    if os.path.exists(firmware_elf):
        info["files"]["firmware.elf"] = {
            "size_bytes": os.path.getsize(firmware_elf),
            "description": "ELF file (for debugging)"
        }
    
    # Check for bootloader and partition table
    bootloader = os.path.join(BUILD_DIR, "bootloader.bin")
    partitions = os.path.join(BUILD_DIR, "partitions.bin")
    
    if os.path.exists(bootloader):
        info["files"]["bootloader.bin"] = {
            "size_bytes": os.path.getsize(bootloader),
            "description": "Bootloader binary"
        }
    
    if os.path.exists(partitions):
        info["files"]["partitions.bin"] = {
            "size_bytes": os.path.getsize(partitions),
            "description": "Partition table"
        }
    
    return info

def create_flash_script():
    """Create a helper script to flash the firmware."""
    bat_script = """@echo off
echo ============================================
echo  ESP32-S3 Firmware Flasher
echo ============================================
echo.
echo Make sure your ESP32-S3 is connected via USB.
echo Hold BOOT button while pressing RESET to 
echo enter download mode if needed.
echo.
pause
esptool.py --chip esp32s3 --port COM3 --baud 921600 ^
  --before default_reset --after hard_reset ^
  write_flash -z ^
  --flash_mode dio --flash_freq 80m --flash_size 4MB ^
  0x0000 bootloader.bin ^
  0x8000 partitions.bin ^
  0x10000 firmware.bin
echo.
echo Done! Press any key to exit.
pause
"""
    
    sh_script = """#!/bin/bash
echo "============================================"
echo " ESP32-S3 Firmware Flasher"
echo "============================================"
echo ""
echo "Make sure your ESP32-S3 is connected via USB."
echo ""

PORT=${1:-/dev/ttyACM0}
echo "Using port: $PORT"

esptool.py --chip esp32s3 --port "$PORT" --baud 921600 \
  --before default_reset --after hard_reset \
  write_flash -z \
  --flash_mode dio --flash_freq 80m --flash_size 4MB \
  0x0000 bootloader.bin \
  0x8000 partitions.bin \
  0x10000 firmware.bin

echo ""
echo "Done!"
"""
    return bat_script, sh_script

def main():
    print(f"\n📦 Packaging {PROJECT_NAME}...")
    print(f"   Build dir: {BUILD_DIR}")
    print(f"   Output dir: {OUTPUT_DIR}")
    
    # Clean and create output directory
    if os.path.exists(OUTPUT_DIR):
        shutil.rmtree(OUTPUT_DIR)
    os.makedirs(OUTPUT_DIR)
    
    # Copy firmware files
    files_to_copy = [
        "firmware.bin",
        "firmware.elf",
        "bootloader.bin",
        "partitions.bin",
    ]
    
    copied = 0
    for filename in files_to_copy:
        src = os.path.join(BUILD_DIR, filename)
        if os.path.exists(src):
            shutil.copy2(src, os.path.join(OUTPUT_DIR, filename))
            size_kb = os.path.getsize(src) / 1024
            print(f"   ✓ {filename} ({size_kb:.1f} KB)")
            copied += 1
        else:
            print(f"   ⚠ {filename} not found, skipping")
    
    # Generate build info
    info = get_firmware_info()
    info_path = os.path.join(OUTPUT_DIR, "build-info.json")
    with open(info_path, "w") as f:
        json.dump(info, f, indent=2)
    print(f"   ✓ build-info.json")
    
    # Create flash helper scripts
    bat_script, sh_script = create_flash_script()
    
    with open(os.path.join(OUTPUT_DIR, "flash.bat"), "w") as f:
        f.write(bat_script)
    
    with open(os.path.join(OUTPUT_DIR, "flash.sh"), "w") as f:
        f.write(sh_script)
    
    print(f"   ✓ flash.bat / flash.sh")
    
    # Create the zip
    zip_name = f"{PROJECT_NAME}-{datetime.utcnow().strftime('%Y%m%d-%H%M%S')}"
    shutil.make_archive(
        os.path.join(OUTPUT_DIR, zip_name),
        'zip',
        OUTPUT_DIR
    )
    print(f"\n✅ Package created: {OUTPUT_DIR}/{zip_name}.zip")
    print(f"   Total files: {copied + 3}")

if __name__ == "__main__":
    main()
