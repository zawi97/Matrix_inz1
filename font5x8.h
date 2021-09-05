const uint8_t fontHEX[][5] = {
	{0x00, 0x00, 0x00, 0x00, 0x00}, //index 0,     , 0x20 , 32
	{0x00, 0x00, 0x6f, 0x00, 0x00}, //index 1,   ! , 0x21 , 33
	{0x00, 0x07, 0x00, 0x07, 0x00}, //index 2,   " , 0x22 , 34
	{0x14, 0x7f, 0x14, 0x7f, 0x14}, //index 3,   # , 0x23 , 35
	{0x00, 0x07, 0x04, 0x1e, 0x00}, //index 4,   $ , 0x24 , 36
	{0x23, 0x13, 0x08, 0x64, 0x62}, //index 5,   % , 0x25 , 37
	{0x36, 0x49, 0x56, 0x20, 0x50}, //index 6,   & , 0x26 , 38
	{0x00, 0x00, 0x07, 0x00, 0x00}, //index 7,   ' , 0x27 , 39
	{0x00, 0x1c, 0x22, 0x41, 0x00}, //index 8,   ( , 0x28 , 40
	{0x00, 0x41, 0x22, 0x1c, 0x00}, //index 9,   ) , 0x29 , 41
	{0x14, 0x08, 0x3e, 0x08, 0x14}, //index 10,  * , 0x2a , 42
	{0x08, 0x08, 0x3e, 0x08, 0x08}, //index 11,  + , 0x2b , 43
	{0x00, 0x50, 0x30, 0x00, 0x00}, //index 12,  , , 0x2c , 44
	{0x08, 0x08, 0x08, 0x08, 0x08}, //index 13,  - , 0x2d , 45
	{0x00, 0x60, 0x60, 0x00, 0x00}, //index 14,  . , 0x2e , 46
	{0x20, 0x10, 0x08, 0x04, 0x02}, //index 15,  / , 0x2f , 47
	{0x3e, 0x51, 0x49, 0x45, 0x3e}, //index 16,  0 , 0x30 , 48
	{0x00, 0x42, 0x7f, 0x40, 0x00}, //index 17,  1 , 0x31 , 49
	{0x42, 0x61, 0x51, 0x49, 0x46}, //index 18,  2 , 0x32 , 50
	{0x21, 0x41, 0x45, 0x4b, 0x31}, //index 19,  3 , 0x33 , 51
	{0x18, 0x14, 0x12, 0x7f, 0x10}, //index 20,  4 , 0x34 , 52
	{0x27, 0x45, 0x45, 0x45, 0x39}, //index 21,  5 , 0x35 , 53
	{0x3c, 0x4a, 0x49, 0x49, 0x30}, //index 22,  6 , 0x36 , 54
	{0x01, 0x71, 0x09, 0x05, 0x03}, //index 23,  7 , 0x37 , 55
	{0x36, 0x49, 0x49, 0x49, 0x36}, //index 24,  8 , 0x38 , 56
	{0x06, 0x49, 0x49, 0x29, 0x1e}, //index 25,  9 , 0x39 , 57
	{0x00, 0x36, 0x36, 0x00, 0x00}, //index 26,  : , 0x3a , 58
	{0x00, 0x56, 0x36, 0x00, 0x00}, //index 27,  ; , 0x3b , 59
	{0x08, 0x14, 0x22, 0x41, 0x00}, //index 28,  < , 0x3c , 60
	{0x14, 0x14, 0x14, 0x14, 0x14}, //index 29,  = , 0x3d , 61
	{0x00, 0x41, 0x22, 0x14, 0x08}, //index 30,  > , 0x3e , 62
	{0x02, 0x01, 0x51, 0x09, 0x06}, //index 31,  ? , 0x3f , 63
	{0x3e, 0x41, 0x5d, 0x49, 0x4e}, //index 32,  @ , 0x40 , 64
	{0x7e, 0x09, 0x09, 0x09, 0x7e}, //index 33,  A , 0x41 , 65
	{0x7f, 0x49, 0x49, 0x49, 0x36}, //index 34,  B , 0x42 , 66
	{0x3e, 0x41, 0x41, 0x41, 0x22}, //index 35,  C , 0x43 , 67
	{0x7f, 0x41, 0x41, 0x41, 0x3e}, //index 36,  D , 0x44 , 68
	{0x7f, 0x49, 0x49, 0x49, 0x41}, //index 37,  E , 0x45 , 69
	{0x7f, 0x09, 0x09, 0x09, 0x01}, //index 38,  F , 0x46 , 70
	{0x3e, 0x41, 0x49, 0x49, 0x7a}, //index 39,  G , 0x47 , 71
	{0x7f, 0x08, 0x08, 0x08, 0x7f}, //index 40,  H , 0x48 , 72
	{0x00, 0x41, 0x7f, 0x41, 0x00}, //index 41,  I , 0x49 , 73
	{0x20, 0x40, 0x41, 0x3f, 0x01}, //index 42,  J , 0x4a , 74
	{0x7f, 0x08, 0x14, 0x22, 0x41}, //index 43,  K , 0x4b , 75
	{0x7f, 0x40, 0x40, 0x40, 0x40}, //index 44,  L , 0x4c , 76
	{0x7f, 0x02, 0x0c, 0x02, 0x7f}, //index 45,  M , 0x4d , 77
	{0x7f, 0x04, 0x08, 0x10, 0x7f}, //index 46,  N , 0x4e , 78
	{0x3e, 0x41, 0x41, 0x41, 0x3e}, //index 47,  O , 0x4f , 79
	{0x7f, 0x09, 0x09, 0x09, 0x06}, //index 48,  P , 0x50 , 80
	{0x3e, 0x41, 0x51, 0x21, 0x5e}, //index 49,  Q , 0x51 , 81
	{0x7f, 0x09, 0x19, 0x29, 0x46}, //index 50,  R , 0x52 , 82
	{0x46, 0x49, 0x49, 0x49, 0x31}, //index 51,  S , 0x53 , 83
	{0x01, 0x01, 0x7f, 0x01, 0x01}, //index 52,  T , 0x54 , 84
	{0x3f, 0x40, 0x40, 0x40, 0x3f}, //index 53,  U , 0x55 , 85
	{0x0f, 0x30, 0x40, 0x30, 0x0f}, //index 54,  V , 0x56 , 86
	{0x3f, 0x40, 0x30, 0x40, 0x3f}, //index 55,  W , 0x57 , 87
	{0x63, 0x14, 0x08, 0x14, 0x63}, //index 56,  X , 0x58 , 88
	{0x07, 0x08, 0x70, 0x08, 0x07}, //index 57,  Y , 0x59 , 89
	{0x61, 0x51, 0x49, 0x45, 0x43}, //index 58,  Z , 0x5a , 90
	{0x3c, 0x4a, 0x49, 0x29, 0x1e}, //index 59,  [ , 0x5b , 91  //
	{0x02, 0x04, 0x08, 0x10, 0x20}, //index 60,  \ , 0x5c , 92
	{0x00, 0x41, 0x7f, 0x00, 0x00}, //index 61,  ] , 0x5d , 93
	{0x04, 0x02, 0x01, 0x02, 0x04}, //index 62,  ^ , 0x5e , 94
	{0x40, 0x40, 0x40, 0x40, 0x40}, //index 63,  _ , 0x5f , 95
	{0x00, 0x00, 0x03, 0x04, 0x00}, //index 64,  ` , 0x60 , 96
	{0x20, 0x54, 0x54, 0x54, 0x78}, //index 65,  a , 0x61 , 97
	{0x7f, 0x48, 0x44, 0x44, 0x38}, //index 66,  b , 0x62 , 98
	{0x38, 0x44, 0x44, 0x44, 0x20}, //index 67,  c , 0x63 , 99
	{0x38, 0x44, 0x44, 0x48, 0x7f}, //index 68,  d , 0x64 , 100
	{0x38, 0x54, 0x54, 0x54, 0x18}, //index 69,  e , 0x65 , 101
	{0x08, 0x7e, 0x09, 0x01, 0x02}, //index 70,  f , 0x66 , 102
	{0x0c, 0x52, 0x52, 0x52, 0x3e}, //index 71,  g , 0x67 , 103
	{0x7f, 0x08, 0x04, 0x04, 0x78}, //index 72,  h , 0x68 , 104
	{0x00, 0x44, 0x7d, 0x40, 0x00}, //index 73,  i , 0x69 , 105
	{0x20, 0x40, 0x44, 0x3d, 0x00}, //index 74,  j , 0x6a , 106
	{0x00, 0x7f, 0x10, 0x28, 0x44}, //index 75,  k , 0x6b , 107
	{0x00, 0x41, 0x7f, 0x40, 0x00}, //index 76,  l , 0x6c , 108
	{0x7c, 0x04, 0x18, 0x04, 0x78}, //index 77,  m , 0x6d , 109
	{0x7c, 0x08, 0x04, 0x04, 0x78}, //index 78,  n , 0x6e , 110
	{0x38, 0x44, 0x44, 0x44, 0x38}, //index 79,  o , 0x6f , 111
	{0x7c, 0x14, 0x14, 0x14, 0x08}, //index 80,  p , 0x70 , 112
	{0x08, 0x14, 0x14, 0x18, 0x7c}, //index 81,  q , 0x71 , 113
	{0x7c, 0x08, 0x04, 0x04, 0x08}, //index 82,  r , 0x72 , 114
	{0x48, 0x54, 0x54, 0x54, 0x20}, //index 83,  s , 0x73 , 115
	{0x04, 0x3f, 0x44, 0x40, 0x20}, //index 84,  t , 0x74 , 116
	{0x3c, 0x40, 0x40, 0x20, 0x7c}, //index 85,  u , 0x75 , 117
	{0x1c, 0x20, 0x40, 0x20, 0x1c}, //index 86,  v , 0x76 , 118
	{0x3c, 0x40, 0x30, 0x40, 0x3c}, //index 87,  w , 0x77 , 119
	{0x44, 0x28, 0x10, 0x28, 0x44}, //index 88,  x , 0x78 , 120
	{0x0c, 0x50, 0x50, 0x50, 0x3c}, //index 89,  y , 0x79 , 121
	{0x44, 0x64, 0x54, 0x4c, 0x44}, //index 90,  z , 0x7a , 122
	{0x00, 0x08, 0x36, 0x41, 0x41}, //index 91,  { , 0x7b , 123
	{0x00, 0x00, 0x7f, 0x00, 0x00}, //index 92,  | , 0x7c , 124
	{0x41, 0x41, 0x36, 0x08, 0x00}, //index 93,  } , 0x7d , 125
	{0x04, 0x02, 0x04, 0x08, 0x04}, //index 94,  ~ , 0x7e , 126
	{0xff, 0xff, 0xff, 0xff, 0xff}, //index 95,  ⌂ , 0x7f , 126	//Pełny prostokąt
};
