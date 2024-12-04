#include <cassert>
#include <iostream>
#include <memory>
#include <typeinfo>

class Engine
{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    
    std::unique_ptr<Engine> clone() const
    {
        auto cloned_engine = do_clone();
        assert(typeid(*cloned_engine) == typeid(*this));
        return cloned_engine;
    }
    
    virtual ~Engine() = default;
protected:
    virtual std::unique_ptr<Engine> do_clone() const = 0;
};

// CRTP for cloning
template <typename TEngine, typename TEngineBase = Engine>
class CloneableEngine : public TEngineBase
{
protected:
    using TEngineBase::TEngineBase;

    std::unique_ptr<Engine> do_clone() const override
    {
        return std::make_unique<TEngine>(static_cast<const TEngine&>(*this)); // call of copy constructor for T
    }
};

class Diesel : public CloneableEngine<Diesel>
{
    double displacement_;
public:
    Diesel(double displacement) : displacement_{displacement}
    {}

    virtual void start() override
    {
        std::cout << "Diesel starts\n";
    }

    virtual void stop() override
    {
        std::cout << "Diesel stops\n";
    }
};

class TDI : public Diesel
{
public:
    using Diesel::Diesel;

    virtual void start() override
    {
        std::cout << "TDI starts\n";
    }

    virtual void stop() override
    {
        std::cout << "TDI stops\n";
    }
};

class Hybrid : public CloneableEngine<Hybrid>
{
public:
    virtual void start() override
    {
        std::cout << "Hybrid starts\n";
    }

    virtual void stop() override
    {
        std::cout << "Hybrid stops\n";
    }
};

class Car
{
    std::unique_ptr<Engine> engine_;

public:
    Car(std::unique_ptr<Engine> engine)
        : engine_{std::move(engine)}
    {
    }

    Car(const Car& source) : engine_{source.engine_->clone()}
    {        
    }
    
    Car& operator=(const Car& source) 
    {
        if (this != &source)
        {
            engine_ = source.engine_->clone();
        }

        return *this;
    }

    Car(Car&&) = default;
    Car& operator=(Car&&) = default;

    void drive(int km)
    {
        engine_->start();
        std::cout << "Driving " << km << " kms\n";
        engine_->stop();
    }
};

int main()
{
    Car c1{std::make_unique<TDI>(2000)};
    c1.drive(100);

    std::cout << "\n";

    Car c2 = c1;
    c2.drive(200);
}
