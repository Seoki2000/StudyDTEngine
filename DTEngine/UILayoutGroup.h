#pragma once

#include "MonoBehaviour.h"
#include "SimpleMathHelper.h"

class RectTransform;

enum class UIAlignment
{
    UpperLeft = 0,
    UpperCenter = 1,
    UpperRight = 2,
    MiddleLeft = 3,
    MiddleCenter = 4,
    MiddleRight = 5,
    LowerLeft = 6,
    LowerCenter = 7,
    LowerRight = 8
};

class UILayoutGroup : public MonoBehaviour
{
public:
    virtual ~UILayoutGroup() = default;

    virtual void ApplyLayout(RectTransform* parentRect, float screenWidth, float screenHeight) {}

protected:
    static int GetHorizontalAlign(UIAlignment alignment);
    static int GetVerticalAlign(UIAlignment alignment);
};

class UIHorizontalLayout : public UILayoutGroup
{
    DTGENERATED_BODY(UIHorizontalLayout);

public:
    void ApplyLayout(RectTransform* parentRect, float screenWidth, float screenHeight) override;

private:
    Vector4 m_padding = Vector4(0.0f, 0.0f, 0.0f, 0.0f); // left, top, right, bottom
    float m_spacing = 0.0f;
    int m_alignment = static_cast<int>(UIAlignment::UpperLeft);
    bool m_controlChildSize = true;
    bool m_childForceExpandWidth = false;
    bool m_childForceExpandHeight = false;
};

class UIVerticalLayout : public UILayoutGroup
{
    DTGENERATED_BODY(UIVerticalLayout);

public:
    void ApplyLayout(RectTransform* parentRect, float screenWidth, float screenHeight) override;

private:
    Vector4 m_padding = Vector4(0.0f, 0.0f, 0.0f, 0.0f); // left, top, right, bottom
    float m_spacing = 0.0f;
    int m_alignment = static_cast<int>(UIAlignment::UpperLeft);
    bool m_controlChildSize = true;
    bool m_childForceExpandWidth = false;
    bool m_childForceExpandHeight = false;
};

enum class UIGridConstraint
{
    FixedColumnCount = 0,
    FixedRowCount = 1
};

class UIGridLayout : public UILayoutGroup
{
    DTGENERATED_BODY(UIGridLayout);

public:
    void ApplyLayout(RectTransform* parentRect, float screenWidth, float screenHeight) override;

private:
    Vector4 m_padding = Vector4(0.0f, 0.0f, 0.0f, 0.0f); // left, top, right, bottom
    Vector2 m_spacing = Vector2(0.0f, 0.0f);
    Vector2 m_cellSize = Vector2(100.0f, 100.0f);
    int m_constraintCount = 2;
    int m_constraint = static_cast<int>(UIGridConstraint::FixedColumnCount);
    int m_alignment = static_cast<int>(UIAlignment::UpperLeft);
};
