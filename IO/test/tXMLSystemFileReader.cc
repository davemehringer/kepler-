
#include <gtest/gtest.h>
#include <IO/XMLSystemFileReader.h>

#include <chrono>

using namespace kepler;

TEST(XMLSystemFileReader, Constructors) {
	XMLSystemFileReader reader ("acen.xml");
	auto system = reader.getSystem();
	ASSERT_EQ(0, system->getTime(SECOND));
	ASSERT_EQ(3, system->getNumberOfBodies());
	auto bodies = system->getBodies();
	for (const auto& body: bodies) {
	    cout << body << endl;
	}

}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


