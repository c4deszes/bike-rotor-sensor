/* Script for -n: mix text and data on same page */
/* Copyright (C) 2014-2015 Free Software Foundation, Inc.
   Copying and distribution of this script, with or without modification,
   are permitted in any medium without royalty provided the copyright
   notice and this notice are preserved.  */
OUTPUT_FORMAT("elf32-avr","elf32-avr","elf32-avr")
OUTPUT_ARCH(avr:103)

/*
 * It's very difficult to change these values or shift stuff around, see:
 * 
 * - https://www.avrfreaks.net/forum/attiny-817-ldexe-error-bss-not-within-region-data
 * - https://electronics.stackexchange.com/questions/408115/how-does-avr-gcc-linker-know-to-put-the-datasection-at-0x800100-rather-than
 * - https://www.avrfreaks.net/forum/solved-compiling-attiny1607-or-other-0-series1-series-avr-gcc
 */
__TEXT_REGION_LENGTH__ = DEFINED(__TEXT_REGION_LENGTH__) ? __TEXT_REGION_LENGTH__ : 1024K;
__DATA_REGION_LENGTH__ = DEFINED(__DATA_REGION_LENGTH__) ? __DATA_REGION_LENGTH__ : 0xffa0;
__EEPROM_REGION_LENGTH__ = DEFINED(__EEPROM_REGION_LENGTH__) ? __EEPROM_REGION_LENGTH__ : 64K;
__FUSE_REGION_LENGTH__ = DEFINED(__FUSE_REGION_LENGTH__) ? __FUSE_REGION_LENGTH__ : 1K;
__LOCK_REGION_LENGTH__ = DEFINED(__LOCK_REGION_LENGTH__) ? __LOCK_REGION_LENGTH__ : 1K;
__SIGNATURE_REGION_LENGTH__ = DEFINED(__SIGNATURE_REGION_LENGTH__) ? __SIGNATURE_REGION_LENGTH__ : 1K;
__USER_SIGNATURE_REGION_LENGTH__ = DEFINED(__USER_SIGNATURE_REGION_LENGTH__) ? __USER_SIGNATURE_REGION_LENGTH__ : 1K;
__RODATA_PM_OFFSET__ = DEFINED(__RODATA_PM_OFFSET__) ? __RODATA_PM_OFFSET__ : 0x8000;
__DATA_REGION_ORIGIN__ = DEFINED(__DATA_REGION_ORIGIN__) ? __DATA_REGION_ORIGIN__ : 0x802000;

/**
 * @brief Memory Layout
 * 
 * |------------------|
 * | Bootloader ROM   |
 * |                  |
 * |                  |
 * |------------------|
 * | Application ROM  |
 * |                  |
 * |                  |
 * |                  |
 * |------------------|
 * | Shared data      |
 * |------------------|
 * | Data             |
 * |                  |
 * |                  |
 * |------------------|
 */
MEMORY
{
  bootloader   (rx)   : ORIGIN = 0, LENGTH = 512
  application  (rw!x) : ORIGIN = 512, LENGTH = __TEXT_REGION_LENGTH__ - 512
  data   (rw!x) : ORIGIN = __DATA_REGION_ORIGIN__, LENGTH = __DATA_REGION_LENGTH__
  eeprom (rw!x) : ORIGIN = 0x810000, LENGTH = __EEPROM_REGION_LENGTH__
  fuse      (rw!x) : ORIGIN = 0x820000, LENGTH = __FUSE_REGION_LENGTH__
  lock      (rw!x) : ORIGIN = 0x830000, LENGTH = __LOCK_REGION_LENGTH__
  signature (rw!x) : ORIGIN = 0x840000, LENGTH = __SIGNATURE_REGION_LENGTH__
  user_signatures (rw!x) : ORIGIN = 0x850000, LENGTH = __USER_SIGNATURE_REGION_LENGTH__
}

__bootloader_start__ = ORIGIN(bootloader);
__bootloader_size__ = LENGTH(bootloader);
__application_start__ = ORIGIN(application);
__application_size__ = LENGTH(application);
