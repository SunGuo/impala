// Copyright 2012 Cloudera Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <gtest/gtest.h>
#include "util/url-coding.h"
#include "util/logging.h"

using namespace std;

namespace impala {

// Tests encoding/decoding of input.  If expected_encoded is non-empty, the
// encoded string is validated against it.
void TestUrl(const string& input, const string& expected_encoded) {
  string intermediate;
  UrlEncode(input, &intermediate);
  string output;
  if (!expected_encoded.empty()) {
    EXPECT_EQ(intermediate, expected_encoded);
  }
  EXPECT_TRUE(UrlDecode(intermediate, &output));
  EXPECT_EQ(input, output);

  // Convert string to vector and try that also
  vector<uint8_t> input_vector;
  input_vector.resize(input.size());
  memcpy(&input_vector[0], input.c_str(), input.size());
  string intermediate2;
  UrlEncode(input_vector, &intermediate2);
  EXPECT_EQ(intermediate, intermediate2);
}

void TestBase64(const string& input, const string& expected_encoded) {
  string intermediate;
  Base64Encode(input, &intermediate);
  string output;
  if (!expected_encoded.empty()) {
    EXPECT_EQ(intermediate, expected_encoded);
  }
  EXPECT_TRUE(Base64Decode(intermediate, &output));
  EXPECT_EQ(input, output);

  // Convert string to vector and try that also
  vector<uint8_t> input_vector;
  input_vector.resize(input.size());
  memcpy(&input_vector[0], input.c_str(), input.size());
  string intermediate2;
  Base64Encode(input_vector, &intermediate2);
  EXPECT_EQ(intermediate, intermediate2);
}

// Test URL encoding. Check that the values that are put in are the
// same that come out.
TEST(UrlCodingTest, Basic) {
  string input = "ABCDEFGHIJKLMNOPQRSTUWXYZ1234567890~!@#$%^&*()<>?,./:\";'{}|[]\\_+-=";
  TestUrl(input, "");
}

TEST(UrlCodingTest, BlankString) {
  TestUrl("", "");
}

TEST(UrlCodingTest, PathSeparators) {
  TestUrl("/home/impala/directory/", "%2Fhome%2Fimpala%2Fdirectory%2F");
}

TEST(Base64Test, Basic) {
  TestBase64("a", "YQ==");
  TestBase64("ab", "YWI=");
  TestBase64("abc", "YWJj");
  TestBase64("abcd", "YWJjZA==");
  TestBase64("abcde", "YWJjZGU=");
  TestBase64("abcdef", "YWJjZGVm");
}

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
