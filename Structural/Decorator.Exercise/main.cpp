#include "starbugs_coffee.hpp"

#include <memory>
#include <cassert>

void client(std::unique_ptr<Coffee> coffee)
{
    std::cout << "Description: " << coffee->get_description() << "; Price: " << coffee->get_total_price() << std::endl;
    coffee->prepare();
}

class CoffeeBuilder
{
    std::unique_ptr<Coffee> coffee_;
public:
    template<typename TBaseDrink>
    CoffeeBuilder& create_base_drink()
    {
        coffee_ = std::make_unique<TBaseDrink>();
        return *this;
    }

    template <typename TCondiment>
    CoffeeBuilder& add()
    {
        assert(coffee_);
        coffee_ = std::make_unique<TCondiment>(std::move(coffee_));
        return *this;
    }

    std::unique_ptr<Coffee> get_coffee()
    {
        return std::move(coffee_);
    }
};

int main()
{
    std::unique_ptr<Coffee> cf = 
        std::make_unique<CreamDecorator>(
            std::make_unique<WhiskyDecorator>(
                std::make_unique<WhiskyDecorator>(
                    std::make_unique<WhiskyDecorator>(
                        std::make_unique<Espresso>()))));

    client(std::move(cf));    

    std::cout << "\n\n";

    CoffeeBuilder coffee_builder;
    coffee_builder
        .create_base_drink<Espresso>()
        .add<CreamDecorator>()
        .add<WhiskyDecorator>()
        .add<WhiskyDecorator>()
        .add<ExtraEspressoDecorator>();

    client(coffee_builder.get_coffee());
}
