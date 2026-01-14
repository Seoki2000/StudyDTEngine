#include "pch.h"
#include "UIButton.h"

BEGINPROPERTY(UIButton)
DTPROPERTY_ACCESSOR(UIButton, m_interactable, GetInteractable, SetInteractable)
DTPROPERTY_ACCESSOR(UIButton, m_normalColor, GetNormalColor, SetNormalColor)
DTPROPERTY_ACCESSOR(UIButton, m_hoverColor, GetHoverColor, SetHoverColor)
DTPROPERTY_ACCESSOR(UIButton, m_pressedColor, GetPressedColor, SetPressedColor)
ENDPROPERTY()

void UIButton::InvokeClick()
{
    if (!m_interactable) return;
    if (m_onClick) m_onClick();
}
