# Components

## How to install

1. Include the `/Components/` folder in your project.
	- You can place the folder anywhere in your project that works best for you.
	- You may remove the files from the `/Components/` folder and place them in another folder.
	- You may separate the files into different folder such. For example: `/src/Components.cpp` and `/inc/Components.h`.
	- As long as all the files are included and linked correctly, you can implement components into your project however you like and everything should work fine.

2. Implement the following `typedefs` if not already implemented in your project:
	- `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`, `int8_t`, `int16_t`, `int32_t`, and `int64_t`
	- Make sure to include any header files containing these `typedefs` into each `Components.*` file so that they can access them.