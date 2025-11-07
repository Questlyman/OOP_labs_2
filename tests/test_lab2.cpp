#include <gtest/gtest.h>
#include "../twelve.h"

TEST(Ctors, FromStringAndToString) {
    Twelve a("0");
    EXPECT_EQ(a.toString(), "0");
    Twelve b("1AB0B");
    EXPECT_EQ(b.toString(), "1AB0B");
    EXPECT_THROW(Twelve("1AC"), std::invalid_argument); // 'C' не цифра
}

TEST(Compare, Basic) {
    Twelve a("B");          // 11
    Twelve b("10");         // 12
    Twelve c("10");
    EXPECT_TRUE(a.isLess(b));
    EXPECT_TRUE(b.isGreater(a));
    EXPECT_TRUE(b.isEqual(c));
    EXPECT_EQ(b.compare(c), 0);
}

TEST(Arithmetic, Add) {
    //  "B" (11) + "1" (1) = "10" (12)
    Twelve a("B"), b("1");
    Twelve s = a.add(b);
    EXPECT_EQ(s.toString(), "10");

    // "1AB0B" + "9" = "1AB18"
    Twelve x("1AB0B"), y("9");
    EXPECT_EQ(x.add(y).toString(), "1AB18");
}

TEST(Arithmetic, SubOk) {
    Twelve a("10");   // 12
    Twelve b("1");    // 1
    Twelve r = a.sub(b);
    EXPECT_EQ(r.toString(), "B"); // 11
}

TEST(Arithmetic, SubFail) {
    Twelve a("1");
    Twelve b("10");
    EXPECT_THROW(a.sub(b), std::domain_error);
}


TEST(Compat, TrySubSignature) {
    Twelve a("144");
    Twelve b("1");
    Twelve out("0");
    bool ok = a.trySub(b, out);
    EXPECT_TRUE(ok);
    EXPECT_EQ(out.toString(), "143");

    Twelve c("1");
    ok = c.trySub(a, out);
    EXPECT_FALSE(ok); // out не должен меняться
    EXPECT_EQ(out.toString(), "143");
}
