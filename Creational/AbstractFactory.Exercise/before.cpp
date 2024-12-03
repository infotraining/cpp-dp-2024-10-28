#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

#define MOTIF

enum class IconType
{
    none,
    ok,
    cancel,
    warning,
    error
};

class Button;
class Menu;

class Widget
{
public:
    virtual void draw() = 0;
    virtual ~Widget() = default;
};

class WidgetCreator
{
public:
    virtual std::unique_ptr<Button> createButton(const std::string& caption, IconType icon_type) = 0;
    virtual std::unique_ptr<Menu> createMenu(const std::string& text) = 0;
    virtual ~WidgetCreator() = default;
};

class Button : public Widget
{
    std::string caption_;
    IconType icon_type_;

public:
    Button(const std::string& caption, IconType icon_type)
        : caption_{caption}
        , icon_type_{icon_type}
    {
    }

    std::string caption() const
    {
        return caption_;
    }

    IconType icon() const
    {
        return icon_type_;
    }
};

class Menu : public Widget
{
    std::string text_;

public:
    Menu(const std::string& text)
        : text_{text}
    {
    }

    std::string text() const
    {
        return text_;
    }
};

class MotifButton : public Button
{
public:
    using Button::Button;

    void draw() override
    {
        cout << "MotifButton [ " << caption() << " ]\n";
    }
};

class MotifMenu : public Menu
{
public:
    using Menu::Menu;

    void draw() override
    {
        cout << "MotifMenu { " << text() << " }\n";
    }
};

class WindowsButton : public Button
{
public:
    using Button::Button;

    void draw() override
    {
        cout << "WindowsButton [ " << caption() << " ]\n";
    }
};

class WindowsMenu : public Menu
{
public:
    using Menu::Menu;

    void draw() override
    {
        cout << "WindowsMenu { " << text() << " }\n";
    }
};

class MotifCreator : public WidgetCreator
{
public:
    std::unique_ptr<Button> createButton(const std::string& caption, IconType icon_type) override
    {
        auto button = std::make_unique<MotifButton>(caption, icon_type);
        return button;
    }

    std::unique_ptr<Menu> createMenu(const std::string& text) override
    {
        auto menu = std::make_unique<MotifMenu>(text);
        return menu;
    }

    virtual ~MotifCreator() = default;
};

class WindowsCreator : public WidgetCreator
{
public:
    std::unique_ptr<Button> createButton(const std::string& caption, IconType icon_type) override
    {
        auto button = std::make_unique<WindowsButton>(caption, icon_type);
        return button;
    }

    std::unique_ptr<Menu> createMenu(const std::string& text) override
    {
        auto menu = std::make_unique<WindowsMenu>(text);
        return menu;
    }

    virtual ~WindowsCreator() = default;
};

class Window
{
    std::vector<std::unique_ptr<Widget>> widgets;
    std::shared_ptr<WidgetCreator> creator_;

public:
    Window() = default;

    void display() const
    {
        std::cout << "######################\n";
        for (const auto& w : widgets)
            w->draw();
        std::cout << "######################\n\n";
    }

    void add_widget(std::unique_ptr<Widget> widget)
    {
        widgets.push_back(std::move(widget));
    }
};

#ifdef MOTIF
auto CREATOR = std::make_shared<MotifCreator>();
#else // WINDOWS
auto CREATOR = std::make_shared<WindowsCreator>();
#endif

class WindowOne : public Window
{
public:
    WindowOne(std::shared_ptr<WidgetCreator> creator)
    {
        add_widget(creator->createButton("OK", IconType::ok));
        add_widget(creator->createMenu("File"));
    }
};

class WindowTwo : public Window
{
public:
    WindowTwo(std::shared_ptr<WidgetCreator> creator)
    {
        add_widget(creator->createMenu("Edit"));
        add_widget(creator->createButton("OK", IconType::ok));
        add_widget(creator->createButton("Cancel", IconType::cancel));
    }
};

int main(void)
{
    WindowOne w1(CREATOR);
    w1.display();

    WindowTwo w2(CREATOR);
    w2.display();
}

namespace Version_2
{

    enum class IconType
    {
        none,
        ok,
        cancel,
        warning,
        error
    };

    class Widget
    {
    public:
        virtual void draw() = 0;
        virtual ~Widget() = default;
    };

    class Button : public Widget
    {
        std::string caption_;
        IconType icon_type_;

    public:
        Button(const std::string& caption, IconType icon_type)
            : caption_{caption}
            , icon_type_{icon_type}
        {
        }

        std::string caption() const
        {
            return caption_;
        }

        IconType icon() const
        {
            return icon_type_;
        }
    };

    class Menu : public Widget
    {
        std::string text_;

    public:
        Menu(const std::string& text)
            : text_{text}
        {
        }

        std::string text() const
        {
            return text_;
        }
    };

    class MotifButton : public Button
    {
    public:
        using Button::Button;

        void draw() override
        {
            cout << "MotifButton [ " << caption() << " ]\n";
        }
    };

    class MotifMenu : public Menu
    {
    public:
        using Menu::Menu;

        void draw() override
        {
            cout << "MotifMenu { " << text() << " }\n";
        }
    };

    class WindowsButton : public Button
    {
    public:
        using Button::Button;

        void draw() override
        {
            cout << "WindowsButton [ " << caption() << " ]\n";
        }
    };

    class WindowsMenu : public Menu
    {
    public:
        using Menu::Menu;

        void draw() override
        {
            cout << "WindowsMenu { " << text() << " }\n";
        }
    };

    class AbstractWidgetFactory
    {
    public:
        virtual std::unique_ptr<Menu> CreateMenu(const std::string& text) = 0;
        virtual std::unique_ptr<Button> CreateButton(const std::string& caption, IconType icon_type) = 0;

        virtual ~AbstractWidgetFactory() = default;
    };

    // enum class System
    // {
    //   LINUX,
    //   WINDOWS
    // };

    class WindowsWidgetFactory : public AbstractWidgetFactory
    {
    public:
        std::unique_ptr<Button> CreateButton(const std::string& caption, IconType icon_type) override
        {
            auto button = std::make_unique<WindowsButton>(caption, icon_type);
            return button;
        }

        std::unique_ptr<Menu> CreateMenu(const std::string& text) override
        {
            auto menu = std::make_unique<WindowsMenu>(text);
            return menu;
        }
    };

    class LinuxWidgetFactory : public AbstractWidgetFactory
    {
    public:
        std::unique_ptr<Button> CreateButton(const std::string& caption, IconType icon_type) override
        {
            auto button = std::make_unique<MotifButton>(caption, icon_type);
            return button;
        }

        std::unique_ptr<Menu> CreateMenu(const std::string& text) override
        {
            auto menu = std::make_unique<MotifMenu>(text);
            return menu;
        }
    };

    class Window
    {
        std::vector<std::unique_ptr<Widget>> widgets;

    public:
        static std::unique_ptr<AbstractWidgetFactory> getFactory()
        {
#ifdef MOTIF
            return std::make_unique<LinuxWidgetFactory>();
#else
            return std::make_unique<WindowsWidgetFactory>();
#endif
        }

        void display() const
        {
            std::cout << "######################\n";
            for (const auto& w : widgets)
                w->draw();
            std::cout << "######################\n\n";
        }

        void add_widget(std::unique_ptr<Widget> widget)
        {
            widgets.push_back(std::move(widget));
        }
    };

    class WindowOne : public Window
    {

    public:
        WindowOne()
        {
            add_widget(getFactory()->CreateButton("OK", IconType::ok));
            add_widget(getFactory()->CreateMenu("File"));
        }
    };

    class WindowTwo : public Window
    {

    public:
        WindowTwo()
        {
            add_widget(getFactory()->CreateButton("OK", IconType::ok));
            add_widget(getFactory()->CreateButton("Cancel", IconType::cancel));
            add_widget(getFactory()->CreateMenu("Edit"));
        }
    };

    int alternative_main(void)
    {
        WindowOne w1;
        w1.display();

        WindowTwo w2;
        w2.display();


        return 0;
    }
}
