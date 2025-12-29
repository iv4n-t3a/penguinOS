#include "tests/kernel/mem/mem_test_util.h"
#include <array>
#include <gtest/gtest.h>

extern "C" {
#include "kernel/config.h"
#include "kernel/mem/kmalloc.h"
#include "kernel/mem/pfalloc.h"
}

class KmallocTest : public ::testing::TestWithParam<size_t> {
protected:
  static const size_t kPagesInPool = 10000;
  static const size_t kPoolSize = kPagesInPool * PAGE_SIZE;

  void SetUp() override {
    mem_test_util::mark_free(mem_.data(), kPoolSize);
    pfalloc_init(mem_.data(), kPoolSize);
    kmalloc_init(get_pfalloc_buddy());
  }

private:
  std::array<std::byte, kPoolSize> mem_;
};

void *kmalloc_and_check(size_t size) {
  void *mem = kmalloc(size);
  EXPECT_NE(mem, nullptr);
  mem_test_util::check_free(mem, size);
  mem_test_util::mark_used(mem, size);
  return mem;
}

TEST_P(KmallocTest, Alloc) {
  void *mem = kmalloc_and_check(GetParam());
  kfree(mem);
}

TEST_P(KmallocTest, Free) {
  size_t kSize = GetParam();

  for (size_t i = 0; i < 10000; ++i) {
    void *mem = kmalloc_and_check(kSize);
    kfree(mem);
    mem_test_util::mark_free(mem, kSize);
  }
}

INSTANTIATE_TEST_SUITE_P(VaryingObjectSizes, KmallocTest,
                         ::testing::Values(42, PAGE_SIZE * 2));
