/*
 * BGRAVideoFrame.h
 *
 *  Created on: 21 Nis 2013
 *      Author: oulcan
 */

#ifndef BGRAVIDEOFRAME_H_
#define BGRAVIDEOFRAME_H_

#include <cstddef>

// A helper struct presenting interleaved BGRA image in memory.
struct BGRAVideoFrame {
	size_t width;
	size_t height;
	size_t stride;

	unsigned char * data;
};

#endif /* BGRAVIDEOFRAME_H_ */
