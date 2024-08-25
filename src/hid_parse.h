#include <stdint.h>

#define NULL ((void *)0)

#define H_REPORT_ID             0x84 
#define H_USAGE_PAGE            0x04
#define H_USAGE                 0x08
#define H_COLLECTION            0xA0
#define H_COLLECTION_END        0xC0
#define H_COLLECTION_APPLICATION 0x01
#define H_REPORT_COUNT          0x94
#define H_REPORT_SIZE           0x74
#define H_INPUT_OUTPUT          0x80
//#define H_OUTPUT              0x82
#define H_LOGICAL_MIN           0x14
#define H_LOGICAL_MAX           0x24 
#define H_PHYSICAL_MIN          0x34
#define H_PHYSICAL_MAX          0x44

#define H_USAGE_PAGE_SYSTEM     0x00
#define H_USAGE_PAGE_GENERIC    0x01
#define H_USAGE_PAGE_CONSUMER   0x05
#define H_USAGE_PAGE_BUTTONS    0x09

#define H_USAGE_DEVICE_MAX      0x12
#define H_USAGE_MOUSE           0x02
#define H_USAGE_JOYSTICK        0x04
#define H_USAGE_GAMEPAD         0x05
#define H_USAGE_KEYBOARD        0x06

#define HID_REPORTS_COUNT 20

typedef struct 
{
  uint8_t deviceAddress;
  uint8_t instanceId;
  int8_t  reportId; // -1 if report has no id 
  uint8_t usagePage; // H_USAGE_PAGE_GENERIC, H_USAGE_PAGE_SYSTEM, H_USAGE_PAGE_CONSUMER
  uint8_t usage; // H_USAGE_MOUSE, H_USAGE_JOYSTICK, H_USAGE_GAMEPAD, H_USAGE_KEYBOARD
  uint32_t length; // Report length (in bytes)
} hid_report_t;

void hid_parse_report_descriptor(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len);
void hid_remove_report_descriptors(uint8_t dev_addr, uint8_t instance);
int8_t hid_find_free_report_descriptor();
int32_t hid_get_report_descriptor_value(const uint8_t *valueArray, uint32_t index, uint32_t bytes);
void hid_print_report_descriptors();
hid_report_t* hid_get_report_descriptor(uint8_t dev_addr, uint8_t instance, uint8_t const* report);