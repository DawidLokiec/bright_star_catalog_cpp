#include "bsc/bright_star_catalog_parser.h"
#include "binary_reader.h"
#include "config.h"

#include <bit>
#include <cmath>
#include <stdexcept>

// false positive warnings, because on another user's host the byte order might not be the same
BrightStarCatalog::v5::ParsingResult BrightStarCatalog::v5::parse() {
	const bool littleEndianPlatform = std::endian::native == std::endian::little;
	const bool bigEndianPlatform = std::endian::native == std::endian::big;
	if constexpr (littleEndianPlatform) {
		return parse(std::string(RESOURCES_FOLDER_PATH) + "bright_star_catalog/v5/bsc5_binary_little_endian.bin");
	} else if constexpr (bigEndianPlatform) {
		return parse(std::string(RESOURCES_FOLDER_PATH) + "bright_star_catalog/v5/bsc5_binary_big_endian.bin");
	} else {
		static_assert(
				littleEndianPlatform || bigEndianPlatform,
				"This function does not support the native byte order"
		);
	}
}

BrightStarCatalog::v5::ParsingResult BrightStarCatalog::v5::parse(const std::string &filepath) {

	BinaryFileReader binaryReader = BinaryFileReader::createBinaryFileReaderByBinaryFile(filepath);

	Header header{};
	binaryReader.readNextBytesInto(&header, sizeof(header));

	// abs, because number of stars may be negative according to the binary format specification !!!
	const size_t numEntries = std::abs(header.STARN);
	std::vector<Entry> entries(numEntries);
	const bool starIdInCatalogAvailable = (1 == header.STNUM) || (2 == header.STNUM);
	const bool starProperMotionInCatalogAvailable = (1 == header.MPROP) || (2 == header.MPROP);
	for (size_t i = 0; i < numEntries; ++i) {
		Entry entry{};
		if (starIdInCatalogAvailable) {
			binaryReader.readNextBytesInto(&entry.XNO, sizeof(entry.XNO));
		}
		binaryReader
				.readNextBytesInto(&entry.SRA0, sizeof(entry.SRA0))
				.readNextBytesInto(&entry.SDEC0, sizeof(entry.SDEC0))
				.readNextBytesInto(&entry.IS, sizeof(entry.IS))
				.readNextBytesInto(&entry.MAG, sizeof(entry.MAG));
		if (starProperMotionInCatalogAvailable) {
			binaryReader
					.readNextBytesInto(&entry.XRPM, sizeof(entry.XRPM))
					.readNextBytesInto(&entry.XDPM, sizeof(entry.XDPM));
		}
		entries[i] = entry;
	}

	return {header, entries};
}
