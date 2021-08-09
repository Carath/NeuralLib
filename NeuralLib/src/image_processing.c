#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_processing.h"


#define THRESHOLD (1.f/512.f)


// Print in the console a grayscale image contained in a 1-dimensional array:
void printGrayscaleImage(const Number *image, int width, int height)
{
	if (image == NULL)
		return;

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			const int pos = row * width + col;

			if (image[pos] > THRESHOLD)
				printf("%5.1f", image[pos]);
			else
				printf("     ");
		}

		printf("\n");
	}

	printf("\n");
}


static inline Number maxPixel(const Number *image, int image_width,
	int kernel_width, int kernel_height, int row, int col)
{
	Number max = -1.f;

	int shift = row * image_width + col;

	for (int kernelRow = 0; kernelRow < kernel_height; ++kernelRow)
	{
		for (int kernelCol = 0; kernelCol < kernel_width; ++kernelCol)
		{
			Number grey = image[shift + kernelCol];
			max = grey > max ? grey : max;
		}

		shift += image_width;
	}

	return max;
}


static inline Number averagePixel(const Number *image, int image_width,
	int kernel_width, int kernel_height, int row, int col)
{
	Number sum = 0;

	const int kernel_pixels = kernel_width * kernel_height;
	int shift = row * image_width + col;

	for (int kernelRow = 0; kernelRow < kernel_height; ++kernelRow)
	{
		for (int kernelCol = 0; kernelCol < kernel_width; ++kernelCol)
			sum += image[shift + kernelCol];

		shift += image_width;
	}

	return sum / kernel_pixels;
}


void pooling(Number *output, const Number *input, int output_width, int output_height,
	int input_width, int input_height, PoolingMode poolmeth)
{
	if (output == NULL || input == NULL)
	{
		printf("\nNULL input or output passed for pooling.\n\n");
		return;
	}

	if (input_width < output_width || input_height < output_height)
	{
		printf("\nInput sizes lower than output sizes! Widths: %d vs %d, heights: %d vs %d.\n\n",
			input_width, output_width, input_height, output_height);
		return;
	}

	if (input_width % output_width != 0 || input_height % output_height != 0)
	{
		printf("\nOutput sizes do not divide input sizes! Widths: %d vs %d, heights: %d vs %d.\n\n",
			input_width, output_width, input_height, output_height);
		return;
	}

	const int kernel_height = input_height / output_height;
	const int kernel_width = input_width / output_width;

	// printf("\nkernel_height = %d\nkernel_width = %d\n", kernel_height, kernel_width);

	if (poolmeth == MAX_POOLING)
	{
		for (int row = 0; row < output_height; ++row)
		{
			for (int col = 0; col < output_width; ++col)
			{
				output[row * output_width + col] = maxPixel(input, input_width, kernel_width, kernel_height,
					row * kernel_height, col * kernel_width);
			}
		}
	}

	else // AVERAGE_POOLING
	{
		for (int row = 0; row < output_height; ++row)
		{
			for (int col = 0; col < output_width; ++col)
			{
				output[row * output_width + col] = averagePixel(input, input_width, kernel_width, kernel_height,
					row * kernel_height, col * kernel_width);
			}
		}
	}
}


// Find the hull of the given image, assuming it has a white background:
void find_hull(const Number *image, int width, int height, Box *box)
{
	if (! image || ! box)
	{
		printf("\nCannot find the hull of a NULL image or box.\n\n");
		return;
	}

	// starting from the opposite edges:
	int rowMin = height - 1, rowMax = 0;
	int colMin = width - 1, colMax = 0;

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			if (image[row * width + col] > THRESHOLD)
			{
				rowMin = row < rowMin ? row : rowMin;
				rowMax = row > rowMax ? row : rowMax;
				colMin = col < colMin ? col : colMin;
				colMax = col > colMax ? col : colMax;
			}
		}
	}

	box -> rowMin = rowMin;
	box -> rowMax = rowMax;
	box -> colMin = colMin;
	box -> colMax = colMax;

	// printf("rowMin = %d\nrowMax = %d\ncolMin = %d\ncolMax = %d\n", rowMin, rowMax, colMin, colMax);
}


// 'src' and 'dest' must not overlap!
void recenter(Number *dest, const Number *src, int width, int height)
{
	if (dest == NULL || src == NULL)
	{
		printf("\nNULL src or dest passed for recentering.\n\n");
		return;
	}

	Box box = {0};
	find_hull(src, width, height, &box);

	const int delta_row = (height - box.rowMax - box.rowMin) / 2;
	const int delta_col = (width - box.colMax - box.colMin) / 2;

	slide(dest, src, width, height, delta_row, delta_col);
}


// 'src' and 'dest' must not overlap!
void slide(Number *dest, const Number *src, int width, int height, int delta_row, int delta_col)
{
	if (dest == NULL || src == NULL)
	{
		printf("\nNULL src or dest passed for sliding.\n\n");
		return;
	}

	const int shift = delta_row * width + delta_col;
	const int rowMin = MAX(0, -delta_row), rowMax = height + MIN(0, -delta_row);
	const int colMin = MAX(0, -delta_col), colMax = width + MIN(0, -delta_col);

	// Resetting dest:
	memset(dest, 0, width * height * sizeof(Number));

	for (int row = rowMin; row < rowMax; ++row)
	{
		for (int col = colMin; col < colMax; ++col)
		{
			const int old_pos = row * width + col;
			dest[shift + old_pos] = src[old_pos];
		}
	}
}


// Resize and recenter the image, with the given margin ratio, which must be in ]0, 1[.
// 'src' and 'dest' must not overlap!
void resize(Number *dest, const Number *src, int width, int height, float marginRatio, UpscalingOption option)
{
	Box box = {0};
	find_hull(src, width, height, &box);

	int boxWidth = box.colMax - box.colMin, boxHeight = box.rowMax - box.rowMin;
	int imageDim = MIN(width, height), boxDim = MAX(boxWidth, boxHeight);
	float scale = boxDim == 0 ? 1.f : (1.f - 2.f * marginRatio) * imageDim / boxDim;
	float offsetRow = (height - scale * (box.rowMin + box.rowMax)) / 2.f;
	float offsetCol = (width - scale * (box.colMin + box.colMax)) / 2.f;

	printf("\n-> scale: %.3f\n", scale);

	// Resetting dest:
	memset(dest, 0, width * height * sizeof(Number));

	for (int row = box.rowMin; row < box.rowMax; ++row)
	{
		for (int col = box.colMin; col < box.colMax; ++col)
		{
			const int old_pos = row * width + col;
			const int new_row = (int) (scale * row + offsetRow + 0.5f);
			const int new_col = (int) (scale * col + offsetCol + 0.5f);

			int newDotSize = (int) (scale + 0.5f);
			newDotSize = MAX(1, newDotSize);

			const int sq_rowMin = MAX(0, new_row - newDotSize / 2);
			const int sq_colMin = MAX(0, new_col - newDotSize / 2);
			const int sq_rowMax = MIN(height, new_row + (newDotSize + 1) / 2);
			const int sq_colMax = MIN(width, new_col + (newDotSize + 1) / 2);

			for (int sq_row = sq_rowMin; sq_row < sq_rowMax; ++sq_row)
			{
				for (int sq_col = sq_colMin; sq_col < sq_colMax; ++sq_col)
				{
					const int pos = sq_row * width + sq_col;

					if (option == PIXEL_MAX) {
						dest[pos] = MAX(dest[pos], src[old_pos]);
					}
					else { // PIXEL_SUM: Summing with transparency - less stable:
						int d1_dist = abs(sq_row - new_row) + abs(sq_col - new_col);
						float alpha = 1.f - (float) d1_dist / newDotSize * 0.5f; // 0.5f arbitrary.
						dest[pos] += src[old_pos] * alpha;
						dest[pos] = MIN(1.f, dest[pos]);
					}
				}
			}
		}
	}
}
