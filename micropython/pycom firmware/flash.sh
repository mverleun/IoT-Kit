


esptool.py --port /dev/cu.*serial* --baud 115200 --after no_reset write_flash --flash_mode dio --flash_freq 80m --flash_size detect 0x1000 bootloader.bin
esptool.py --port /dev/cu.*serial* --baud 115200 --after no_reset write_flash --flash_mode dio --flash_freq 80m --flash_size detect 0x8000 partitions.bin
esptool.py --port /dev/cu.*serial* --baud 115200 --after no_reset write_flash --flash_mode dio --flash_freq 80m --flash_size detect 0x10000 wipy.bin

