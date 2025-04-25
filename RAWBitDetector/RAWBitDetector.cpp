/*
25 april 2025

Maximov Evgeny
9890175@mail.ru


RAW depth bit calculator
*/

#ifdef WIN32

	#include "libraw/libraw.h"

	#define LIBRAW_WIN32_CALLS	1

	#ifndef LIBRAW_WIN32_CALLS
		#include <netinet/in.h>
	#else
		#include <winsock2.h>
	#endif

	#ifdef LIBRAW_WIN32_CALLS
		#define snprintf _snprintf
	#endif
#else
	#include <libraw/libraw.h>
#endif

#include <iostream>
#include <stdio.h>

#include "classMinMax.h"

#include <math.h>

unsigned int GetCoord(unsigned int x, unsigned int y, unsigned int width)
{
	return x + y * width;
}

int CalculateBitDepth(int max_value)
{
	int counter = 0;

	while (pow(2, counter)<max_value)
	{
		counter++;

		if (counter > 32)
		{
			return -1;
		}
	}
	return counter;
}

void PrintMessage(int message_id)
{
	switch (message_id)
	{
		default:
		{
			printf("Error message id\n");
		}break;
	case 1:
	{
		printf("Open file error\n");
	}break;
	case 2:
	{
		printf("Unpack error\n");
	}break;
	}
}

int main(int arg_count, char* arg[])
{
	if (arg_count == 1)
	{
		printf("No input file(s)");
		//getchar();
		return 0;
	}
	else
	{
		LibRaw RawConverter;

		int LibRAW_Result = 0;

		int processing_state = 0;

		if (arg_count > 1)
		{
			processing_state = 1;
			LibRAW_Result = RawConverter.open_file(arg[1]);
			if (LibRAW_Result == LIBRAW_SUCCESS)
			{
				processing_state = 2;
				LibRAW_Result = RawConverter.unpack();
				if (LibRAW_Result == LIBRAW_SUCCESS)
				{
					RawConverter.raw2image();

					classMinMax<unsigned int> channels_value[4];

					int coord_raw = 0;

					unsigned int raw_bitmap_size = RawConverter.imgdata.sizes.height * RawConverter.imgdata.sizes.width;

					for (unsigned int x = 0; x < RawConverter.imgdata.sizes.width; x++)
					{
						for (unsigned int y = 0; y < RawConverter.imgdata.sizes.height; y++)
						{
							coord_raw = GetCoord(x, y, RawConverter.imgdata.sizes.width);

							if (x % 2)
							{
								if (y % 2)
								{
									// B channel
									channels_value[2].Calculate(RawConverter.imgdata.image[coord_raw][2]);
								}
								else
								{
									// G1 channel
									channels_value[1].Calculate(RawConverter.imgdata.image[coord_raw][1]);
								}
							}
							else
							{
								if (y % 2)
								{
									// G2 channel
									channels_value[3].Calculate(RawConverter.imgdata.image[coord_raw][3]);
								}
								else
								{
									// R channel
									channels_value[0].Calculate(RawConverter.imgdata.image[coord_raw][0]);
								}
							}
						}
					}

					printf("R channel min: %i, max: %i, bits: %i\n", channels_value[0].GetMinValue(), channels_value[0].GetMaxValue(), CalculateBitDepth(channels_value[0].GetMaxValue()));
					printf("G1 channel min: %i, max: %i, bits: %i\n", channels_value[1].GetMinValue(), channels_value[1].GetMaxValue(), CalculateBitDepth(channels_value[1].GetMaxValue()));
					printf("B channel min: %i, max: %i, bits: %i\n", channels_value[2].GetMinValue(), channels_value[2].GetMaxValue(), CalculateBitDepth(channels_value[2].GetMaxValue()));
					printf("G2 channel min: %i, max: %i, bits: %i\n", channels_value[3].GetMinValue(), channels_value[3].GetMaxValue(), CalculateBitDepth(channels_value[3].GetMaxValue()));
					
					

				}
				else
				{
					PrintMessage(processing_state);
				}
			}
			else
			{
				PrintMessage(processing_state);
				printf("Libraw error: %s\n", libraw_strerror(LibRAW_Result));
			}
		}
		else
		{
			PrintMessage(processing_state);
		}

		RawConverter.recycle();
		getchar();
		return 0;
	}
}
