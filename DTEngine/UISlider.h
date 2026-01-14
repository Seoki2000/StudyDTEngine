#pragma once

#include "MonoBehaviour.h"
#include "SimpleMathHelper.h"
#include <functional>

class UISlider : public MonoBehaviour
{
    DTGENERATED_BODY(UISlider);

public:
    UISlider() = default;
    ~UISlider() override = default;

    void Awake() override;

    void SetMinValue(float value) { m_minValue = value; }
    float GetMinValue() const { return m_minValue; }

    void SetMaxValue(float value) { m_maxValue = value; }
    float GetMaxValue() const { return m_maxValue; }

    void SetValue(float value);
    float GetValue() const { return m_value; }

    void SetWholeNumbers(bool value) { m_wholeNumbers = value; }
    bool GetWholeNumbers() const { return m_wholeNumbers; }

    void SetInteractable(bool value) { m_interactable = value; }
    bool GetInteractable() const { return m_interactable; }

    void SetFillColor(const Vector4& value) { m_fillColor = value; }
    const Vector4& GetFillColor() const { return m_fillColor; }

    void SetOnValueChanged(std::function<void(float)> callback) { m_onValueChanged = std::move(callback); }
    void InvokeValueChanged();

private:
    void CacheHandle();
    void UpdateHandleVisual();

    class RectTransform* m_rectTransform = nullptr;
    class RectTransform* m_handleRect = nullptr;
    class Image* m_handleImage = nullptr;

    float m_minValue = 0.0f;
    float m_maxValue = 1.0f;
    float m_value = 0.0f;
    bool m_wholeNumbers = false;
    bool m_interactable = true;
    Vector4 m_fillColor = Vector4(1.f, 1.f, 1.f, 1.f);
    std::function<void(float)> m_onValueChanged;
};
