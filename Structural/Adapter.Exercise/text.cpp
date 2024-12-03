#include "text.hpp"

#include "paragraph.hpp"
#include "shape.hpp"
#include "shape_factories.hpp"

using namespace Drawing;

namespace
{
    bool is_registered = SingletonShapeFactory::instance().register_creator(
        Text::id, []
        { return std::make_unique<Text>(); });
}

Text::Text(int x, int y, const std::string& text)
    : ShapeBase(x, y)
    , LegacyCode::Paragraph(text.c_str())
{
}

void Text::draw() const
{
    render_at(coord().x, coord().y);
}

void Text::set_text(const std::string& text)
{
    set_paragraph(text.c_str());
}

std::string Text::get_text() const
{
    return get_paragraph();
}
