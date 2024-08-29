#include <gtest/gtest.h>
#include "graph.h"
#include "dataset.h"
#include "causalDiscovery.h"
#include "CSVReader.h"

class CausalDiscoveryTest : public ::testing::Test
{
protected:
    CausalDiscoveryTest()
    {
    }

    ~CausalDiscoveryTest() override
    {
    }

    void SetUp() override
    {
    }

    void TearDown() override
    {
    }
};

TEST_F(CausalDiscoveryTest, UseVehicleDataSet)
{
    /*
    Columns:
    hengerûrtartalom	teljesítmény	Elhaladási zaj [dB(A)]	CO2 kibocsátás [g/km] (V.7.)
    */

    auto columns = CSVReader::readCSVFile("KV-41762_202301_test.csv", 4);
    auto data = std::make_shared<Dataset>(std::move(columns));
    auto graph = std::make_shared<Graph>(data);

    double alpha = 0.05;
    CausalDiscovery fci;
    fci.runFCI(graph, alpha);
    graph->printGraph();

    auto expectedGraph = std::make_shared<Graph>(data);
    expectedGraph->addDirectedEdge(0,1);
    expectedGraph->addDirectedEdge(0,2);
    expectedGraph->addDirectedEdge(0,3);
    expectedGraph->addDirectedEdge(1,3);
    expectedGraph->addDirectedEdge(2,3);

    ASSERT_EQ(graph, expectedGraph) << "FCI algorithm should produce the expected graph structure";
}

TEST_F(CausalDiscoveryTest, SmokeTestFCIWithLargerDataset1)
{
    auto data = std::make_shared<Dataset>(std::vector<Column>{
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
        { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 },
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 },
        { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 },
        { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 },
        { 1, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38 },
        { 38, 36, 34, 32, 30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0 },
        { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 },
        { 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48 },
        { 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 },
        { 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },
        { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40 }
    });

    auto graph = std::make_shared<Graph>(data);

    double alpha = 0.05;

    CausalDiscovery fci;
    fci.runFCI(graph, alpha);

    auto expectedGraph = std::make_shared<Graph>(data);

    expectedGraph->addDirectedEdge(0, 2);
    expectedGraph->addDirectedEdge(0, 11);

    expectedGraph->addDirectedEdge(1, 7);
    expectedGraph->addDirectedEdge(1, 8);

    expectedGraph->addDirectedEdge(2, 11);

    expectedGraph->addDirectedEdge(3, 8);
    expectedGraph->addDirectedEdge(3, 9);

    expectedGraph->addDirectedEdge(6, 10);

    expectedGraph->addDirectedEdge(8, 9);

    //expectedGraph->compareGraphs(*graph);

    ASSERT_EQ(graph, expectedGraph) << "FCI algorithm should produce the expected graph structure";
}

// TODO: it fails, but shouldn't
TEST_F(CausalDiscoveryTest, DISABLED_SmokeTestFCIWithLargerDataset2)
{
    auto data = std::make_shared<Dataset>(std::vector<Column>{
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 },  // col_0
        { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21 }, // col_1
        { 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5, 11.5, 12.5, 13.5, 14.5, 15.5, 16.5, 17.5, 18.5, 19.5, 20.5 }, // col_2
        { 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60 }, // col_3
        { 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43 }, // col_4
        { 8, 6, 4, 2, 0, -2, -4, -6, -8, -10, -12, -14, -16, -18, -20, -22, -24, -26, -28, -30 }, // col_5
        { 0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5, 11.5, 12.5, 13.5, 14.5, 15.5, 16.5, 17.5, 18.5, 19.5 }, // col_6
        { 2, 1, 0, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16, -17 }, // col_7
        { 1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 33, 35, 37, 39 }, // col_8
        { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40 }, // col_9
        { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5, -6, -7, -8, -9 }, // col_10
        { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288 }, // col_11
        { 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81 }, // col_12
        { 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 8.8, 9.9, 11, 12.1, 13.2, 14.3, 15.4, 16.5, 17.6, 18.7, 19.8, 20.9, 22 }, // col_13
        { 1.05, 2.1, 3.15, 4.2, 5.25, 6.3, 7.35, 8.4, 9.45, 10.5, 11.55, 12.6, 13.65, 14.7, 15.75, 16.8, 17.85, 18.9, 19.95, 21 } // col_14
    });

    auto graph = std::make_shared<Graph>(data);

    // Significance level
    double alpha = 0.05;

    CausalDiscovery fci;
    fci.runFCI(graph, alpha);

    auto expectedGraph = std::make_shared<Graph>(data);
    expectedGraph->addDirectedEdge(0, 1);
    expectedGraph->addDirectedEdge(0, 2);
    expectedGraph->addDirectedEdge(0, 3);
    expectedGraph->addDirectedEdge(0, 6);
    expectedGraph->addDirectedEdge(0, 9);
    expectedGraph->addDirectedEdge(0, 13);
    expectedGraph->addDirectedEdge(0, 14);
    expectedGraph->addDirectedEdge(1, 7);
    expectedGraph->addDirectedEdge(2, 6);
    expectedGraph->addDirectedEdge(5, 10);

    expectedGraph->compareGraphs(*graph);

    ASSERT_EQ(graph, expectedGraph) << "FCI algorithm should produce the expected graph structure";
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
