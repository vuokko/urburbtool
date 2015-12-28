unsigned char sent_png[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
  0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
  0x08, 0x06, 0x00, 0x00, 0x00, 0x1f, 0xf3, 0xff, 0x61, 0x00, 0x00, 0x00,
  0x06, 0x62, 0x4b, 0x47, 0x44, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xa0,
  0xbd, 0xa7, 0x93, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00,
  0x00, 0x0b, 0x13, 0x00, 0x00, 0x0b, 0x13, 0x01, 0x00, 0x9a, 0x9c, 0x18,
  0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4d, 0x45, 0x07, 0xdf, 0x0c, 0x0f,
  0x15, 0x3b, 0x30, 0xb2, 0xa9, 0x47, 0x02, 0x00, 0x00, 0x00, 0x1d, 0x69,
  0x54, 0x58, 0x74, 0x43, 0x6f, 0x6d, 0x6d, 0x65, 0x6e, 0x74, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x43, 0x72, 0x65, 0x61, 0x74, 0x65, 0x64, 0x20, 0x77,
  0x69, 0x74, 0x68, 0x20, 0x47, 0x49, 0x4d, 0x50, 0x64, 0x2e, 0x65, 0x07,
  0x00, 0x00, 0x00, 0x60, 0x49, 0x44, 0x41, 0x54, 0x38, 0xcb, 0xad, 0x92,
  0x41, 0x12, 0x80, 0x20, 0x0c, 0x03, 0xdb, 0x0c, 0xff, 0xff, 0x72, 0x3c,
  0x38, 0x8e, 0x0e, 0x6d, 0xd1, 0x50, 0x73, 0x85, 0x5d, 0x4a, 0xc0, 0x49,
  0x5a, 0x1e, 0x9f, 0x16, 0xe8, 0xd9, 0x2e, 0x7c, 0x83, 0xeb, 0xa0, 0x03,
  0x27, 0x82, 0x0a, 0xce, 0xc7, 0x9f, 0x04, 0x3a, 0xfc, 0x10, 0xec, 0xc1,
  0x27, 0x49, 0x13, 0xee, 0x1c, 0x85, 0xc3, 0xa4, 0xc4, 0x49, 0x61, 0xcd,
  0xfc, 0x21, 0x78, 0x2f, 0x6a, 0x95, 0x71, 0x97, 0xd3, 0x7a, 0x46, 0xad,
  0xb8, 0x42, 0xb0, 0x3a, 0xa9, 0x96, 0x20, 0x8e, 0xab, 0x75, 0x82, 0xdd,
  0x1f, 0x78, 0xe5, 0x00, 0x8e, 0xae, 0x1c, 0x1d, 0xc2, 0x3d, 0xe5, 0x03,
  0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};
unsigned int sent_png_len = 252;
unsigned char received_png[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
  0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
  0x08, 0x06, 0x00, 0x00, 0x00, 0x1f, 0xf3, 0xff, 0x61, 0x00, 0x00, 0x00,
  0x06, 0x62, 0x4b, 0x47, 0x44, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xa0,
  0xbd, 0xa7, 0x93, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00,
  0x00, 0x0b, 0x13, 0x00, 0x00, 0x0b, 0x13, 0x01, 0x00, 0x9a, 0x9c, 0x18,
  0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4d, 0x45, 0x07, 0xdf, 0x0c, 0x0f,
  0x15, 0x3b, 0x16, 0x60, 0xa4, 0xc2, 0xff, 0x00, 0x00, 0x00, 0x1d, 0x69,
  0x54, 0x58, 0x74, 0x43, 0x6f, 0x6d, 0x6d, 0x65, 0x6e, 0x74, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x43, 0x72, 0x65, 0x61, 0x74, 0x65, 0x64, 0x20, 0x77,
  0x69, 0x74, 0x68, 0x20, 0x47, 0x49, 0x4d, 0x50, 0x64, 0x2e, 0x65, 0x07,
  0x00, 0x00, 0x00, 0x5b, 0x49, 0x44, 0x41, 0x54, 0x38, 0xcb, 0xc5, 0x90,
  0xc1, 0x0e, 0xc0, 0x30, 0x08, 0x42, 0xc5, 0xf8, 0xff, 0xbf, 0x4c, 0x0f,
  0x3d, 0x2c, 0x69, 0x98, 0xda, 0x79, 0x18, 0x57, 0xe5, 0x05, 0x00, 0x49,
  0xd3, 0xc2, 0x71, 0x20, 0xd4, 0x97, 0x5b, 0x5b, 0xe0, 0x10, 0xa0, 0x21,
  0x09, 0x40, 0x47, 0x3e, 0x21, 0x45, 0x82, 0x1a, 0xd2, 0xa8, 0x90, 0x43,
  0xe2, 0x66, 0x30, 0xf5, 0x17, 0xfd, 0x67, 0x2d, 0xb7, 0xa1, 0xfe, 0x06,
  0x10, 0xf1, 0xbe, 0x72, 0x35, 0xe8, 0xf6, 0xf9, 0x37, 0x73, 0xab, 0x42,
  0x66, 0x7e, 0x52, 0xc7, 0x6d, 0xe7, 0xc1, 0x88, 0x7a, 0xab, 0x05, 0xc5,
  0x82, 0x19, 0x25, 0xc9, 0x66, 0x5d, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x49,
  0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};
unsigned int received_png_len = 247;
unsigned char error_png[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
  0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
  0x08, 0x06, 0x00, 0x00, 0x00, 0x1f, 0xf3, 0xff, 0x61, 0x00, 0x00, 0x00,
  0x06, 0x62, 0x4b, 0x47, 0x44, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xa0,
  0xbd, 0xa7, 0x93, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00,
  0x00, 0x0b, 0x13, 0x00, 0x00, 0x0b, 0x13, 0x01, 0x00, 0x9a, 0x9c, 0x18,
  0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4d, 0x45, 0x07, 0xdf, 0x0c, 0x0f,
  0x16, 0x02, 0x12, 0x6b, 0x0b, 0x35, 0x05, 0x00, 0x00, 0x00, 0x1d, 0x69,
  0x54, 0x58, 0x74, 0x43, 0x6f, 0x6d, 0x6d, 0x65, 0x6e, 0x74, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x43, 0x72, 0x65, 0x61, 0x74, 0x65, 0x64, 0x20, 0x77,
  0x69, 0x74, 0x68, 0x20, 0x47, 0x49, 0x4d, 0x50, 0x64, 0x2e, 0x65, 0x07,
  0x00, 0x00, 0x00, 0xa8, 0x49, 0x44, 0x41, 0x54, 0x38, 0xcb, 0x85, 0x52,
  0x51, 0x16, 0x04, 0x21, 0x08, 0x82, 0xde, 0xde, 0xff, 0xca, 0xec, 0xc7,
  0x64, 0x29, 0x63, 0x8d, 0x5f, 0xbd, 0x14, 0x50, 0x91, 0x92, 0x50, 0x82,
  0xb4, 0x8f, 0xe7, 0x17, 0xea, 0xbe, 0x81, 0x9f, 0x81, 0x37, 0x60, 0x87,
  0x00, 0x68, 0xe5, 0xa4, 0x9c, 0xc3, 0x68, 0xc0, 0x0f, 0x48, 0x59, 0x1d,
  0x5c, 0x40, 0xb2, 0xd4, 0x8e, 0x02, 0x9e, 0x5a, 0x45, 0x3b, 0x1e, 0x31,
  0x5a, 0x8c, 0x32, 0x31, 0xe3, 0x5d, 0x9c, 0x74, 0x3d, 0xa2, 0x8b, 0x44,
  0x32, 0xac, 0xf5, 0x20, 0x63, 0x61, 0xed, 0x16, 0x3b, 0x49, 0x46, 0xab,
  0xde, 0x39, 0x22, 0xb0, 0x2d, 0xd1, 0x95, 0x00, 0x67, 0x02, 0xba, 0x0b,
  0x6f, 0x30, 0xbe, 0xd4, 0xa1, 0x1b, 0x81, 0x93, 0xf5, 0x14, 0x76, 0x48,
  0xc6, 0xbe, 0x60, 0x4c, 0x44, 0xea, 0x76, 0x70, 0xdb, 0x43, 0x4e, 0xb8,
  0x63, 0x12, 0xc6, 0xe9, 0xc6, 0xf7, 0x02, 0x59, 0xad, 0x8b, 0xfa, 0x62,
  0x23, 0x0e, 0x33, 0xce, 0x23, 0x2e, 0x76, 0x5a, 0x17, 0x94, 0x9d, 0x66,
  0xe9, 0x3a, 0xba, 0x10, 0x58, 0x5d, 0x51, 0x43, 0x70, 0x9a, 0xf3, 0xa1,
  0xa3, 0x03, 0x23, 0xfe, 0xac, 0x1e, 0x4b, 0x17, 0x71, 0x50, 0x94, 0xb8,
  0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};
unsigned int error_png_len = 324;
unsigned char timeout_png[] = {
  0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
  0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
  0x08, 0x06, 0x00, 0x00, 0x00, 0x1f, 0xf3, 0xff, 0x61, 0x00, 0x00, 0x00,
  0x06, 0x62, 0x4b, 0x47, 0x44, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xa0,
  0xbd, 0xa7, 0x93, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00,
  0x00, 0x0b, 0x13, 0x00, 0x00, 0x0b, 0x13, 0x01, 0x00, 0x9a, 0x9c, 0x18,
  0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4d, 0x45, 0x07, 0xdf, 0x0c, 0x0f,
  0x16, 0x01, 0x2c, 0x81, 0x47, 0x7b, 0x6d, 0x00, 0x00, 0x00, 0x1d, 0x69,
  0x54, 0x58, 0x74, 0x43, 0x6f, 0x6d, 0x6d, 0x65, 0x6e, 0x74, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x43, 0x72, 0x65, 0x61, 0x74, 0x65, 0x64, 0x20, 0x77,
  0x69, 0x74, 0x68, 0x20, 0x47, 0x49, 0x4d, 0x50, 0x64, 0x2e, 0x65, 0x07,
  0x00, 0x00, 0x00, 0x8e, 0x49, 0x44, 0x41, 0x54, 0x38, 0xcb, 0x95, 0x53,
  0x41, 0x0e, 0xc0, 0x20, 0x08, 0x6b, 0x89, 0xff, 0xff, 0x72, 0x77, 0x99,
  0x53, 0x14, 0xd0, 0x91, 0x2c, 0xc6, 0xac, 0x2d, 0x50, 0x81, 0x92, 0x10,
  0x06, 0xe9, 0x7f, 0x48, 0x0c, 0x61, 0x4e, 0x60, 0x25, 0x01, 0x80, 0x40,
  0x10, 0xca, 0x84, 0x86, 0x40, 0x27, 0x0b, 0x03, 0xd0, 0x89, 0x02, 0x01,
  0xf9, 0x04, 0xaf, 0x90, 0xa5, 0x64, 0x77, 0xd7, 0x20, 0xf5, 0x0a, 0x5e,
  0x8e, 0xed, 0xe0, 0x8b, 0x98, 0x44, 0x2c, 0xec, 0x7b, 0x52, 0x3d, 0x89,
  0x58, 0x9d, 0x9d, 0xd3, 0x19, 0x43, 0x1a, 0x7e, 0x05, 0xb1, 0x1a, 0xdc,
  0x0a, 0xac, 0xce, 0x5e, 0x80, 0x76, 0x99, 0x58, 0x99, 0xa0, 0x55, 0xea,
  0xdf, 0x57, 0x54, 0x45, 0x01, 0xaa, 0x8d, 0x54, 0x39, 0xde, 0x96, 0xcd,
  0x78, 0x68, 0xdc, 0x3c, 0x48, 0xee, 0x19, 0x8f, 0xa6, 0x31, 0xad, 0x62,
  0xdf, 0x85, 0xac, 0x9d, 0x64, 0xa1, 0xb8, 0xad, 0x73, 0x35, 0x99, 0x41,
  0xbb, 0x0f, 0xa9, 0xbf, 0x53, 0x0e, 0xe1, 0x6a, 0xa5, 0x6d, 0x00, 0x00,
  0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};
unsigned int timeout_png_len = 298;
