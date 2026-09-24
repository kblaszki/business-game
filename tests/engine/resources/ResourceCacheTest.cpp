#include <eng/core/Handle.hpp>
#include <eng/resources/ResourceCache.hpp>
#include <eng/resources/ResourceError.hpp>
#include <gtest/gtest.h>

#include <expected>
#include <string>

namespace
{

struct Item
{
    int value{};
};

using ItemId = eng::Handle<struct ItemTag>;

struct CountingLoader
{
    int* calls{};
    std::expected<Item, eng::ResourceError> result{Item{.value = 1}};

    std::expected<Item, eng::ResourceError> operator()()
    {
        ++(*calls);
        return result;
    }
};

} // namespace

TEST(ResourceCacheTest, DedupesSameKeyWithoutReloading)
{
    eng::ResourceCache<ItemId, Item> cache;
    int calls = 0;
    CountingLoader loader{.calls = &calls, .result = Item{.value = 42}};

    const auto first = cache.load("hero", loader);
    const auto second = cache.load("hero", loader);

    ASSERT_TRUE(first.has_value());
    ASSERT_TRUE(second.has_value());
    EXPECT_EQ(first->id, second->id);
    EXPECT_EQ(calls, 1);
    EXPECT_EQ(cache.size(), 1u);
    ASSERT_NE(cache.get(*first), nullptr);
    EXPECT_EQ(cache.get(*first)->value, 42);
}

TEST(ResourceCacheTest, LoaderErrorLeavesSizeUnchanged)
{
    eng::ResourceCache<ItemId, Item> cache;
    int calls = 0;
    CountingLoader ok{.calls = &calls, .result = Item{.value = 7}};
    ASSERT_TRUE(cache.load("ok", ok).has_value());
    EXPECT_EQ(cache.size(), 1u);

    CountingLoader fail{.calls = &calls, .result = std::unexpected(eng::ResourceError::NotFound)};
    const auto failed = cache.load("missing", fail);

    ASSERT_FALSE(failed.has_value());
    EXPECT_EQ(failed.error(), eng::ResourceError::NotFound);
    EXPECT_EQ(cache.size(), 1u);
    EXPECT_EQ(cache.find("missing"), std::nullopt);
}

TEST(ResourceCacheTest, GetPointerStableAfterManyInserts)
{
    eng::ResourceCache<ItemId, Item> cache;
    int calls = 0;
    CountingLoader firstLoader{.calls = &calls, .result = Item{.value = 99}};
    const auto firstId = cache.load("first", firstLoader);
    ASSERT_TRUE(firstId.has_value());
    const Item* ptr = cache.get(*firstId);
    ASSERT_NE(ptr, nullptr);

    for(int i = 0; i < 1000; ++i)
    {
        CountingLoader loader{.calls = &calls, .result = Item{.value = i}};
        const auto id = cache.load(std::to_string(i), loader);
        ASSERT_TRUE(id.has_value());
    }

    EXPECT_EQ(cache.get(*firstId), ptr);
    EXPECT_EQ(ptr->value, 99);
    EXPECT_EQ(cache.size(), 1001u);
}

TEST(ResourceCacheTest, InvalidIdReturnsNullptr)
{
    eng::ResourceCache<ItemId, Item> cache;
    EXPECT_EQ(cache.get(ItemId{}), nullptr);
    EXPECT_EQ(cache.get(ItemId{.id = 1}), nullptr);
    EXPECT_EQ(cache.get(ItemId{.id = 999}), nullptr);
}

TEST(ResourceCacheTest, DescribeCoversAllErrors)
{
    EXPECT_EQ(eng::describe(eng::ResourceError::NotFound), "NotFound");
    EXPECT_EQ(eng::describe(eng::ResourceError::DecodeFailed), "DecodeFailed");
    EXPECT_EQ(eng::describe(eng::ResourceError::Unsupported), "Unsupported");
}
