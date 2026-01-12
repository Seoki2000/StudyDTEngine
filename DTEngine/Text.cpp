#include "pch.h"
#include "Text.h"
#include "DX11Renderer.h"
#include "GameObject.h"
#include "Transform.h" 
#include "RectTransform.h"

BEGINPROPERTY(Text)

DTPROPERTY(Text, m_text)
DTPROPERTY(Text, m_color)
DTPROPERTY(Text, m_localOffset)
DTPROPERTY(Text, m_fontSize)

ENDPROPERTY()

Text::Text() = default;
Text::~Text() = default;

void Text::SetText(const std::string& text)
{
    m_text.assign(text.begin(), text.end());
}

void Text::SetText(const std::wstring& text)
{
    m_text = text;
}

void Text::Render()
{
    if (m_text.empty()) return;

    Vector2 position = m_localOffset;

    if (RectTransform* rect = GetComponent<RectTransform>())
    {
        float width = DX11Renderer::Instance().GetUIRenderWidth();
        float height = DX11Renderer::Instance().GetUIRenderHeight();

        if (width <= 0.0f || height <= 0.0f)
        {
            width = static_cast<float>(DX11Renderer::Instance().GetWidth());
            height = static_cast<float>(DX11Renderer::Instance().GetHeight());
        }

        position = rect->GetScreenPosition(width, height) + m_localOffset;
    }

    DX11Renderer::Instance().DrawString(m_text, position, m_fontSize, m_color);
}
