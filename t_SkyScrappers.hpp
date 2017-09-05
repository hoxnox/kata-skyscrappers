#include "SkyScrappers.hpp"

class TestSkyScrappers : public ::testing::Test
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
    SkyScrappers<4> ss;
    SkyScrappers<6> ss6;
    SkyScrappers<7> ss7;
};

TEST_F(TestSkyScrappers, getLine_top)
{
    SkyScrappers<4>::Matrix etalon =
        { { 1, 3, 4, 2 },
          { 4, 2, 1, 3 },
          { 3, 4, 2, 1 },
          { 2, 1, 3, 4 } };
    auto line = SkyScrappers<4>::getLine(etalon, 1);
    EXPECT_EQ(3, *line[0]);
    EXPECT_EQ(2, *line[1]);
    EXPECT_EQ(4, *line[2]);
    EXPECT_EQ(1, *line[3]);
}

TEST_F(TestSkyScrappers, getLine_right)
{
    SkyScrappers<4>::Matrix etalon =
        { { 1, 3, 4, 2 },
          { 4, 2, 1, 3 },
          { 3, 4, 2, 1 },
          { 2, 1, 3, 4 } };
    auto line = SkyScrappers<4>::getLine(etalon, 6);
    EXPECT_EQ(1, *line[0]);
    EXPECT_EQ(2, *line[1]);
    EXPECT_EQ(4, *line[2]);
    EXPECT_EQ(3, *line[3]);
}

TEST_F(TestSkyScrappers, getLine_bottom)
{
    SkyScrappers<4>::Matrix etalon =
        { { 1, 3, 4, 2 },
          { 4, 2, 1, 3 },
          { 3, 4, 2, 1 },
          { 2, 1, 3, 4 } };
    auto line = SkyScrappers<4>::getLine(etalon, 10);
    EXPECT_EQ(1, *line[0]);
    EXPECT_EQ(4, *line[1]);
    EXPECT_EQ(2, *line[2]);
    EXPECT_EQ(3, *line[3]);

    SkyScrappers<6>::Matrix etalon6 =
    { { 2, 1, 4, 3, 5, 6},
      { 1, 6, 3, 2, 4, 5},
      { 4, 3, 6, 5, 1, 2},
      { 6, 5, 2, 1, 3, 4},
      { 5, 4, 1, 6, 2, 3},
      { 3, 2, 5, 4, 6, 1} };
    auto line6 = SkyScrappers<6>::getLine(etalon6, 13);
    EXPECT_EQ(6, *line6[0]);
    EXPECT_EQ(2, *line6[1]);
    EXPECT_EQ(3, *line6[2]);
    EXPECT_EQ(1, *line6[3]);
    EXPECT_EQ(4, *line6[4]);
    EXPECT_EQ(5, *line6[5]);
}


TEST_F(TestSkyScrappers, getLine_left)
{
    SkyScrappers<4>::Matrix etalon =
        { { 1, 3, 4, 2 },
          { 4, 2, 1, 3 },
          { 3, 4, 2, 1 },
          { 2, 1, 3, 4 } };
    auto line = SkyScrappers<4>::getLine(etalon, 15);
    EXPECT_EQ(1, *line[0]);
    EXPECT_EQ(3, *line[1]);
    EXPECT_EQ(4, *line[2]);
    EXPECT_EQ(2, *line[3]);
}

TEST_F(TestSkyScrappers, ridx)
{
    EXPECT_EQ( 0, SkyScrappers<7>::ridx(20));
    EXPECT_EQ( 1, SkyScrappers<7>::ridx(19));
    EXPECT_EQ( 2, SkyScrappers<7>::ridx(18));
    EXPECT_EQ( 3, SkyScrappers<7>::ridx(17));
    EXPECT_EQ( 4, SkyScrappers<7>::ridx(16));
    EXPECT_EQ( 5, SkyScrappers<7>::ridx(15));
    EXPECT_EQ( 6, SkyScrappers<7>::ridx(14));
    EXPECT_EQ( 7, SkyScrappers<7>::ridx(27));
    EXPECT_EQ( 8, SkyScrappers<7>::ridx(26));
    EXPECT_EQ( 9, SkyScrappers<7>::ridx(25));
    EXPECT_EQ(10, SkyScrappers<7>::ridx(24));
    EXPECT_EQ(11, SkyScrappers<7>::ridx(23));
    EXPECT_EQ(12, SkyScrappers<7>::ridx(22));
    EXPECT_EQ(13, SkyScrappers<7>::ridx(21));
    EXPECT_EQ(14, SkyScrappers<7>::ridx( 6));
    EXPECT_EQ(15, SkyScrappers<7>::ridx( 5));
    EXPECT_EQ(16, SkyScrappers<7>::ridx( 4));
    EXPECT_EQ(17, SkyScrappers<7>::ridx( 3));
    EXPECT_EQ(18, SkyScrappers<7>::ridx( 2));
    EXPECT_EQ(19, SkyScrappers<7>::ridx( 1));
    EXPECT_EQ(20, SkyScrappers<7>::ridx( 0));
    EXPECT_EQ(21, SkyScrappers<7>::ridx(13));
    EXPECT_EQ(22, SkyScrappers<7>::ridx(12));
    EXPECT_EQ(23, SkyScrappers<7>::ridx(11));
    EXPECT_EQ(24, SkyScrappers<7>::ridx(10));
    EXPECT_EQ(25, SkyScrappers<7>::ridx( 9));
    EXPECT_EQ(26, SkyScrappers<7>::ridx( 8));
    EXPECT_EQ(27, SkyScrappers<7>::ridx( 7));
}

TEST_F(TestSkyScrappers, print_4x4)
{
    EXPECT_EQ("             \n"
              " +---------+ \n"
              " | 0 0 0 0 | \n"
              " | 0 0 0 0 | \n"
              " | 0 0 0 0 | \n"
              " | 0 0 0 0 | \n"
              " +---------+ \n"
              "             \n", ss.PrintLastMatrix());
}

TEST_F(TestSkyScrappers, fullfilled_4x4)
{
    std::array<int, 16> clues = { 2, 2, 1, 3, 2, 2, 3, 1, 1, 2, 2, 3, 3, 2, 1, 3 };
    int etalon[4][4] = { { 1, 3, 4, 2 },
                         { 4, 2, 1, 3 },
                         { 3, 4, 2, 1 },
                         { 2, 1, 3, 4 } };
    EXPECT_EQ(1, equal(ss.Solve(clues.data()), etalon)) << ss.PrintLastMatrix();
}

TEST_F(TestSkyScrappers, happy1_4x4)
{
    std::array<int, 16> clues = { 0, 0, 1, 2, 0, 2, 0, 0, 0, 3, 0, 0, 0, 1, 0, 0 };
    int etalon[4][4] = { { 2, 1, 4, 3 },
                         { 3, 4, 1, 2 },
                         { 4, 2, 3, 1 },
                         { 1, 3, 2, 4 } };
    EXPECT_EQ(1, equal(ss.Solve(clues.data()), etalon)) << ss.PrintLastMatrix();
}

TEST_F(TestSkyScrappers, happy2_4x4)
{
    std::array<int, 16> clues = { 3, 0, 0, 0, 0, 2, 3, 0, 4, 0, 2, 0, 0, 1, 0, 3 };
    int etalon[4][4] = { { 1, 3, 2, 4 },
                         { 2, 4, 1, 3 },
                         { 4, 1, 3, 2 },
                         { 3, 2, 4, 1 } };
    EXPECT_EQ(1, equal(ss.Solve(clues.data()), etalon)) << ss.PrintLastMatrix();
}

TEST_F(TestSkyScrappers, fullfilled_6x6)
{
    std::array<int, 6*4> clues = { 3, 2, 2, 3, 2, 1, 1, 2, 3, 3, 2, 2, 5, 1, 2, 2, 4, 3, 3, 2, 1, 2, 2, 4 };
    int etalon[6][6] = { { 2, 1, 4, 3, 5, 6},
                         { 1, 6, 3, 2, 4, 5},
                         { 4, 3, 6, 5, 1, 2},
                         { 6, 5, 2, 1, 3, 4},
                         { 5, 4, 1, 6, 2, 3},
                         { 3, 2, 5, 4, 6, 1 } };
    EXPECT_EQ(1, equal(ss6.Solve(clues.data()), etalon)) << ss6.PrintLastMatrix();
}

TEST_F(TestSkyScrappers, happy1_6x6)
{
    std::array<int, 6*4> clues = { 0, 0, 0, 2, 2, 0, 0, 0, 0, 6, 3, 0, 0, 4, 0, 0, 0, 0, 4, 4, 0, 3, 0, 0 };
    int etalon[6][6] = { { 5, 6, 1, 4, 3, 2 },
                         { 4, 1, 3, 2, 6, 5 },
                         { 2, 3, 6, 1, 5, 4 },
                         { 6, 5, 4, 3, 2, 1 },
                         { 1, 2, 5, 6, 4, 3 },
                         { 3, 4, 2, 5, 1, 6 } };
    EXPECT_EQ(1, equal(ss6.Solve(clues.data()), etalon)) << ss6.PrintLastMatrix();
}

TEST_F(TestSkyScrappers, happy2_6x6)
{
    std::array<int, 6*4> clues = { 0, 3, 0, 5, 3, 4, 0, 0, 0, 0, 0, 1, 0, 3, 0, 3, 2, 3, 3, 2, 0, 3, 1, 0};
    int etalon[6][6] = { { 5, 2, 6, 1, 4, 3 },
                         { 6, 4, 3, 2, 5, 1 },
                         { 3, 1, 5, 4, 6, 2 },
                         { 2, 6, 1, 5, 3, 4 },
                         { 4, 3, 2, 6, 1, 5 },
                         { 1, 5, 4, 3, 2, 6 } };
    EXPECT_EQ(1, equal(ss6.Solve(clues.data()), etalon)) << ss6.PrintLastMatrix();
}

TEST_F(TestSkyScrappers, happy1_7x7)
{
    std::array<int, 7*4> clues = { 7, 0, 0, 0, 2, 2, 3, 0, 0, 3, 0, 0, 0, 0, 3, 0, 3, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 4 };
    int etalon[7][7] = { { 1, 5, 6, 7, 4, 3, 2 },
                         { 2, 7, 4, 5, 3, 1, 6 },
                         { 3, 4, 5, 6, 7, 2, 1 },
                         { 4, 6, 3, 1, 2, 7, 5 },
                         { 5, 3, 1, 2, 6, 4, 7 },
                         { 6, 2, 7, 3, 1, 5, 4 },
                         { 7, 1, 2, 4, 5, 6, 3 } };
    EXPECT_EQ(1, equal(ss7.Solve(clues.data()), etalon)) << ss7.PrintLastMatrix();
}


TEST_F(TestSkyScrappers, happy2_7x7)
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
    EXPECT_EQ(1, equal(ss7.Solve(clues.data()), etalon)) << ss7.PrintLastMatrix();
}

TEST_F(TestSkyScrappers, happy3_7x7)
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
    EXPECT_EQ(1, equal(ss7.Solve(clues.data()), etalon)) << ss7.PrintLastMatrix();
}

