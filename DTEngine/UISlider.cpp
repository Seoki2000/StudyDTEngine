#include "pch.h"
#include "UISlider.h"

BEGINPROPERTY(UISlider)
DTPROPERTY_ACCESSOR(UISlider, m_minValue, GetMinValue, SetMinValue)
DTPROPERTY_ACCESSOR(UISlider, m_maxValue, GetMaxValue, SetMaxValue)
DTPROPERTY_ACCESSOR(UISlider, m_value, GetValue, SetValue)
DTPROPERTY_ACCESSOR(UISlider, m_wholeNumbers, GetWholeNumbers, SetWholeNumbers)
DTPROPERTY_ACCESSOR(UISlider, m_interactable, GetInteractable, SetInteractable)
DTPROPERTY_ACCESSOR(UISlider, m_fillColor, GetFillColor, SetFillColor)
ENDPROPERTY()
