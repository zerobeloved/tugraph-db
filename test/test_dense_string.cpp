﻿/* Copyright (c) 2022 AntGroup. All Rights Reserved. */

#include "fma-common/logging.h"

#include "import/dense_string.h"

#include "gtest/gtest.h"
#include "./ut_utils.h"

class TestDenseString : public TuGraphTest {};

TEST_F(TestDenseString, DenseString) {
    using namespace lgraph;
    using namespace import_v2;
    using namespace fma_common;
    DenseString s("hello");
    UT_LOG() << sizeof(s);
    DenseString s2("hello");
    DenseString tmp_s2("hello");
    UT_EXPECT_EQ(s, s2);
    UT_EXPECT_TRUE(!(s != s2));
    UT_EXPECT_EQ(s, std::move(tmp_s2));

    DenseString s_t("hillo");
    UT_EXPECT_NE(s, s_t);
    s_t.~DenseString();

    UT_EXPECT_TRUE(!(s < s2));
    UT_EXPECT_TRUE(!(s2 < s));
    UT_EXPECT_TRUE(!(s < s));
    UT_LOG() << "hash(s)=" << s.hash1();
    UT_LOG() << "hash2(s)=" << s.hash2();
    DenseString s3(std::string("world"));
    UT_LOG() << "hash(s3)=" << s3.hash1();
    UT_EXPECT_LT(s, s3);
    UT_EXPECT_LT(s2, s3);
    s = s3;
    s = DenseString("zabcdefghijklmn");
    UT_LOG() << "hash(s)=" << s.hash1();
    UT_LOG() << "hash2(s)=" << s.hash2();
    UT_EXPECT_LT(s3, s);

    s2 = DenseString("zabcdefghijklmn");
    UT_LOG() << "hash(s2)=" << s2.hash1();
    UT_LOG() << "hash2(s2)=" << s2.hash2();
    UT_EXPECT_EQ(s, s2);
}
