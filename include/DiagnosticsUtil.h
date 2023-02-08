#pragma once

#include "Definitions.h"

#include <implot.h>

struct ScrollingBuffer {
	int max_size;
	int offset;
	containers::DynamicArray<ImVec2> Data;

	ScrollingBuffer(int size = 100) {
		max_size = size;
		offset = 0;
		Data.reserve(max_size);
	}

	void AddPoint(float x, float y) {
		if (Data.size() < max_size)
			Data.push_back(ImVec2(x, y));
		else {
			Data[offset] = ImVec2(x, y);
			offset = (offset + 1) % max_size;
		}
	}

	void Erase() {
		if (Data.size() > 0) {
			Data.clear();
			offset = 0;
		}
	}
};
