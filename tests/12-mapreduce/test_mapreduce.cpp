#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include "mapper_column/lib.h"
#include "reducer_avg/lib.h"
#include "reducer_var/lib.h"

const std::string input_data(
    "5295,Beautiful 1br on Upper West Side,7702,Lena,Manhattan,Upper West "
    "Side,40.80316,-73.96545,Entire home/apt,135,5,53,2019-06-22,0.43,1,6\n"
    "5441,Central Manhattan/near Broadway,7989,Kate,Manhattan,Hell's "
    "Kitchen,40.76076,-73.98867,Private room,85,2,188,2019-06-23,1.50,1,39\n"
    "5803,\"Lovely Room 1; Garden; Best Area; Legal "
    "rental\",9744,Laurie,Brooklyn,South Slope,40.66829,-73.98779,Private "
    "room,89,4,167,2019-06-24,1.34,3,314\n"
    "6021,Wonderful Guest Bedroom in Manhattan for "
    "SINGLES,11528,Claudio,Manhattan,Upper West "
    "Side,40.79826,-73.96113,Private room,85,2,113,2019-07-05,0.91,1,333\n"
    "6090,West Village Nest - Superhost,11975,Alina,Manhattan,West "
    "Village,40.7353,-74.00525,Entire home/apt,120,90,27,2018-10-31,0.22,1,0\n"
    "6848,Only 2 stops to Manhattan studio,15991,Allen & "
    "Irina,Brooklyn,Williamsburg,40.70837,-73.95352,Entire "
    "home/apt,140,2,148,2019-06-29,1.20,1,46\n"
    "7097,Perfect for Your Parents + Garden,17571,Jane,Brooklyn,Fort "
    "Greene,40.69169,-73.97185,Entire "
    "home/apt,215,2,198,2019-06-28,1.72,1,321");

const std::string price_data("135\n85\n89\n85\n120\n140\n215\n");
constexpr double avg_price = 124.142857143;
constexpr double var_price = 1848.693877551;

TEST(mapper, price) {
  std::stringstream in(input_data);
  std::stringstream out;

  EXPECT_TRUE(get_column(in, out, 9, ','));
  EXPECT_EQ(out.str(), price_data);
}

TEST(avg, reduce) {
  std::stringstream in(price_data);
  double result = get_avg(in);
  EXPECT_NEAR(result, avg_price, 1e-9);
}

TEST(var, reduce) {
  std::stringstream in(price_data);
  double result = get_var(in);
  EXPECT_NEAR(result, var_price, 1e-9);
}