#ifndef ADAPTER_HPP_
#define ADAPTER_HPP_

#include <iostream>

// "Target"
class Target
{
public:
    virtual void request() = 0;
    virtual ~Target() = default;
};

// "Adaptee"
class Adaptee
{
public:
    void specific_request()
    {
        std::cout << "Called specific_request()" << std::endl;
    }
};

// "Adapter"
class ClassAdapter : public Target, private Adaptee
{
public:
    void request() override
    {
        specific_request();
    }
};

// "Adapter"
class ObjectAdapter : public Target
{
private:
    Adaptee& adaptee_;

public:
    ObjectAdapter(Adaptee& adaptee)
        : adaptee_(adaptee)
    {
    }

    void request() override
    {
        adaptee_.specific_request();
    }
};

namespace HighLevel
{
    class ISwitch
    {
    public:
        virtual void on() = 0;
        virtual void off() = 0;
        virtual ~ISwitch() = default;
    };
}

namespace LowLevel
{
    namespace API 
    {
        class LEDLight
        {
        public:
            void set_rgb(uint8_t r, uint8_t b, uint8_t c)
            {
                std::cout << "LEDLight::set_rgb(" << static_cast<int>(r) << ", " << static_cast<int>(b) << ", " << static_cast<int>(c) << ")" << std::endl;
            }
        };

            class CMYKLight
        {
        public:
            void set_cmyk(uint8_t c, uint8_t m, uint8_t y, uint8_t k)
            {
                std::cout << "CMYKLight::set_cmyk(" << static_cast<int>(c) << ", " << static_cast<int>(m) << ", " << static_cast<int>(y) << ", " << static_cast<int>(k) << ")" << std::endl;
            }
        };
    }

    namespace Adapters
    {
        // class adapter
        class LEDSwitch : public HighLevel::ISwitch, private API::LEDLight
        {
        public:
            void on() override
            {
                set_rgb(255, 255, 255);
            }

            void off() override
            {
                set_rgb(0, 0, 0);
            }
        };

        // object adapter
        class LEDSwitchWrapper : public HighLevel::ISwitch
        {
            API::LEDLight led_light_;
        public:
            void on() override
            {
                led_light_.set_rgb(255, 255, 255);
            }

            void off() override
            {
                led_light_.set_rgb(0, 0, 0);
            }
        };
    }
}

namespace HighLevel
{
    class Button
    {
        ISwitch& led_switch_;

    public:
        Button(ISwitch& switch_)
            : led_switch_(switch_)
        {
        }

        void click()
        {
            led_switch_.on();
        }
    };
}

#endif /*ADAPTER_HPP_*/
