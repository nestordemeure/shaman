#include "../eft.h"

#include <gtest/gtest.h>

/*
 * NOTE :
 * avoid power of two in tests, they tend to produce correct roundings a lot more often
 */
namespace
{
    /*
     * needed for hexadecimal number with negative exponent
     * should not be required from C++17 onward
     * fails with std::strtod(hexstr, nullptr);
     */
    template<typename F>
    F numOfHex(const char hexstr[])
    {
        uint32_t num;
        F f;
        sscanf(hexstr, "%x", &num);
        f = *((F*)&num);
        return f;
    }

    /*
     * test whether an EFT is computed accurately in case of cancellation
     * x is much larger than y
     */
    template<typename F>
    bool test_cancellation(F x, F y)
    {
        F z = x + y;
        F error = EFT::TwoSum(x, y, z);
        return (error == y);
    }

    /*
     * test whether an EFT is computed accurately in case of exact subtraction
     * y/2 < x < y*2
     * (requires larger bound to make sure that rounding will not get lucky)
     */
    template<typename F>
    bool test_exactSub(F x, F y)
    {
        F z = x - y;
        F error = EFT::TwoSum(x, -y, z);
        return (error == 0);
    }

    TEST(EFT_SUM, float)
    {
        // cancellation
        EXPECT_TRUE(test_cancellation<float>(1e30, 1.1));
        EXPECT_FALSE(test_cancellation<float>(1.2, 1.2));

        // exact subtraction
        EXPECT_TRUE(test_exactSub<float>(1.3, 0.7));
        EXPECT_TRUE(test_exactSub<float>(1.4, 2.1));
        EXPECT_FALSE(test_exactSub<float>(1.5, 0.3));
        EXPECT_FALSE(test_exactSub<float>(1.7, 4.7));
    }

    // TODO we might search for binary values to complete the test suite
    TEST(EFT_SUM, double)
    {
        // cancellation
        EXPECT_TRUE(test_cancellation<double>(1e30, 1.1));
        EXPECT_FALSE(test_cancellation<double>(1.2, 1.2));

        // exact subtraction
        EXPECT_TRUE(test_exactSub<double>(1.3, 0.7));
        EXPECT_TRUE(test_exactSub<double>(1.4, 2.1));
        EXPECT_FALSE(test_exactSub<double>(1.5, 0.3));
        EXPECT_FALSE(test_exactSub<double>(1.7, 4.7));
    }

    // tests inspired by https://bugs.python.org/file46304/fma_reference.py
    TEST(EFT_FMA, float)
    {
        // zero result from nonzero inputs
        EXPECT_EQ(std::fma(2.0, 2.0, -4.0), 0);
        EXPECT_EQ(std::fma(2.0, -2.0, 4.0), 0);
        EXPECT_EQ(std::fma(-2.0, -2.0, -4.0), 0);
        EXPECT_EQ(std::fma(-2.0, 2.0, 4.0), 0);

        // overflow from multiplication
        float a = numOfHex<float>("0x1p64");
        float b = a;
        float c = numOfHex<float>("0x1p127");
        EXPECT_EQ(std::fma(a, b, -c), c);
        EXPECT_NEQ(a*b-c, c); // could pass if the compiler introduces an fma

        // single rounding
        a = numOfHex<float>("0x1p-50");
        EXPECT_EQ(std::fma(a - 1.0, a + 1.0, 1.0), a*a);

        // random tests
        std::vector<std::string> test_values = {
            // a, b, c, expected
            "0x1.694adde428b44p-1", "0x1.371b0d64caed7p-1", "0x1.f347e7b8deab8p-4", "0x1.19f10da56c8adp-1",
            "0x1.605401ccc6ad6p-2", "0x1.ce3a40bf56640p-2", "0x1.96e3bf7bf2e20p-2", "0x1.1af6d8aa83101p-1",
            "0x1.e5abd653a67d4p-2", "0x1.a2e400209b3e6p-1", "0x1.a90051422ce13p-1", "0x1.37d68cc8c0fbbp+0",
            "0x1.f94e8efd54700p-2", "0x1.123065c812cebp-1", "0x1.458f86fb6ccd0p-1", "0x1.ccdcee26a3ff3p-1",
            "0x1.bd926f1eedc96p-1", "0x1.eee9ca68c5740p-1", "0x1.960c703eb3298p-2", "0x1.3cdcfb4fdb007p+0",
            "0x1.27348350fbccdp-1", "0x1.3b073914a53f1p-1", "0x1.e300da5c2b4cbp-1", "0x1.4c51e9a3c4e29p+0",
            "0x1.2774f00b3497bp-1", "0x1.7038ec336bff0p-2", "0x1.2f6f2ccc3576bp-1", "0x1.99ad9f9c2688bp-1",
            "0x1.51d5a99300e5cp-1", "0x1.5cd74abd445a1p-1", "0x1.8880ab0bbe530p-1", "0x1.3756f96b91129p+0",
            "0x1.73cb965b821b8p-2", "0x1.218fd3d8d5371p-1", "0x1.d1ea966a1f758p-2", "0x1.5217b8fd90119p-1",
            "0x1.4aa98e890b046p-1", "0x1.954d85dff1041p-1", "0x1.122b59317ebdfp-1", "0x1.0bf644b340cc5p+0",
            "0x1.e28f29e44750fp-1", "0x1.4bcc4fdcd18fep-1", "0x1.fd47f81298259p-1", "0x1.9b000afbc9995p+0",
            "0x1.d2e850717fe78p-3", "0x1.1dd7531c303afp-1", "0x1.e0869746a2fc2p-2", "0x1.316df6eb26439p-1",
            "0x1.cf89c75ee6fbap-2", "0x1.b23decdc66825p-1", "0x1.3d1fe76ac6168p-1", "0x1.00d8ea4c12abbp+0",
            "0x1.3265ae6f05572p-2", "0x1.16d7ec285f7a2p-1", "0x1.0b8405b3827fbp-1", "0x1.5ef33c118a001p-1",
            "0x1.c4d1bf55ec1a5p-1", "0x1.bc59618459e12p-2", "0x1.ce5b73dc1773dp-1", "0x1.496cf6164f99bp+0",
            "0x1.d350026ac3946p-1", "0x1.9a234e149a68cp-2", "0x1.f5467b1911fd6p-2", "0x1.b5cee3225caa5p-1"
        };
        for(int i = 0; i < test_values.size(); i+=4)
        {
            float ra = numOfHex<float>(test_values[i].c_str());
            float rb = numOfHex<float>(test_values[i+1].c_str());
            float rc = numOfHex<float>(test_values[i+2].c_str());
            float expected = numOfHex<float>(test_values[i+3].c_str());
            EXPECT_EQ(std::fma(ra, rb, rc), expected);
        }
    }
}