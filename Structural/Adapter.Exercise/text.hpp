#ifndef TEXT_HPP
#define TEXT_HPP

#include "paragraph.hpp"
#include "shape.hpp"

#include <string>

namespace Drawing
{
    class Text : public ShapeBase, private LegacyCode::Paragraph
    {
    public:
        static constexpr const char* id = "Text";

        Text(int x = 0, int y = 0, const std::string& text = "");
        void draw() const override;
        void set_text(const std::string& text);
        std::string get_text() const;
    };
} // namespace Drawing

#endif
