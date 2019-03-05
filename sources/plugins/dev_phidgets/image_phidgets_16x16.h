#include <ximage.h>

static uint8_t phidgets_16x16_data[] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x85, 0x7D, 0x84, 0xC2, 
    0xC7, 0xCE, 0xCD, 0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
    0x6C, 0x67, 0x6C, 0x62, 0x91, 0xBD, 0xA2, 0xFF, 0x57, 0xA3, 0x65, 0xFF, 0x5F, 0xB4, 0x7F, 0xFF, 0x74, 0xC1, 0x97, 0xFF, 
    0x75, 0xC8, 0x8D, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
    0x00, 0x00, 0x00, 0xFF, 0xC1, 0xC0, 0xC0, 0xD2, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFA, 0xFF, 0xC1, 0xCC, 0x89, 0xFF, 
    0x48, 0x19, 0x3A, 0xFF, 0xC6, 0xB5, 0x8C, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xBD, 0xBB, 0xBD, 0xE4, 0xFA, 0xF5, 0xFF, 0xFF, 0xF6, 0xFF, 0xF8, 0xFF, 
    0x73, 0xA0, 0x5A, 0xFF, 0x04, 0x00, 0x00, 0xFF, 0x74, 0x81, 0x4E, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x7F, 0x7E, 0x80, 0x75, 0xFF, 0xF0, 0xE7, 0xFF, 
    0xD5, 0xDC, 0xA6, 0xFF, 0x4D, 0xA1, 0x82, 0xFF, 0x91, 0xCF, 0xB3, 0xFF, 0x7F, 0xCB, 0xA4, 0xFF, 0x3E, 0x87, 0x90, 0xFF, 
    0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x09, 0x43, 0x41, 0xFF, 0x09, 0x09, 0x09, 0x32, 
    0x25, 0x09, 0x06, 0xFF, 0x4D, 0x51, 0x0D, 0xFF, 0x5C, 0x75, 0x50, 0xFF, 0x6D, 0xA1, 0x81, 0xFF, 0xA7, 0xA7, 0x81, 0xFF, 
    0xA0, 0x8D, 0x51, 0xFF, 0x1F, 0x5F, 0x7C, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x17, 0x4F, 0x6B, 0xFF, 0x08, 0x50, 0x1A, 0xFF, 
    0x68, 0x63, 0x60, 0x56, 0x76, 0x93, 0xAE, 0xFF, 0x91, 0xAC, 0x98, 0xFF, 0x51, 0x91, 0x61, 0xFF, 0x23, 0x77, 0x48, 0xFF, 
    0x13, 0x62, 0x37, 0xFF, 0x08, 0x4F, 0x22, 0xFF, 0x02, 0x5A, 0x12, 0xFF, 0x21, 0x3E, 0x4D, 0xFF, 0x00, 0x00, 0x00, 0xFF, 
    0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0x2D, 0x63, 0x67, 0xFF, 0x11, 0x66, 0x2A, 0xFF, 
    0x04, 0x4E, 0x1A, 0xFF, 0xC6, 0xC4, 0xC3, 0xDA, 0x46, 0xC3, 0xFF, 0xFF, 0x12, 0xA0, 0xFF, 0xFF, 0x77, 0x73, 0x3B, 0xFF, 
    0x49, 0x7B, 0x4D, 0xFF, 0x3E, 0x82, 0x5B, 0xFF, 0x40, 0x84, 0x61, 0xFF, 0x2F, 0x75, 0x55, 0xFF, 0x0D, 0x62, 0x42, 0xFF, 
    0x04, 0x55, 0x21, 0xFF, 0x03, 0x43, 0x14, 0xFF, 0x10, 0x44, 0x1C, 0xFF, 0x07, 0x3A, 0x16, 0xFF, 0x29, 0x73, 0x58, 0xFF, 
    0x1C, 0x6B, 0x46, 0xFF, 0x16, 0x63, 0x2F, 0xFF, 0xBE, 0xBD, 0xB2, 0xC4, 0x0C, 0x82, 0xFD, 0xFF, 0x05, 0x70, 0xE9, 0xFF, 
    0x45, 0x83, 0x3C, 0xFF, 0x3B, 0x81, 0x46, 0xFF, 0x41, 0x83, 0x5A, 0xFF, 0x2F, 0x7A, 0x56, 0xFB, 0x5F, 0xA6, 0x79, 0xFF, 
    0xB5, 0xE2, 0xA6, 0xFF, 0x92, 0xCE, 0xA0, 0xFF, 0x97, 0xD3, 0xA0, 0xFF, 0x96, 0xDE, 0xA8, 0xFF, 0x87, 0xCE, 0x98, 0xFF, 
    0xBB, 0xE4, 0xAC, 0xFF, 0x6E, 0xAB, 0x81, 0xFF, 0x18, 0x67, 0x3D, 0xEB, 0x6F, 0x63, 0x55, 0x71, 0x6B, 0xAB, 0xE2, 0xF3, 
    0x01, 0x7D, 0xFF, 0x5F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x21, 0x5C, 0x26, 0x97, 0xF0, 0xCA, 0x68, 0xFF, 0xCF, 0xB7, 0x5A, 0xFF, 0xCD, 0xB1, 0x59, 0xFF, 0xC4, 0xB2, 0x5B, 0xFF, 
    0xC5, 0xB1, 0x5B, 0xFF, 0xED, 0xC7, 0x63, 0xFF, 0x4E, 0x7A, 0x3C, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x8F, 0x7C, 0x40, 0xFF, 0x92, 0x7E, 0x40, 0xFF, 0x8E, 0x7C, 0x40, 0xFF, 0x88, 0x7B, 0x40, 0xFF, 
    0x86, 0x7E, 0x41, 0xFF, 0x82, 0x7C, 0x42, 0xFF, 0x9B, 0x81, 0x3E, 0xFF, 0x8B, 0x7B, 0x3E, 0xFF, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 
};

static const ximage image_phidgets_16x16 =
{
    &phidgets_16x16_data[0],
    16, 16,
    64,
    XPixelFormatRGBA32
};
