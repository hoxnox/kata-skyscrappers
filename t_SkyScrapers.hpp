#include "SkyScrapers.hpp"

class TestSkyScrapers : public ::testing::Test
{
protected:
    int equal(int **puzzle, int expected[4][4]) {
        if (!puzzle || !expected) return 0;
        for (int i = 0; i < 4; ++i) if (memcmp (puzzle[i], expected[i], 4*sizeof (int))) return 0;
        return 1;
    }
    int equal(int **puzzle, int expected[6][6]) {
        if (!puzzle || !expected) return 0;
        for (int i = 0; i < 6; ++i) if (memcmp (puzzle[i], expected[i], 6*sizeof (int))) return 0;
        return 1;
    }
    int equal(int **puzzle, int expected[7][7]) {
        if (!puzzle || !expected) return 0;
        for (int i = 0; i < 7; ++i) if (memcmp (puzzle[i], expected[i], 7*sizeof (int))) return 0;
        return 1;
    }
    SkyScrapers<4> ss;
    SkyScrapers<6> ss6;
    SkyScrapers<7> ss7;
};


TEST_F(TestSkyScrapers, print_4x4)
{
    std::string empty = "             \n"
                        " +---------+ \n"
                        " | 0 0 0 0 | \n"
                        " | 0 0 0 0 | \n"
                        " | 0 0 0 0 | \n"
                        " | 0 0 0 0 | \n"
                        " +---------+ \n"
                        "             \n";
    EXPECT_EQ(empty, print<4>(ss.GetLastMatrix()));
}

TEST_F(TestSkyScrapers, fullfilled_4x4)
{
    std::array<int, 16> clues = { 2, 2, 1, 3, 2, 2, 3, 1, 1, 2, 2, 3, 3, 2, 1, 3 };
    int etalon[4][4] = { { 1, 3, 4, 2 },
                         { 4, 2, 1, 3 },
                         { 3, 4, 2, 1 },
                         { 2, 1, 3, 4 } };
    EXPECT_EQ(1, equal(ss.Solve(clues.data()), etalon)) << print<4>(ss.GetLastMatrix(), clues);
}

TEST_F(TestSkyScrapers, happy1_4x4)
{
    std::array<int, 16> clues = { 0, 0, 1, 2, 0, 2, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0 };
    int etalon[4][4] = { { 2, 1, 4, 3 },
                         { 3, 4, 1, 2 },
                         { 4, 2, 3, 1 },
                         { 1, 3, 2, 4 } };
    EXPECT_EQ(1, equal(ss.Solve(clues.data()), etalon)) << print<4>(ss.GetLastMatrix(), clues);
}

TEST_F(TestSkyScrapers, happy2_4x4)
{
    std::array<int, 16> clues = { 3, 0, 0, 0, 0, 2, 3, 0, 4, 0, 2, 0, 0, 1, 0, 3 };
    int etalon[4][4] = { { 1, 3, 2, 4 },
                         { 2, 4, 1, 3 },
                         { 4, 1, 3, 2 },
                         { 3, 2, 4, 1 } };
    EXPECT_EQ(1, equal(ss.Solve(clues.data()), etalon)) << print<4>(ss.GetLastMatrix(), clues);
}

TEST_F(TestSkyScrapers, fullfilled_6x6)
{
    std::array<int, 6*4> clues = { 3, 2, 2, 3, 2, 1, 1, 2, 3, 3, 2, 2, 5, 1, 2, 2, 4, 3, 3, 2, 1, 2, 2, 4 };
    int etalon[6][6] = { { 2, 1, 4, 3, 5, 6},
                         { 1, 6, 3, 2, 4, 5},
                         { 4, 3, 6, 5, 1, 2},
                         { 6, 5, 2, 1, 3, 4},
                         { 5, 4, 1, 6, 2, 3},
                         { 3, 2, 5, 4, 6, 1 } };
    EXPECT_EQ(1, equal(ss6.Solve(clues.data()), etalon)) << print<6>(ss.GetLastMatrix(), clues);
}

TEST_F(TestSkyScrapers, happy1_6x6)
{
    std::array<int, 6*4> clues = { 0, 0, 0, 2, 2, 0, 0, 0, 0, 6, 3, 0, 0, 4, 0, 0, 0, 0, 4, 4, 0, 3, 0, 0 };
    int etalon[6][6] = { { 5, 6, 1, 4, 3, 2 },
                         { 4, 1, 3, 2, 6, 5 },
                         { 2, 3, 6, 1, 5, 4 },
                         { 6, 5, 4, 3, 2, 1 },
                         { 1, 2, 5, 6, 4, 3 },
                         { 3, 4, 2, 5, 1, 6 } };
    EXPECT_EQ(1, equal(ss6.Solve(clues.data()), etalon)) << print<6>(ss.GetLastMatrix(), clues);
}

TEST_F(TestSkyScrapers, happy2_6x6)
{
    std::array<int, 6*4> clues = { 0, 3, 0, 5, 3, 4, 0, 0, 0, 0, 0, 1, 0, 3, 0, 3, 2, 3, 3, 2, 0, 3, 1, 0};
    int etalon[6][6] = { { 5, 2, 6, 1, 4, 3 },
                         { 6, 4, 3, 2, 5, 1 },
                         { 3, 1, 5, 4, 6, 2 },
                         { 2, 6, 1, 5, 3, 4 },
                         { 4, 3, 2, 6, 1, 5 },
                         { 1, 5, 4, 3, 2, 6 } };
    EXPECT_EQ(1, equal(ss6.Solve(clues.data()), etalon)) << print<6>(ss.GetLastMatrix(), clues);
}

TEST_F(TestSkyScrapers, happy1_7x7)
{
    std::array<int, 7*4> clues = { 7, 0, 0, 0, 2, 2, 3, 0, 0, 3, 0, 0, 0, 0, 3, 0, 3, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 4 };
    int etalon[7][7] = { { 1, 5, 6, 7, 4, 3, 2 },
                         { 2, 7, 4, 5, 3, 1, 6 },
                         { 3, 4, 5, 6, 7, 2, 1 },
                         { 4, 6, 3, 1, 2, 7, 5 },
                         { 5, 3, 1, 2, 6, 4, 7 },
                         { 6, 2, 7, 3, 1, 5, 4 },
                         { 7, 1, 2, 4, 5, 6, 3 } };
    EXPECT_EQ(1, equal(ss7.Solve(clues.data()), etalon)) << print<7>(ss.GetLastMatrix(), clues);
}


TEST_F(TestSkyScrapers, happy2_7x7)
{
    /*
                   1  2     4
                   2  3     2
              +---------------------+
         27 1 | 7, 6, 2, 1, 5, 4, 3 | 5 7
         26 4 | 1, 3, 5, 4, 2, 7, 6 |
         25 2 | 6, 5, 4, 7, 3, 2, 1 | 4 9
         24 2 | 5, 1, 7, 6, 4, 3, 2 | 5 10
         23 2 | 4, 2, 1, 3, 7, 6, 5 |
         22 2 | 3, 7, 6, 2, 1, 5, 4 | 4 12
         21 5 | 2, 4, 3, 5, 6, 1, 7 |
              +---------------------+
                6           2  4
               20          16 15
     */
    std::array<int, 7*4> clues = { 0, 2, 3, 0, 2, 0, 0, 5, 0, 4, 5, 0, 4, 0, 0, 4, 2, 0, 0, 0, 6, 5, 2, 2, 2, 2, 4, 1 };
    int etalon[7][7] = { { 7, 6, 2, 1, 5, 4, 3 },
                         { 1, 3, 5, 4, 2, 7, 6 },
                         { 6, 5, 4, 7, 3, 2, 1 },
                         { 5, 1, 7, 6, 4, 3, 2 },
                         { 4, 2, 1, 3, 7, 6, 5 },
                         { 3, 7, 6, 2, 1, 5, 4 },
                         { 2, 4, 3, 5, 6, 1, 7 } };
    EXPECT_EQ(1, equal(ss7.Solve(clues.data()), etalon)) << print<7>(ss.GetLastMatrix(), clues);
}

TEST_F(TestSkyScrapers, happy3_7x7)
{
    /*
      0 1 2 3 4 5 6 
      6 4   2     3   
    +---------------+ 
27 3| 2 1 6 4 3 7 5 |  7
26  | 3 2 5 7 4 6 1 |3 8
25  | 4 6 7 5 1 2 3 |3 9
24 4| 1 3 2 6 7 5 4 |3 10
23  | 5 7 1 3 2 4 6 |  11
22  | 6 4 3 2 5 1 7 |  12
21  | 7 5 4 1 6 3 2 |4 13
    +---------------+ 
        2   5   5    
      20191817161514

     */
    std::array<int, 7*4> clues = { 6, 4, 0, 2, 0, 0, 3, 0, 3, 3, 3, 0, 0, 4, 0, 5, 0, 5, 0, 2, 0, 0, 0, 0, 4, 0, 0, 3 };
    int etalon[7][7] = { { 2, 1, 6, 4, 3, 7, 5 },
                         { 3, 2, 5, 7, 4, 6, 1 },
                         { 4, 6, 7, 5, 1, 2, 3 },
                         { 1, 3, 2, 6, 7, 5, 4 },
                         { 5, 7, 1, 3, 2, 4, 6 },
                         { 6, 4, 3, 2, 5, 1, 7 },
                         { 7, 5, 4, 1, 6, 3, 2 } };
    EXPECT_EQ(1, equal(ss7.Solve(clues.data()), etalon)) << print<7>(ss.GetLastMatrix(), clues);
}

