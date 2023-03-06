#include <iostream>
#include <stdexcept>

#include "binary_reader.h"

using namespace BrightStarCatalog;

BinaryFileReader::BinaryFileReader(const std::string &filepath) : binaryInputStream() {
	// Let it crash
	binaryInputStream.exceptions(std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit);
	binaryInputStream.open(filepath, std::ios::binary);
}

BinaryFileReader BinaryFileReader::createBinaryFileReaderByBinaryFile(const std::string &filepath) {
	return BinaryFileReader(filepath);
}

BinaryFileReader &BinaryFileReader::readNextBytesInto(void *destinationAddress, const long long numBytes) {
	if (0 < numBytes) {
		binaryInputStream.read(reinterpret_cast<char *>(destinationAddress), numBytes);
		return *this;
	} else {
		throw std::invalid_argument("The number of bytes to read must be positive!");
	}
}

BinaryFileReader::~BinaryFileReader() {
	if (binaryInputStream.is_open()) {
		binaryInputStream.close();
	}
}
