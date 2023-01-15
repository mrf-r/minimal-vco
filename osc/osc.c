#include <stdint.h>

struct {
  uint32_t inc;
  uint32_t recp;
} table_pitch_inc[513] = {
    {0x2c9b3, 0x5b7f},  {0x2cedf, 0x5ad6},  {0x2d414, 0x5a2f},
    {0x2d953, 0x5989},  {0x2de9c, 0x58e4},  {0x2e3ef, 0x5840},
    {0x2e94c, 0x579e},  {0x2eeb2, 0x56fc},  {0x2f423, 0x565c},
    {0x2f99e, 0x55bd},  {0x2ff22, 0x551f},  {0x304b2, 0x5482},
    {0x30a4b, 0x53e7},  {0x30fef, 0x534c},  {0x3159d, 0x52b3},
    {0x31b56, 0x521a},  {0x32119, 0x5183},  {0x326e7, 0x50ed},
    {0x32cc0, 0x5058},  {0x332a4, 0x4fc4},  {0x33893, 0x4f31},
    {0x33e8c, 0x4e9f},  {0x34491, 0x4e0e},  {0x34aa1, 0x4d7f},
    {0x350bc, 0x4cf0},  {0x356e3, 0x4c62},  {0x35d14, 0x4bd6},
    {0x36352, 0x4b4a},  {0x3699b, 0x4abf},  {0x36fef, 0x4a36},
    {0x37650, 0x49ad},  {0x37cbc, 0x4925},  {0x38334, 0x489f},
    {0x389b8, 0x4819},  {0x39048, 0x4794},  {0x396e5, 0x4710},
    {0x39d8d, 0x468d},  {0x3a442, 0x460b},  {0x3ab04, 0x458a},
    {0x3b1d2, 0x450a},  {0x3b8ac, 0x448b},  {0x3bf93, 0x440d},
    {0x3c687, 0x4390},  {0x3cd88, 0x4313},  {0x3d496, 0x4298},
    {0x3dbb2, 0x421d},  {0x3e2da, 0x41a3},  {0x3ea0f, 0x412a},
    {0x3f152, 0x40b2},  {0x3f8a3, 0x403b},  {0x40001, 0x3fc5},
    {0x4076c, 0x3f4f},  {0x40ee5, 0x3edb},  {0x4166d, 0x3e67},
    {0x41e02, 0x3df4},  {0x425a5, 0x3d82},  {0x42d57, 0x3d11},
    {0x43516, 0x3ca0},  {0x43ce4, 0x3c31},  {0x444c1, 0x3bc2},
    {0x44cac, 0x3b54},  {0x454a6, 0x3ae6},  {0x45caf, 0x3a7a},
    {0x464c6, 0x3a0e},  {0x46ced, 0x39a3},  {0x47522, 0x3939},
    {0x47d67, 0x38d0},  {0x485bc, 0x3867},  {0x48e1f, 0x37ff},
    {0x49692, 0x3798},  {0x49f15, 0x3732},  {0x4a7a8, 0x36cc},
    {0x4b04b, 0x3667},  {0x4b8fd, 0x3603},  {0x4c1c0, 0x359f},
    {0x4ca93, 0x353d},  {0x4d377, 0x34db},  {0x4dc6a, 0x3479},
    {0x4e56f, 0x3419},  {0x4ee84, 0x33b9},  {0x4f7aa, 0x3359},
    {0x500e1, 0x32fb},  {0x50a29, 0x329d},  {0x51383, 0x3240},
    {0x51ced, 0x31e3},  {0x5266a, 0x3187},  {0x52ff7, 0x312c},
    {0x53997, 0x30d2},  {0x54348, 0x3078},  {0x54d0c, 0x301e},
    {0x556e1, 0x2fc6},  {0x560c9, 0x2f6e},  {0x56ac3, 0x2f16},
    {0x574cf, 0x2ec0},  {0x57eee, 0x2e6a},  {0x58920, 0x2e14},
    {0x59365, 0x2dbf},  {0x59dbd, 0x2d6b},  {0x5a828, 0x2d17},
    {0x5b2a7, 0x2cc4},  {0x5bd39, 0x2c72},  {0x5c7de, 0x2c20},
    {0x5d298, 0x2bcf},  {0x5dd65, 0x2b7e},  {0x5e846, 0x2b2e},
    {0x5f33b, 0x2ade},  {0x5fe45, 0x2a90},  {0x60963, 0x2a41},
    {0x61496, 0x29f3},  {0x61fde, 0x29a6},  {0x62b3a, 0x2959},
    {0x636ac, 0x290d},  {0x64233, 0x28c2},  {0x64dcf, 0x2877},
    {0x65980, 0x282c},  {0x66548, 0x27e2},  {0x67125, 0x2799},
    {0x67d19, 0x2750},  {0x68922, 0x2707},  {0x69542, 0x26bf},
    {0x6a178, 0x2678},  {0x6adc5, 0x2631},  {0x6ba29, 0x25eb},
    {0x6c6a4, 0x25a5},  {0x6d335, 0x2560},  {0x6dfdf, 0x251b},
    {0x6ec9f, 0x24d6},  {0x6f978, 0x2493},  {0x70668, 0x244f},
    {0x71370, 0x240c},  {0x72090, 0x23ca},  {0x72dc9, 0x2388},
    {0x73b1a, 0x2347},  {0x74884, 0x2306},  {0x75607, 0x22c5},
    {0x763a3, 0x2285},  {0x77158, 0x2246},  {0x77f27, 0x2206},
    {0x78d0f, 0x21c8},  {0x79b11, 0x218a},  {0x7a92d, 0x214c},
    {0x7b763, 0x210e},  {0x7c5b4, 0x20d2},  {0x7d41f, 0x2095},
    {0x7e2a4, 0x2059},  {0x7f145, 0x201e},  {0x80001, 0x1fe2},
    {0x80ed8, 0x1fa8},  {0x81dcb, 0x1f6d},  {0x82cd9, 0x1f34},
    {0x83c04, 0x1efa},  {0x84b4a, 0x1ec1},  {0x85aad, 0x1e88},
    {0x86a2d, 0x1e50},  {0x879c9, 0x1e18},  {0x88982, 0x1de1},
    {0x89958, 0x1daa},  {0x8a94c, 0x1d73},  {0x8b95d, 0x1d3d},
    {0x8c98c, 0x1d07},  {0x8d9d9, 0x1cd2},  {0x8ea45, 0x1c9d},
    {0x8facf, 0x1c68},  {0x90b77, 0x1c34},  {0x91c3e, 0x1c00},
    {0x92d25, 0x1bcc},  {0x93e2b, 0x1b99},  {0x94f50, 0x1b66},
    {0x96095, 0x1b34},  {0x971fb, 0x1b01},  {0x98380, 0x1ad0},
    {0x99526, 0x1a9e},  {0x9a6ed, 0x1a6d},  {0x9b8d5, 0x1a3d},
    {0x9cade, 0x1a0c},  {0x9dd08, 0x19dc},  {0x9ef54, 0x19ad},
    {0xa01c3, 0x197d},  {0xa1453, 0x194f},  {0xa2706, 0x1920},
    {0xa39db, 0x18f2},  {0xa4cd3, 0x18c4},  {0xa5fef, 0x1896},
    {0xa732e, 0x1869},  {0xa8690, 0x183c},  {0xa9a17, 0x180f},
    {0xaadc2, 0x17e3},  {0xac191, 0x17b7},  {0xad585, 0x178b},
    {0xae99e, 0x1760},  {0xafddd, 0x1735},  {0xb1241, 0x170a},
    {0xb26cb, 0x16e0},  {0xb3b7a, 0x16b6},  {0xb5051, 0x168c},
    {0xb654e, 0x1662},  {0xb7a71, 0x1639},  {0xb8fbc, 0x1610},
    {0xba52f, 0x15e7},  {0xbbac9, 0x15bf},  {0xbd08c, 0x1597},
    {0xbe677, 0x156f},  {0xbfc8a, 0x1548},  {0xc12c6, 0x1521},
    {0xc292c, 0x14fa},  {0xc3fbb, 0x14d3},  {0xc5674, 0x14ad},
    {0xc6d57, 0x1487},  {0xc8465, 0x1461},  {0xc9b9d, 0x143b},
    {0xcb301, 0x1416},  {0xcca90, 0x13f1},  {0xce24a, 0x13cc},
    {0xcfa31, 0x13a8},  {0xd1244, 0x1384},  {0xd2a84, 0x1360},
    {0xd42f0, 0x133c},  {0xd5b8a, 0x1319},  {0xd7452, 0x12f5},
    {0xd8d47, 0x12d2},  {0xda66b, 0x12b0},  {0xdbfbd, 0x128d},
    {0xdd93f, 0x126b},  {0xdf2ef, 0x1249},  {0xe0cd0, 0x1228},
    {0xe26e0, 0x1206},  {0xe4121, 0x11e5},  {0xe5b92, 0x11c4},
    {0xe7635, 0x11a3},  {0xe9109, 0x1183},  {0xeac0e, 0x1163},
    {0xec746, 0x1143},  {0xee2b0, 0x1123},  {0xefe4e, 0x1103},
    {0xf1a1e, 0x10e4},  {0xf3622, 0x10c5},  {0xf525a, 0x10a6},
    {0xf6ec6, 0x1087},  {0xf8b67, 0x1069},  {0xfa83d, 0x104b},
    {0xfc549, 0x102d},  {0xfe28a, 0x100f},  {0x100002, 0xff1},
    {0x101db0, 0xfd4},  {0x103b96, 0xfb7},  {0x1059b3, 0xf9a},
    {0x107808, 0xf7d},  {0x109695, 0xf60},  {0x10b55b, 0xf44},
    {0x10d459, 0xf28},  {0x10f392, 0xf0c},  {0x111304, 0xef0},
    {0x1132b1, 0xed5},  {0x115298, 0xeba},  {0x1172ba, 0xe9e},
    {0x119319, 0xe84},  {0x11b3b3, 0xe69},  {0x11d489, 0xe4e},
    {0x11f59d, 0xe34},  {0x1216ee, 0xe1a},  {0x12387d, 0xe00},
    {0x125a4a, 0xde6},  {0x127c55, 0xdcc},  {0x129ea0, 0xdb3},
    {0x12c12b, 0xd9a},  {0x12e3f5, 0xd81},  {0x130700, 0xd68},
    {0x132a4d, 0xd4f},  {0x134dda, 0xd37},  {0x1371aa, 0xd1e},
    {0x1395bc, 0xd06},  {0x13ba11, 0xcee},  {0x13dea9, 0xcd6},
    {0x140385, 0xcbf},  {0x1428a6, 0xca7},  {0x144e0b, 0xc90},
    {0x1473b6, 0xc79},  {0x1499a6, 0xc62},  {0x14bfde, 0xc4b},
    {0x14e65b, 0xc34},  {0x150d21, 0xc1e},  {0x15342e, 0xc08},
    {0x155b84, 0xbf1},  {0x158322, 0xbdb},  {0x15ab0b, 0xbc6},
    {0x15d33d, 0xbb0},  {0x15fbba, 0xb9a},  {0x162482, 0xb85},
    {0x164d95, 0xb70},  {0x1676f5, 0xb5b},  {0x16a0a1, 0xb46},
    {0x16ca9b, 0xb31},  {0x16f4e3, 0xb1c},  {0x171f79, 0xb08},
    {0x174a5e, 0xaf4},  {0x177593, 0xae0},  {0x17a118, 0xacb},
    {0x17cced, 0xab8},  {0x17f914, 0xaa4},  {0x18258d, 0xa90},
    {0x185258, 0xa7d},  {0x187f76, 0xa6a},  {0x18ace8, 0xa56},
    {0x18daaf, 0xa43},  {0x1908ca, 0xa30},  {0x19373b, 0xa1e},
    {0x196602, 0xa0b},  {0x199520, 0x9f9},  {0x19c495, 0x9e6},
    {0x19f462, 0x9d4},  {0x1a2488, 0x9c2},  {0x1a5507, 0x9b0},
    {0x1a85e0, 0x99e},  {0x1ab714, 0x98c},  {0x1ae8a3, 0x97b},
    {0x1b1a8e, 0x969},  {0x1b4cd6, 0x958},  {0x1b7f7b, 0x947},
    {0x1bb27d, 0x936},  {0x1be5df, 0x925},  {0x1c199f, 0x914},
    {0x1c4dc0, 0x903},  {0x1c8242, 0x8f2},  {0x1cb725, 0x8e2},
    {0x1cec6a, 0x8d2},  {0x1d2211, 0x8c1},  {0x1d581d, 0x8b1},
    {0x1d8e8c, 0x8a1},  {0x1dc561, 0x891},  {0x1dfc9b, 0x882},
    {0x1e343c, 0x872},  {0x1e6c44, 0x862},  {0x1ea4b4, 0x853},
    {0x1edd8c, 0x844},  {0x1f16ce, 0x834},  {0x1f507a, 0x825},
    {0x1f8a92, 0x816},  {0x1fc515, 0x807},  {0x200004, 0x7f9},
    {0x203b61, 0x7ea},  {0x20772c, 0x7db},  {0x20b366, 0x7cd},
    {0x20f010, 0x7bf},  {0x212d2a, 0x7b0},  {0x216ab5, 0x7a2},
    {0x21a8b3, 0x794},  {0x21e724, 0x786},  {0x222608, 0x778},
    {0x226561, 0x76a},  {0x22a530, 0x75d},  {0x22e575, 0x74f},
    {0x232631, 0x742},  {0x236766, 0x734},  {0x23a913, 0x727},
    {0x23eb3a, 0x71a},  {0x242ddc, 0x70d},  {0x2470fa, 0x700},
    {0x24b494, 0x6f3},  {0x24f8ab, 0x6e6},  {0x253d41, 0x6da},
    {0x258256, 0x6cd},  {0x25c7eb, 0x6c0},  {0x260e01, 0x6b4},
    {0x265499, 0x6a8},  {0x269bb4, 0x69b},  {0x26e353, 0x68f},
    {0x272b77, 0x683},  {0x277421, 0x677},  {0x27bd52, 0x66b},
    {0x28070a, 0x65f},  {0x28514b, 0x654},  {0x289c16, 0x648},
    {0x28e76c, 0x63c},  {0x29334d, 0x631},  {0x297fbb, 0x626},
    {0x29ccb7, 0x61a},  {0x2a1a42, 0x60f},  {0x2a685c, 0x604},
    {0x2ab708, 0x5f9},  {0x2b0645, 0x5ee},  {0x2b5615, 0x5e3},
    {0x2ba67a, 0x5d8},  {0x2bf773, 0x5cd},  {0x2c4903, 0x5c3},
    {0x2c9b2a, 0x5b8},  {0x2cedea, 0x5ad},  {0x2d4143, 0x5a3},
    {0x2d9536, 0x599},  {0x2de9c6, 0x58e},  {0x2e3ef2, 0x584},
    {0x2e94bc, 0x57a},  {0x2eeb25, 0x570},  {0x2f422f, 0x566},
    {0x2f99da, 0x55c},  {0x2ff228, 0x552},  {0x304b19, 0x548},
    {0x30a4b0, 0x53e},  {0x30feed, 0x535},  {0x3159d1, 0x52b},
    {0x31b55e, 0x522},  {0x321194, 0x518},  {0x326e76, 0x50f},
    {0x32cc04, 0x505},  {0x332a3f, 0x4fc},  {0x33892a, 0x4f3},
    {0x33e8c4, 0x4ea},  {0x344910, 0x4e1},  {0x34aa0e, 0x4d8},
    {0x350bc1, 0x4cf},  {0x356e28, 0x4c6},  {0x35d146, 0x4bd},
    {0x36351c, 0x4b5},  {0x3699ab, 0x4ac},  {0x36fef5, 0x4a3},
    {0x3764fb, 0x49b},  {0x37cbbd, 0x492},  {0x38333f, 0x48a},
    {0x389b81, 0x482},  {0x390483, 0x479},  {0x396e49, 0x471},
    {0x39d8d3, 0x469},  {0x3a4423, 0x461},  {0x3ab039, 0x459},
    {0x3b1d18, 0x451},  {0x3b8ac2, 0x449},  {0x3bf936, 0x441},
    {0x3c6878, 0x439},  {0x3cd887, 0x431},  {0x3d4967, 0x429},
    {0x3dbb18, 0x422},  {0x3e2d9c, 0x41a},  {0x3ea0f5, 0x413},
    {0x3f1523, 0x40b},  {0x3f8a29, 0x404},  {0x400008, 0x3fc},
    {0x4076c2, 0x3f5},  {0x40ee58, 0x3ee},  {0x4166cc, 0x3e6},
    {0x41e01f, 0x3df},  {0x425a53, 0x3d8},  {0x42d56a, 0x3d1},
    {0x435166, 0x3ca},  {0x43ce47, 0x3c3},  {0x444c10, 0x3bc},
    {0x44cac2, 0x3b5},  {0x454a60, 0x3ae},  {0x45caea, 0x3a8},
    {0x464c63, 0x3a1},  {0x46cecb, 0x39a},  {0x475226, 0x394},
    {0x47d674, 0x38d},  {0x485bb8, 0x386},  {0x48e1f3, 0x380},
    {0x496927, 0x37a},  {0x49f156, 0x373},  {0x4a7a81, 0x36d},
    {0x4b04ab, 0x366},  {0x4b8fd6, 0x360},  {0x4c1c02, 0x35a},
    {0x4ca932, 0x354},  {0x4d3769, 0x34e},  {0x4dc6a7, 0x348},
    {0x4e56ef, 0x342},  {0x4ee842, 0x33c},  {0x4f7aa3, 0x336},
    {0x500e14, 0x330},  {0x50a296, 0x32a},  {0x51382c, 0x324},
    {0x51ced7, 0x31e},  {0x52669a, 0x318},  {0x52ff76, 0x313},
    {0x53996e, 0x30d},  {0x543483, 0x307},  {0x54d0b8, 0x302},
    {0x556e0f, 0x2fc},  {0x560c8a, 0x2f7},  {0x56ac2b, 0x2f1},
    {0x574cf4, 0x2ec},  {0x57eee7, 0x2e7},  {0x589206, 0x2e1},
    {0x593654, 0x2dc},  {0x59dbd3, 0x2d7},  {0x5a8285, 0x2d1},
    {0x5b2a6d, 0x2cc},  {0x5bd38b, 0x2c7},  {0x5c7de4, 0x2c2},
    {0x5d2978, 0x2bd},  {0x5dd64b, 0x2b8},  {0x5e845e, 0x2b3},
    {0x5f33b4, 0x2ae},  {0x5fe450, 0x2a9},  {0x609633, 0x2a4},
    {0x614960, 0x29f},  {0x61fdd9, 0x29a},  {0x62b3a2, 0x296},
    {0x636abb, 0x291},  {0x642328, 0x28c},  {0x64dcec, 0x287},
    {0x659808, 0x283},  {0x66547f, 0x27e},  {0x671253, 0x27a},
    {0x67d188, 0x275},  {0x689220, 0x270},  {0x69541c, 0x26c},
    {0x6a1781, 0x267},  {0x6adc50, 0x263},  {0x6ba28c, 0x25f},
    {0x6c6a38, 0x25a},  {0x6d3357, 0x256},  {0x6dfdea, 0x252},
    {0x6ec9f5, 0x24d},  {0x6f977b, 0x249},  {0x70667e, 0x245},
    {0x713701, 0x241},  {0x720907, 0x23d},  {0x72dc92, 0x239},
    {0x73b1a6, 0x234},  {0x748845, 0x230},  {0x756073, 0x22c},
    {0x763a31, 0x228},  {0x771583, 0x224},  {0x77f26c, 0x220},
    {0x78d0ef, 0x21c},  {0x79b10f, 0x219},  {0x7a92ce, 0x215},
    {0x7b7631, 0x211},  {0x7c5b39, 0x20d},  {0x7d41ea, 0x209},
    {0x7e2a46, 0x206},  {0x7f1452, 0x202},  {0x800011, 0x1fe},
    {0x80ed84, 0x1fa},  {0x81dcb0, 0x1f7},  {0x82cd97, 0x1f3},
    {0x83c03e, 0x1f0},  {0x84b4a7, 0x1ec},  {0x85aad5, 0x1e9},
    {0x86a2cb, 0x1e5},  {0x879c8e, 0x1e2},  {0x889820, 0x1de},
    {0x899585, 0x1db},  {0x8a94c0, 0x1d7},  {0x8b95d4, 0x1d4},
    {0x8c98c5, 0x1d0},  {0x8d9d97, 0x1cd},  {0x8ea44c, 0x1ca},
    {0x8face9, 0x1c6},  {0x90b770, 0x1c3},  {0x91c3e6, 0x1c0},
    {0x92d24e, 0x1bd},  {0x93e2ac, 0x1ba},  {0x94f503, 0x1b6},
    {0x960957, 0x1b3},  {0x971fab, 0x1b0},  {0x983804, 0x1ad},
    {0x995265, 0x1aa},  {0x9a6ed2, 0x1a7},  {0x9b8d4e, 0x1a4},
    {0x9cadde, 0x1a1},  {0x9dd085, 0x19e},  {0x9ef547, 0x19b},
    {0xa01c28, 0x198},  {0xa1452d, 0x195},  {0xa27058, 0x192},
    {0xa39dae, 0x18f},  {0xa4cd34, 0x18c},  {0xa5feec, 0x189},
    {0xa732dc, 0x187},  {0xa86906, 0x184},  {0xa9a171, 0x181},
    {0xaadc1e, 0x17e},  {0xac1914, 0x17b},  {0xad5855, 0x179},
    {0xae99e7, 0x176},  {0xafddcd, 0x173},  {0xb1240c, 0x171},
    {0xb26ca9, 0x16e},  {0xb3b7a7, 0x16b},  {0xb5050b, 0x169},
    {0xb654d9, 0x166},  {0xb7a716, 0x164},  {0xb8fbc7, 0x161},
    {0xba52f0, 0x15e},  {0xbbac95, 0x15c},  {0xbd08bc, 0x159},
    {0xbe6769, 0x157},  {0xbfc8a0, 0x154},  {0xc12c66, 0x152},
    {0xc292c0, 0x150},  {0xc3fbb3, 0x14d},  {0xc56744, 0x14b},
    {0xc6d577, 0x148},  {0xc84651, 0x146},  {0xc9b9d8, 0x144},
    {0xcb300f, 0x141},  {0xcca8fd, 0x13f},  {0xce24a7, 0x13d},
    {0xcfa310, 0x13a},  {0xd1243f, 0x138},  {0xd2a839, 0x136},
    {0xd42f02, 0x134},  {0xd5b8a0, 0x132},  {0xd74519, 0x12f},
    {0xd8d470, 0x12d},  {0xda66ad, 0x12b},  {0xdbfbd4, 0x129},
    {0xdd93ea, 0x127},  {0xdf2ef6, 0x125},  {0xe0ccfc, 0x122},
    {0xe26e02, 0x120},  {0xe4120e, 0x11e},  {0xe5b925, 0x11c},
    {0xe7634c, 0x11a},  {0xe9108b, 0x118},  {0xeac0e5, 0x116},
    {0xec7462, 0x114},  {0xee2b06, 0x112},  {0xefe4d9, 0x110},
    {0xf1a1de, 0x10e},  {0xf3621e, 0x10c},  {0xf5259d, 0x10a},
    {0xf6ec61, 0x108},  {0xf8b671, 0x107},  {0xfa83d3, 0x105},
    {0xfc548d, 0x103},  {0xfe28a5, 0x101},  {0x1000021, 0xff},
    {0x101db08, 0xfd},  {0x103b960, 0xfb},  {0x1059b2f, 0xfa},
    {0x107807c, 0xf8},  {0x109694d, 0xf6},  {0x10b55a9, 0xf4},
    {0x10d4597, 0xf3},  {0x10f391c, 0xf1},  {0x1113040, 0xef},
    {0x1132b0a, 0xed},  {0x115297f, 0xec},  {0x1172ba8, 0xea},
    {0x119318a, 0xe8},  {0x11b3b2d, 0xe7},  {0x11d4898, 0xe5},
    {0x11f59d1, 0xe3},  {0x1216ee1, 0xe2},  {0x12387cd, 0xe0},
    {0x125a49d, 0xde},  {0x127c558, 0xdd},  {0x129ea06, 0xdb},
    {0x12c12ae, 0xda},  {0x12e3f56, 0xd8},  {0x1307008, 0xd6},
    {0x132a4ca, 0xd5},  {0x134dda3, 0xd3},  {0x1371a9c, 0xd2},
    {0x1395bbb, 0xd0},  {0x13ba109, 0xcf},  {0x13dea8e, 0xcd},
    {0x1403851, 0xcc},  {0x1428a59, 0xca},  {0x144e0b0, 0xc9},
    {0x1473b5d, 0xc8},  {0x1499a67, 0xc6},  {0x14bfdd8, 0xc5},
    {0x14e65b7, 0xc3},  {0x150d20d, 0xc2},  {0x15342e1, 0xc0},
    {0x155b83d, 0xbf},  {0x1583228, 0xbe},  {0x15ab0ab, 0xbc},
    {0x15d33ce, 0xbb},  {0x15fbb9b, 0xba},  {0x1624819, 0xb8},
    {0x164d951, 0xb7},  {0x1676f4d, 0xb6},  {0x16a0a15, 0xb4},
    {0x16ca9b2, 0xb3},  {0x16f4e2d, 0xb2},  {0x171f78e, 0xb1},
    {0x174a5e0, 0xaf},  {0x177592b, 0xae},  {0x17a1178, 0xad},
    {0x17cced1, 0xab},  {0x17f913f, 0xaa},  {0x18258cc, 0xa9},
    {0x1852580, 0xa8},  {0x187f766, 0xa7},  {0x18ace87, 0xa5},
    {0x18daaed, 0xa4},  {0x1908ca2, 0xa3},  {0x19373af, 0xa2},
    {0x196601f, 0xa1},  {0x19951fb, 0xa0},  {0x19c494d, 0x9e},
    {0x19f4621, 0x9d},  {0x1a2487f, 0x9c},  {0x1a55072, 0x9b},
    {0x1a85e04, 0x9a},  {0x1ab7140, 0x99},  {0x1ae8a31, 0x98},
    {0x1b1a8e1, 0x97},  {0x1b4cd5a, 0x95},  {0x1b7f7a8, 0x94},
    {0x1bb27d5, 0x93},  {0x1be5dec, 0x92},  {0x1c199f8, 0x91},
    {0x1c4dc04, 0x90},  {0x1c8241b, 0x8f},  {0x1cb7249, 0x8e},
    {0x1cec699, 0x8d},  {0x1d22115, 0x8c},  {0x1d581ca, 0x8b},
    {0x1d8e8c4, 0x8a},  {0x1dc560d, 0x89},  {0x1dfc9b1, 0x88},
    {0x1e343bd, 0x87},  {0x1e6c43c, 0x86},  {0x1ea4b3a, 0x85},
    {0x1edd8c2, 0x84},  {0x1f16ce3, 0x83},  {0x1f507a6, 0x82},
    {0x1f8a91a, 0x81},  {0x1fc514a, 0x80},  {0x2000042, 0x80},
    {0x203b610, 0x7f},  {0x20772c0, 0x7e},  {0x20b365e, 0x7d},
    {0x20f00f8, 0x7c},  {0x212d29b, 0x7b},  {0x216ab53, 0x7a},
    {0x21a8b2e, 0x79},  {0x21e7238, 0x78},  {0x2226081, 0x78},
    {0x2265614, 0x77},  {0x22a52ff, 0x76},  {0x22e5750, 0x75},
    {0x2326314, 0x74},  {0x236765a, 0x73},  {0x23a9130, 0x72},
    {0x23eb3a3, 0x72},  {0x242ddc1, 0x71},  {0x2470f99, 0x70},
    {0x24b4939, 0x6f},  {0x24f8ab0, 0x6e},  {0x253d40c, 0x6e},
    {0x258255b, 0x6d},  {0x25c7ead, 0x6c},  {0x260e010, 0x6b},
    {0x2654993, 0x6a},  {0x269bb46, 0x6a},  {0x26e3537, 0x69},
    {0x272b776, 0x68},  {0x2774213, 0x67},  {0x27bd51c, 0x67},
    {0x28070a1, 0x66},  {0x28514b3, 0x65},  {0x289c160, 0x64},
    {0x28e76b9, 0x64},  {0x29334cf, 0x63},  {0x297fbb0, 0x62},
    {0x29ccb6f, 0x62},  {0x2a1a41a, 0x61},  {0x2a685c3, 0x60},
    {0x2ab7079, 0x60},  {0x2b0644f, 0x5f},  {0x2b56155, 0x5e},
    {0x2ba679c, 0x5d},  {0x2bf7735, 0x5d},  {0x2c49032, 0x5c},
    {0x2c9b2a3, 0x5b},  {0x2cede9b, 0x5b},  {0x2d4142a, 0x5a},
    {0x2d95364, 0x5a},  {0x2de9c59, 0x59},  {0x2e3ef1d, 0x58},
    {0x2e94bc0, 0x58},  {0x2eeb256, 0x57},  {0x2f422f0, 0x56},
    {0x2f99da2, 0x56},  {0x2ff227e, 0x55},  {0x304b197, 0x55},
    {0x30a4b00, 0x54},  {0x30feecc, 0x53},  {0x3159d0e, 0x53},
    {0x31b55da, 0x52},  {0x3211944, 0x52},  {0x326e75e, 0x51},
    {0x32cc03e, 0x50},  {0x332a3f6, 0x50},  {0x338929b, 0x4f},
    {0x33e8c41, 0x4f},  {0x34490fd, 0x4e},  {0x34aa0e3, 0x4d},
    {0x350bc08, 0x4d},  {0x356e281, 0x4c},  {0x35d1462, 0x4c},
    {0x36351c2, 0x4b},  {0x3699ab5, 0x4b},  {0x36fef50, 0x4a},
    {0x3764faa, 0x4a},  {0x37cbbd8, 0x49},  {0x38333f0, 0x49},
    {0x389b808, 0x48},  {0x3904837, 0x48},  {0x396e492, 0x47},
    {0x39d8d31, 0x47},  {0x3a4422b, 0x46},  {0x3ab0395, 0x46},
    {0x3b1d188, 0x45},  {0x3b8ac1a, 0x45},  {0x3bf9362, 0x44},
    {0x3c6877a, 0x44},  {0x3cd8877, 0x43},  {0x3d49673, 0x43},
    {0x3dbb185, 0x42},  {0x3e2d9c5, 0x42},  {0x3ea0f4d, 0x41},
    {0x3f15234, 0x41},  {0x3f8a293, 0x40},  {0x4000084, 0x40},
    {0x4076c20, 0x3f},  {0x40ee57f, 0x3f},  {0x4166cbc, 0x3e},
    {0x41e01f0, 0x3e},  {0x425a535, 0x3e},  {0x42d56a5, 0x3d},
    {0x435165b, 0x3d},  {0x43ce471, 0x3c},  {0x444c101, 0x3c},
    {0x44cac27, 0x3b},  {0x454a5fd, 0x3b},  {0x45cae9f, 0x3a},
    {0x464c629, 0x3a},  {0x46cecb5, 0x3a},  {0x4752260, 0x39},
    {0x47d6745, 0x39},  {0x485bb82, 0x38},  {0x48e1f32, 0x38},
    {0x4969273, 0x38},  {0x49f1560, 0x37},  {0x4a7a817, 0x37},
    {0x4b04ab6, 0x36},  {0x4b8fd5a, 0x36},  {0x4c1c020, 0x36},
    {0x4ca9327, 0x35},  {0x4d3768c, 0x35},  {0x4dc6a6f, 0x34},
    {0x4e56eed, 0x34},  {0x4ee8425, 0x34},  {0x4f7aa37, 0x33},
    {0x500e142, 0x33},  {0x50a2965, 0x33},  {0x51382c0, 0x32},
    {0x51ced73, 0x32},  {0x526699e, 0x32},  {0x52ff761, 0x31},
    {0x53996dd, 0x31},  {0x5434834, 0x30},  {0x54d0b85, 0x30},
    {0x556e0f3, 0x30},  {0x560c89f, 0x2f},  {0x56ac2ab, 0x2f},
    {0x574cf38, 0x2f},  {0x57eee6a, 0x2e},  {0x5892063, 0x2e},
    {0x5936546, 0x2e},  {0x59dbd35, 0x2d},  {0x5a82855, 0x2d},
    {0x5b2a6c8, 0x2d},  {0x5bd38b3, 0x2c},  {0x5c7de39, 0x2c},
    {0x5d29780, 0x2c},  {0x5dd64ab, 0x2b},  {0x5e845e0, 0x2b},
    {0x5f33b44, 0x2b},  {0x5fe44fc, 0x2b},  {0x609632e, 0x2a},
    {0x6149600, 0x2a},  {0x61fdd98, 0x2a},  {0x62b3a1d, 0x29},
    {0x636abb5, 0x29},  {0x6423288, 0x29},  {0x64dcebd, 0x28},
    {0x659807b, 0x28},  {0x66547ec, 0x28},  {0x6712536, 0x28},
    {0x67d1882, 0x27},  {0x68921fa, 0x27},  {0x69541c6, 0x27},
    {0x6a17810, 0x26},  {0x6adc502, 0x26},  {0x6ba28c5, 0x26},
    {0x6c6a384, 0x26},  {0x6d33569, 0x25},  {0x6dfdea0, 0x25},
    {0x6ec9f54, 0x25},  {0x6f977b0, 0x25},  {0x70667e0, 0x24},
    {0x7137010, 0x24},  {0x720906e, 0x24},  {0x72dc925, 0x24},
    {0x73b1a63, 0x23},  {0x7488455, 0x23},  {0x756072a, 0x23},
    {0x763a30f, 0x23},  {0x7715833, 0x22},  {0x77f26c5, 0x22},
    {0x78d0ef4, 0x22},  {0x79b10ef, 0x22},  {0x7a92ce6, 0x21},
    {0x7b7630a, 0x21},  {0x7c5b38b, 0x21},  {0x7d41e9a, 0x21},
    {0x7e2a468, 0x20},  {0x7f14527, 0x20},  {0x8000109, 0x20},
    {0x80ed840, 0x20},  {0x81dcafe, 0x1f},  {0x82cd978, 0x1f},
    {0x83c03e0, 0x1f},  {0x84b4a6a, 0x1f},  {0x85aad4b, 0x1f},
    {0x86a2cb6, 0x1e},  {0x879c8e2, 0x1e},  {0x8898202, 0x1e},
    {0x899584e, 0x1e},  {0x8a94bfb, 0x1d},  {0x8b95d3f, 0x1d},
    {0x8c98c51, 0x1d},  {0x8d9d96a, 0x1d},  {0x8ea44c0, 0x1d},
    {0x8face8b, 0x1c},  {0x90b7704, 0x1c},  {0x91c3e65, 0x1c},
    {0x92d24e5, 0x1c},  {0x93e2ac0, 0x1c},  {0x94f502f, 0x1b},
    {0x960956c, 0x1b},  {0x971fab3, 0x1b},  {0x9838040, 0x1b},
    {0x995264d, 0x1b},  {0x9a6ed18, 0x1a},  {0x9b8d4dd, 0x1a},
    {0x9caddd9, 0x1a},  {0x9dd084b, 0x1a},  {0x9ef546f, 0x1a},
    {0xa01c284, 0x19},  {0xa1452ca, 0x19},  {0xa270580, 0x19},
    {0xa39dae6, 0x19},  {0xa4cd33b, 0x19},  {0xa5feec2, 0x19},
    {0xa732dbb, 0x18},  {0xa869067, 0x18},  {0xa9a170a, 0x18},
    {0xaadc1e6, 0x18},  {0xac1913e, 0x18},  {0xad58555, 0x18},
    {0xae99e70, 0x17},  {0xafddcd5, 0x17},  {0xb1240c6, 0x17},
    {0xb26ca8c, 0x17},  {0xb3b7a6a, 0x17},  {0xb5050aa, 0x17},
    {0xb654d90, 0x16},  {0xb7a7166, 0x16},  {0xb8fbc73, 0x16},
    {0xba52f00, 0x16},  {0xbbac957, 0x16},  {0xbd08bc1, 0x16},
    {0xbe67688, 0x15},  {0xbfc89f8, 0x15},  {0xc12c65c, 0x15},
    {0xc292c00, 0x15},  {0xc3fbb30, 0x15},  {0xc567439, 0x15},
    {0xc6d576a, 0x15},  {0xc846510, 0x14},  {0xc9b9d79, 0x14},
    {0xcb300f7, 0x14},  {0xcca8fd7, 0x14},  {0xce24a6c, 0x14},
    {0xcfa3105, 0x14},  {0xd1243f4, 0x14},  {0xd2a838d, 0x13},
    {0xd42f021, 0x13},  {0xd5b8a04, 0x13},  {0xd74518a, 0x13},
    {0xd8d4708, 0x13},  {0xda66ad2, 0x13},  {0xdbfbd40, 0x13},
    {0xdd93ea8, 0x12},  {0xdf2ef5f, 0x12},  {0xe0ccfc0, 0x12},
    {0xe26e020, 0x12},  {0xe4120db, 0x12},  {0xe5b9249, 0x12},
    {0xe7634c5, 0x12},  {0xe9108aa, 0x12},  {0xeac0e54, 0x11},
    {0xec7461e, 0x11},  {0xee2b066, 0x11},  {0xefe4d8a, 0x11},
    {0xf1a1de7, 0x11},  {0xf3621dd, 0x11},  {0xf5259cc, 0x11},
    {0xf6ec614, 0x11},  {0xf8b6716, 0x10},  {0xfa83d34, 0x10},
    {0xfc548d0, 0x10},  {0xfe28a4e, 0x10},  {0x10000211, 0x10},
    {0x101db07f, 0x10}, {0x103b95fd, 0x10}, {0x1059b2f0, 0x10},
    {0x107807c0, 0xf},  {0x109694d4, 0xf},  {0x10b55a96, 0xf},
    {0x10d4596d, 0xf},  {0x10f391c4, 0xf},  {0x11130405, 0xf},
    {0x1132b09c, 0xf},  {0x115297f5, 0xf},  {0x1172ba7e, 0xf},
    {0x119318a3, 0xf},  {0x11b3b2d4, 0x1d},
};

static inline void oscIncGet(uint16_t pitch, uint32_t *inc, uint32_t *recp) {
  const uint32_t cell_steps =
      65536 / (sizeof(table_pitch_inc) / sizeof(table_pitch_inc[0]));
  uint32_t pos = pitch / cell_steps;
  uint32_t spos = pitch & (cell_steps - 1);
  uint32_t v0 = table_pitch_inc[pos].inc;
  *recp = table_pitch_inc[pos].recp;
  int32_t diff = table_pitch_inc[pos + 1].inc - v0;
  *inc = v0 + diff * spos / (0x800000 / 1024);
}

#define GEN2_MAX_OCTAVE_OFFSET 3
#define MAX_ADC 4096
#define MAX_PWM 333
uint16_t adc[6];  // 16 bit left align
uint16_t timer;
uint16_t pwm[3];
uint32_t calib_scale = 65536 * (65536 / MAX_ADC);
int32_t calib_offset = 0;

typedef enum {
  ADC_PITCH = 0,
  ADC_OCTAVE,
  ADC_GEN1AMP,
  ADC_SYNCPHASE,
  ADC_GEN2PITCH,
  ADC_SYNC,
};

static inline int32_t pd(uint32_t inc, int32_t noise16) {
  return (inc / 65536) * noise16;
}

void vcoTap() {
  static uint32_t cycles = 0;
  static int32_t gen1;
  static int32_t gen2;

  // lcg
  static int32_t lcg;
  lcg = lcg * 1103515245 + 12345;
  int32_t lcg16 = lcg / 65536;

  // pitch calibration
  uint32_t base_pitch = adc[ADC_PITCH] * calib_scale / 65536 + calib_offset;

  uint32_t base_inc;
  uint32_t base_recp;
  oscIncGet(base_pitch, &base_inc, &base_recp);
  // gen1 core
  int32_t gen1new += base_inc;

  // gen1 octave
  // PD must be calculated for every octave
  const uint32_t oct_fade_steps = MAX_ADC / 4;
  uint32_t oct_mul = adc[ADC_OCTAVE] / oct_fade_steps;
  uint32_t oct_fade = adc[ADC_OCTAVE] & oct_fade_steps;
  // get 2 octaves of gen1 signal with PDAM
  int32_t gen1o1 = gen1new * oct_mul + pd(base_inc * oct_mul, lcg16);
  oct_mul++;
  int32_t gen1o2 = gen1new * oct_mul + pd(base_inc * oct_mul, lcg16);
  // x-fade them
  int32_t gen1full = gen1o1 / oct_fade_steps * (oct_fade_steps - 1 - oct_fade) +
                     gen1o2 / oct_fade_steps * oct_fade;
  pwm[0] =
      ((gen1full + 0x80000000) / 65536 * MAX_PWM + (uint32_t)lcg16) / 65536;
  // GEN2 is hard synced to gen1core
  uint32_t inc2 = base_inc + base_inc / (MAX_ADC / GEN2_MAX_OCTAVE_OFFSET) *
                                 adc[ADC_GEN2PITCH];
  int32_t gen2new;
  if (gen1new < gen1) {
    // sync gen2
    uint32_t adc_sync = adc[ADC_SYNC] * adc[ADC_SYNC] / MAX_ADC;
    if (adc_sync < (lcg16 * MAX_ADC / 65536)) {
      uint32_t subpos_norm = (gen1new & 0x7FFFFFFF) * base_recp;
      gen2new = (subpos_norm / 65536) * inc2 / 65536;
      // random phase on a rare events
      gen2new += lcg16 * (adc_sync * 65536 / MAX_ADC);
      // phasemod disabled, need depth knob
      gen2new += (adc[ADC_SYNCPHASE] - MAX_ADC / 2) *
                 ((int32_t)(0x100000000ULL / MAX_ADC));
    } else {
      goto nosync;
    }
  } else {
  nosync:
    gen2new = gen2 + inc2;
  }
  gen1 = gen1new;

  int32_t gen2o = gen2new + pd(gen2new - gen2, lcg16);
  gen2 = gen2new;

  int32_t adc_mix = adc[ADC_GEN1AMP] - MAX_ADC / 2;
  int32_t adc_mix_abs = adc_mix < 0 ? -adc_mix : adc_mix;
  int32_t mix = (gen2o / MAX_ADC) * (MAX_ADC - 1 - adc_mix_abs) +
                (gen1full / MAX_ADC) * adc_mix;
  pwm[1] = ((mix + 0x80000000) / 65536 * MAX_PWM + (uint32_t)lcg16) / 65536;

  if (cycles < timer)
    cycles = timer;
  else
    cycles--;
}
