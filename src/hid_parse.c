#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hid_parse.h"

hid_report_t hid_report[HID_REPORTS_COUNT] = {0};

void hid_parse_report_descriptor(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
  // Check if the report will have a Report ID

  uint32_t reportByte = 0;
  uint8_t valueSize = 0;
  uint32_t value = 0;
  uint8_t tag = 0;
  uint8_t type  = 0;
  int8_t usageGlobal = 0;
  uint8_t usagePage = 0;
  int8_t repIndex = 0;

  while(reportByte < len)
  {
    tag = report[reportByte] & 0xfc;
    valueSize = report[reportByte] & 0x03;
    type = (report[reportByte] >> 2) & 0x03;
    reportByte++;
    switch(tag)
    {
      case H_USAGE_PAGE:
        usagePage = report[reportByte];
        break;

      case H_USAGE:
        if(report[reportByte] <= H_USAGE_DEVICE_MAX)
          usageGlobal = report[reportByte];
        break;

      case H_COLLECTION:
        if(report[reportByte] == H_COLLECTION_APPLICATION) {
          repIndex = hid_find_free_report_descriptor();
          hid_report[repIndex].usage = usageGlobal; // Set previously read global usage here
          hid_report[repIndex].usagePage = usagePage; // Set previously read usage page here
          hid_report[repIndex].deviceAddress = dev_addr;
          hid_report[repIndex].instanceId = instance;
        }
        break;

      case H_REPORT_ID:
        hid_report[repIndex].reportId = report[reportByte];
        break;
    }
    reportByte += valueSize;
  }
  //printf("ReportID: %d\n", kb_reid);
}

void hid_print_report_descriptors()
{
  for(uint8_t i=0; i<HID_REPORTS_COUNT; i++)
  {
    if(hid_report[i].usage > 0)
    {
      printf("Device: %d\nInstance: %d\nReportID: %d\nUsage: %d\n---------\n", hid_report[i].deviceAddress, hid_report[i].instanceId, hid_report[i].reportId, hid_report[i].usage);
    }
  }
}

void hid_remove_report_descriptors(uint8_t dev_addr, uint8_t instance)
{
  for(uint8_t i=0; i<HID_REPORTS_COUNT; i++)
  {
    if(hid_report[i].deviceAddress == dev_addr && hid_report[i].instanceId == instance)
      memset(&hid_report[i], 0, sizeof(hid_report_t));
  }
}

int8_t hid_find_free_report_descriptor()
{
  for(int8_t i=0; i<HID_REPORTS_COUNT; i++)
  {
    if(hid_report[i].usage == 0)
      return i;
  }
  return -1;
}

int32_t hid_get_report_descriptor_value(const uint8_t *valueArray, uint32_t index, uint32_t bytes)
{
  int8_t  value8  = 0;
  int16_t value16 = 0;
  int32_t value32 = 0;
  if(bytes == 4) {
    memcpy(&value32, &valueArray[index], 4);
    return value32;
  }
  else if(bytes == 2) {
    memcpy(&value16, &valueArray[index], 2);
    return value16;
  }
  else {
    memcpy(&value8, &valueArray[index], 1);
    return value8;
  }
}

hid_report_t* hid_get_report_descriptor(uint8_t dev_addr, uint8_t instance, uint8_t const* report)
{
  for(int8_t i=0; i<HID_REPORTS_COUNT; i++)
  {
    if(hid_report[i].usage == 0)
      return NULL;
    else if(hid_report[i].deviceAddress == dev_addr && hid_report[i].instanceId == instance && (hid_report[i].reportId == 0 || hid_report[i].reportId == report[0]))
      return &hid_report[i];
  }
  return NULL;
}