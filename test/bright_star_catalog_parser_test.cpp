#include <gtest/gtest.h>
#include <type_traits>
#include "rapidcsv.h"
#include <numbers>

#include "bsc/bright_star_catalog_parser.h"

namespace {
	double toRadians(const double degrees) {
		return degrees * (std::numbers::pi / 180.0);
	}
}

TEST(BrightStartCatalogV5BinaryParserTest, HeaderStructureShouldHas28Bytes) {
	// see the specification http://tdc-www.harvard.edu/catalogs/bsc5.header.html
	ASSERT_EQ(28, sizeof(BrightStarCatalog::v5::Header));
}

TEST(BrightStartCatalogV5BinaryParserTest, HeaderStructureShouldHasStandardLayout) {
	const auto isStandardLayout = std::is_standard_layout<BrightStarCatalog::v5::Header>::value;
	ASSERT_TRUE(isStandardLayout);
}

TEST(BrightStartCatalogV5BinaryParserTest, HeaderStructureShouldBeTrivial) {
	const auto isTrivial = std::is_trivial<BrightStarCatalog::v5::Header>::value;
	ASSERT_TRUE(isTrivial);
}

TEST(BrightStartCatalogV5BinaryParserTest, EntryStructureShouldHas32Bytes) {
	// @formatter:off
	const size_t sizeofXNO 		= sizeof(BrightStarCatalog::v5::Entry::XNO);
	const size_t sizeofSRA0 	= sizeof(BrightStarCatalog::v5::Entry::SRA0);
	const size_t sizeofSDEC0 	= sizeof(BrightStarCatalog::v5::Entry::SDEC0);
	const size_t sizeofIS 		= sizeof(BrightStarCatalog::v5::Entry::IS);
	const size_t sizeofMAG 		= sizeof(BrightStarCatalog::v5::Entry::MAG);
	const size_t sizeofXRPM 	= sizeof(BrightStarCatalog::v5::Entry::XRPM);
	const size_t sizeofXDPM 	= sizeof(BrightStarCatalog::v5::Entry::XDPM);

	ASSERT_EQ( 4, sizeofXNO);
	ASSERT_EQ( 8, sizeofSRA0);
	ASSERT_EQ( 8, sizeofSDEC0);
	ASSERT_EQ( 2, sizeofIS);
	ASSERT_EQ( 2, sizeofMAG);
	ASSERT_EQ( 4, sizeofXRPM);
	ASSERT_EQ( 4, sizeofXDPM);
	ASSERT_EQ(32, sizeofXNO + sizeofSRA0 + sizeofSDEC0 + sizeofIS + sizeofMAG + sizeofXRPM + sizeofXDPM);
	// might be > 32 due to padding
	ASSERT_TRUE(sizeof(BrightStarCatalog::v5::Entry) >= 32);
	// @formatter:on
}

TEST(BrightStartCatalogV5BinaryParserTest, EntryStructureShouldHasStandardLayout) {
	const auto isStandardLayout = std::is_standard_layout<BrightStarCatalog::v5::Entry>::value;
	ASSERT_TRUE(isStandardLayout);
}

TEST(BrightStartCatalogV5BinaryParserTest, EntryStructureShouldBeTrivial) {
	const auto isTrivial = std::is_trivial<BrightStarCatalog::v5::Entry>::value;
	ASSERT_TRUE(isTrivial);
}

TEST(BrightStartCatalogV5BinaryParserTest, ParsingResultStructureShouldHasStandardLayout) {
	const auto isStandardLayout = std::is_standard_layout<BrightStarCatalog::v5::ParsingResult>::value;
	ASSERT_TRUE(isStandardLayout);
}

TEST(BrightStartCatalogV5BinaryParserTest, ShouldParseHeaderProperly) {
	// Source: http://tdc-www.harvard.edu/catalogs/bsc5.header.html
	BrightStarCatalog::v5::Header header = BrightStarCatalog::v5::parse().header;
	ASSERT_EQ(0, header.STAR0);
	ASSERT_EQ(1, header.STAR1);
	ASSERT_TRUE((9'110 == header.STARN) || (-9'110 == header.STARN));
	ASSERT_EQ(1, header.STNUM);
	ASSERT_EQ(1, header.MPROP);
	ASSERT_TRUE((1 == header.NMAG) || (-1 == header.NMAG));
	ASSERT_EQ(32, header.NBENT);
}

TEST(BrightStartCatalogV5BinaryParserTest, ShouldParseEntiresProperly) {
	const rapidcsv::Document document(
			"./res/expected_values.csv",
			rapidcsv::LabelParams(0, -1),
			rapidcsv::SeparatorParams(';')
	);
	std::vector<double> rightAscensionInDegrees = document.GetColumn<double>(0);
	std::vector<double> declinationInDegrees = document.GetColumn<double>(1);
	std::vector<int> hr = document.GetColumn<int>(2);
	std::vector<float> magnitude = document.GetColumn<float>(3);

	const size_t rowCount = document.GetRowCount();
	// Precondition: Test data were passed correctly
	ASSERT_EQ(9'096, rowCount);

	std::vector<BrightStarCatalog::v5::Entry> entries = BrightStarCatalog::v5::parse().entries;


	// 9,100 entries were parsed
	ASSERT_EQ(9'110, entries.size());
	const float deltaMagnitude = 1e-3;
	const double deltaPosition = 1e-7;
	for (size_t i = 0, j = 0; i < entries.size(); ++i) {
		// Ignored the 14 non-stellar objects which have received HR numbers:
		// HR 92; 95; 182; 1,057; 1,841; 2,472; 2,496; 3,515; 3,671; 6,309; 6,515; 7,189; 7,539 and 8,296.
		switch (i) {
			// @formatter:off
			case    91:
			case    94:
			case   181:
			case 1'056:
			case 1'840:
			case 2'471:
			case 2'495:
			case 3'514:
			case 3'670:
			case 6'308:
			case 6'514:
			case 7'188:
			case 7'538:
			case 8'295:
			// @formatter:on
				ASSERT_EQ(i + 1, entries[i].XNO);
				continue;
			default:
				ASSERT_EQ(hr[j], entries[i].XNO);
				EXPECT_NEAR(magnitude[j], entries[i].MAG * 0.01f, deltaMagnitude);
				EXPECT_NEAR(toRadians(rightAscensionInDegrees[j]), entries[i].SRA0, deltaPosition);
				EXPECT_NEAR(toRadians(declinationInDegrees[j]), entries[i].SDEC0, deltaPosition);
				++j;
		}
	}

	// Expected values from: http://tdc-www.harvard.edu/catalogs/bsc5.entry.html
	// @formatter:off
	ASSERT_EQ(6.70, entries[0].MAG * 0.01);
	ASSERT_EQ('A', 	entries[0].IS[0]);
	ASSERT_EQ('1', 	entries[0].IS[1]);

	ASSERT_EQ(4.50, entries[9'075].MAG * 0.01);
	ASSERT_EQ('B', 	entries[9'075].IS[0]);
	ASSERT_EQ('9', 	entries[9'075].IS[1]);

	ASSERT_EQ(6.29, entries[9'076].MAG * 0.01);
	ASSERT_EQ('G', 	entries[9'076].IS[0]);
	ASSERT_EQ('3', 	entries[9'076].IS[1]);

	ASSERT_EQ(5.80, entries[9'109].MAG * 0.01);
	ASSERT_EQ('B', 	entries[9'109].IS[0]);
	ASSERT_EQ('8', 	entries[9'109].IS[1]);
	// @formatter:on
}

TEST(BrightStartCatalogV5BinaryParserTest, ShouldThrowExceptionIfUnknownFilepath) {
	EXPECT_THROW(BrightStarCatalog::v5::parse("not_exist_file.bin"), std::exception);
}

