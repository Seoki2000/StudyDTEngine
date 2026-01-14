#include "pch.h"
#include "UISlider.h"
#include <algorithm>
#include <cmath>

BEGINPROPERTY(UISlider)
DTPROPERTY_ACCESSOR(UISlider, m_minValue, GetMinValue, SetMinValue)
DTPROPERTY_ACCESSOR(UISlider, m_maxValue, GetMaxValue, SetMaxValue)
DTPROPERTY_ACCESSOR(UISlider, m_value, GetValue, SetValue)
DTPROPERTY_ACCESSOR(UISlider, m_wholeNumbers, GetWholeNumbers, SetWholeNumbers)
DTPROPERTY_ACCESSOR(UISlider, m_interactable, GetInteractable, SetInteractable)
DTPROPERTY_ACCESSOR(UISlider, m_fillColor, GetFillColor, SetFillColor)
ENDPROPERTY()

void UISlider::SetValue(float value)
{
    if (m_wholeNumbers)
    {
        value = std::round(value);
    }

    float clamped = std::clamp(value, m_minValue, m_maxValue);

    if (clamped == m_value) return;

    m_value = clamped;
    InvokeValueChanged();
}

void UISlider::InvokeValueChanged()
{
    if (!m_interactable) return;
    if (m_onValueChanged) m_onValueChanged(m_value);
}
