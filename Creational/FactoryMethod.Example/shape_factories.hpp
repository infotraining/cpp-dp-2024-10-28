#ifndef SHAPE_FACTORIES_HPP
#define SHAPE_FACTORIES_HPP

#include "shape.hpp"
#include "shape_readers_writers/shape_reader_writer.hpp"

#include <functional>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

namespace Drawing
{
    using ShapeCreator = std::function<std::unique_ptr<Shape>()>;
    using ShapeRWCreator = std::function<std::unique_ptr<IO::ShapeReaderWriter>()>;

    template <typename TProduct, typename TKey = std::string, typename TCreator = std::function<std::unique_ptr<TProduct>()>>
    class GenericFactory
    {
        std::unordered_map<TKey, TCreator> creators_;

    public:
        bool register_creator(const TKey& id, TCreator creator)
        {
            return creators_.emplace(id, creator).second;
        }

        std::unique_ptr<TProduct> create(const TKey& id)
        {
            auto it = creators_.find(id);
            if (it != creators_.end())
                return it->second();

            throw std::runtime_error("Unknown id");
        }
    };

    template <typename T>
    class SingletonHolder
    {
        SingletonHolder() = default;

    public:
        SingletonHolder(const SingletonHolder&) = delete;
        SingletonHolder& operator=(const SingletonHolder&) = delete;

        static T& instance()
        {
            static T unique_instance;

            return unique_instance;
        }
    };

    template <typename T>
    std::type_index make_type_index()
    {
        return std::type_index{typeid(T)};
    }

    template <typename T>
    std::type_index make_type_index(const T& obj)
    {
        return std::type_index{typeid(obj)};
    }

    using ShapeFactory = GenericFactory<Shape>;
    using ShapeRWFactory = GenericFactory<IO::ShapeReaderWriter, std::type_index>;

    using SingletonShapeFactory = SingletonHolder<ShapeFactory>;
    using SingletonShapeRWFactory = SingletonHolder<ShapeRWFactory>;
}

#endif