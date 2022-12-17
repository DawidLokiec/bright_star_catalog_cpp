#ifndef BRIGHT_START_CATALOG_PARSER_H
#define BRIGHT_START_CATALOG_PARSER_H

#include <string>
#include <vector>

/**
 * This namespace declares the scope of the functions belonging to the current library Bright Star Catalog 5 for C++.
 *
 * @see http://tdc-www.harvard.edu/catalogs/bsc5.html
 */
namespace BrightStarCatalog::v5 {

	/**
	 * The header of the Bright Star Catalog (BSC) 5.
	 * The catalog header tells the program what to expect in each.
	 * The catalog header is 28 bytes in size.
	 *
	 * @see http://tdc-www.harvard.edu/catalogs/bsc5.header.html
	 * @see http://tdc-www.harvard.edu/catalogs/catalogsb.html
	 */
	struct Header {
		/**
		 * Subtract this number to get the sequence number. Expected to be 0.
		 */
		int32_t STAR0;

		/**
		 * The first star number in file. Expected to be 1.
		 */
		int32_t STAR1;

		/**
		 * Number of stars in file. If negative, coordinates are J2000 instead of B1950.
		 * Expected to be 9,110 or -9,100.
		 */
		int32_t STARN;

		/**
		 *  - 0 if no star ID numbers are present.
		 *  - 1 if star ID numbers are in catalog file.
		 *  - 2 if star ID numbers are in file.
		 *  Expected to be 1.
		 *
		 */
		int32_t STNUM;

		/**
		 * - 0 if no proper motion is included.
		 * - 1 if proper motion is included.
		 * - 2 if radial velocity is included.
		 * Expected to be 1.
		 */
		int32_t MPROP;

		/**
		 * The data are expected to be in B1950 equatorial coordinates unless NMAG is negated.
		 * Number of magnitudes present (-1=J2000 instead of B1950=1).
		 * Number of magnitudes present (0-10).
		 */
		int32_t NMAG;

		/**
		 * Number of bytes per star entry. Expected to be 32.
		 */
		int32_t NBENT;
	};

	/**
	 * Each catalog entry in BSC5 contains 32 bytes.
	 *
	 * @see http://tdc-www.harvard.edu/catalogs/bsc5.entry.html
	 * @see http://tdc-www.harvard.edu/catalogs/catalogsb.html
	 */
	struct Entry {
		/**
		 * The Catalog number of star.
		 * Must be 4 bytes in size!
		 */
		float XNO;

		/**
		 * The B1950 Right Ascension (radians).
		 *
		 * Must be 8 bytes in size!
		 */
		double SRA0;

		/**
		 * The B1950 Declination (radians).
		 * Must be 8 bytes in size!
		 */
		double SDEC0;

		/**
		 * The Spectral type (2 characters).
		 * Must be 2 bytes in size!
		 */
		char IS[2];

		/**
		 * V Magnitude * 100 [0-10 may be present]
		 * Must be 2 bytes in size!
		 */
		int16_t MAG;

		/**
		 * R.A. proper motion (radians per year).
		 * Must be 4 bytes in size!
		 */
		float XRPM;

		/**
		 * Dec. proper motion (radians per year)
		 * Must be 4 bytes in size!
		 */
		float XDPM;
	};

	/**
	 * The parsing result.
	 */
	struct ParsingResult {
		/**
		 * The parsed header of the star catalog in the binary format.
		 *
		 * @see http://tdc-www.harvard.edu/catalogs/bsc5.header.html
		 */
		Header header;

		/**
		 * The parsed entries of the star catalog in the binary format.
		 *
		 * @see http://tdc-www.harvard.edu/catalogs/bsc5.entry.html
		 */
		std::vector<Entry> entries;
	};

	/**
 	 * @brief Parses the star catalog from http://tdc-www.harvard.edu/catalogs/bsc5.html.
 	 * This function takes little and big endian byte order into account.
 	 * @return the parsing result.
 	 */
	ParsingResult parse();

	/**
	 * Parses the star catalogs in binary format from http://tdc-www.harvard.edu/catalogs/bsc5.html.
	 * @param filepath the path to the star catalog to parse.
	 * @return the parsing result.
	 */
	ParsingResult parse(const std::string &filepath);
}

#endif //BRIGHT_START_CATALOG_PARSER_H
