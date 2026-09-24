#pragma once

#include <eng/resources/ResourceError.hpp>

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace eng
{

template<class Loader, class T>
concept ResourceLoader = std::invocable<Loader&>
                         && std::same_as<std::invoke_result_t<Loader&>, std::expected<T, ResourceError>>;

template<class Id, class T>
class ResourceCache
{
public:
    template<class Loader>
        requires ResourceLoader<Loader, T>
    std::expected<Id, ResourceError> load(std::string_view key, Loader&& loader)
    {
        if(const std::optional<Id> existing = find(key))
        {
            return *existing;
        }

        std::expected<T, ResourceError> loaded = std::invoke(std::forward<Loader>(loader));
        if(!loaded)
        {
            return std::unexpected(loaded.error());
        }

        const std::size_t index = storage_.size();
        storage_.push_back(std::make_unique<T>(std::move(*loaded)));
        keys_.emplace_back(key);
        return Id{.id = static_cast<std::uint32_t>(index + 1)};
    }

    const T* get(Id id) const noexcept
    {
        if(id.id == 0 || static_cast<std::size_t>(id.id) > storage_.size())
        {
            return nullptr;
        }
        return storage_[static_cast<std::size_t>(id.id - 1)].get();
    }

    std::optional<Id> find(std::string_view key) const
    {
        for(std::size_t i = 0; i < keys_.size(); ++i)
        {
            if(keys_[i] == key)
            {
                return Id{.id = static_cast<std::uint32_t>(i + 1)};
            }
        }
        return std::nullopt;
    }

    std::size_t size() const noexcept
    {
        return storage_.size();
    }

private:
    std::vector<std::unique_ptr<T>> storage_;
    std::vector<std::string> keys_;
};

} // namespace eng
