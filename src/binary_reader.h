#ifndef BRIGHT_STAR_CATALOG_CPP_BINARY_READER_H
#define BRIGHT_STAR_CATALOG_CPP_BINARY_READER_H

#include <fstream> // to read binary files
#include <string>  // to specify the path of a binary file

/**
 * @brief This namespace declares the scope of the functions belonging to the current library Bright Star Catalog for C++.
 */
namespace BrightStarCatalog {

	/**
	 * @brief A binary file reader.
	 */
	class BinaryFileReader {
		private:
			/**
			 * @brief The binary input stream of the current binary file reader.
			 */
			std::ifstream binaryInputStream;

			/**
			 * @brief The parametrized constructor.
			 * @details Creates an new instance of this class by the passed arguments.
			 * @param filepath the path of the binary file to read.
			 */
			explicit BinaryFileReader(const std::string &filepath);

		public:
			/**
			 * @brief The destructor.
			 */
			~BinaryFileReader();

			/**
			 * @brief Creates a binary file reader for the binary file specified by the passed path.
			 * @param filepath the filepath of the binary file to be read by the created reader.
			 * @return a binary file reader for the binary file specified by the passed path.
			 */
			static BinaryFileReader createBinaryFileReaderByBinaryFile(const std::string &filepath);

			/**
			 * @brief Reads the next number of bytes into the passed destination address.
			 * @param destinationAddress the destination address to read into the bytes.
			 * @param numBytes the number of bytes to read.
			 * @return a reference to the current instance of the binary file reader.
			 * @throws std::invalid_argument if the passed number of bytes to read is lower than 1.
			 */
			BinaryFileReader &readNextBytesInto(void *destinationAddress, long long numBytes);
	};
}

#endif //BRIGHT_STAR_CATALOG_CPP_BINARY_READER_H
