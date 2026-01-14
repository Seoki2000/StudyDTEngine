#pragma once

#include "MonoBehaviour.h"
#include "SimpleMathHelper.h"

class UIButton : public MonoBehaviour
{
    DTGENERATED_BODY(UIButton);

public:
    UIButton() = default;
    ~UIButton() override = default;

    void SetInteractable(bool value) { m_interactable = value; }
    bool GetInteractable() const { return m_interactable; }

    void SetNormalColor(const Vector4& value) { m_normalColor = value; }
    const Vector4& GetNormalColor() const { return m_normalColor; }

    void SetHoverColor(const Vector4& value) { m_hoverColor = value; }
    const Vector4& GetHoverColor() const { return m_hoverColor; }

    void SetPressedColor(const Vector4& value) { m_pressedColor = value; }
    const Vector4& GetPressedColor() const { return m_pressedColor; }

private:
    bool m_interactable = true;
    Vector4 m_normalColor = Vector4(1.f, 1.f, 1.f, 1.f);
    Vector4 m_hoverColor = Vector4(0.8f, 0.8f, 0.8f, 1.f);
    Vector4 m_pressedColor = Vector4(0.6f, 0.6f, 0.6f, 1.f);
};
