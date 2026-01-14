#include "pch.h"
#include "RectTransform.h"
#include "Transform.h"
#include "GameObject.h"
#include "UILayoutGroup.h"
#include "Canvas.h"

BEGINPROPERTY(RectTransform)
DTPROPERTY_ACCESSOR(RectTransform, m_anchorMin, GetAnchorMin, SetAnchorMin)
DTPROPERTY_ACCESSOR(RectTransform, m_anchorMax, GetAnchorMax, SetAnchorMax)
DTPROPERTY_ACCESSOR(RectTransform, m_pivot, GetPivot, SetPivot)
DTPROPERTY_ACCESSOR(RectTransform, m_sizeDelta, GetSizeDelta, SetSizeDelta)
DTPROPERTY_ACCESSOR(RectTransform, m_anchoredPosition, GetAnchoredPosition, SetAnchoredPosition)
ENDPROPERTY()

void RectTransform::SetAnchorMin(const Vector2& value)
{
    m_anchorMin = value;
}

void RectTransform::SetAnchorMax(const Vector2& value)
{
    m_anchorMax = value;
}

void RectTransform::SetPivot(const Vector2& value)
{
    m_pivot = value;
}

void RectTransform::SetSizeDelta(const Vector2& value)
{
    m_sizeDelta = value;
}

void RectTransform::SetAnchoredPosition(const Vector2& value)
{
    m_anchoredPosition = value;
}

RectTransform* RectTransform::GetParentRect() const
{
    Transform* tf = GetTransform();
    if (!tf) return nullptr;

    Transform* parent = tf->GetParent();
    if (!parent) return nullptr;

    return parent->_GetOwner()->GetComponent<RectTransform>();
}

static Canvas* GetCanvasInHierarchy(Transform* start)
{
    Transform* current = start;
    while (current)
    {
        if (auto* canvas = current->_GetOwner()->GetComponent<Canvas>())
        {
            return canvas;
        }
        current = current->GetParent();
    }
    return nullptr;
}

void RectTransform::ApplyLayout(float screenWidth, float screenHeight)
{
    if (screenWidth <= 0.0f || screenHeight <= 0.0f) return;

    RectTransform* parentRect = GetParentRect();

    Vector2 parentSize = Vector2(screenWidth, screenHeight);
    Vector2 parentCenter = Vector2(0.0f, 0.0f);

    if (parentRect)
    {
        parentSize = parentRect->GetSize();
        parentCenter = parentRect->GetWorldCenter2D();
    }
    else
    {
        Transform* tf = GetTransform();
        Canvas* canvas = tf ? GetCanvasInHierarchy(tf) : nullptr;
        if (canvas)
        {
            parentSize = canvas->GetCanvasSize(screenWidth, screenHeight);
        }
    }

    Vector2 parentMin = Vector2(-parentSize.x * 0.5f, -parentSize.y * 0.5f);
    Vector2 anchorMinPos = parentMin + Vector2(parentSize.x * m_anchorMin.x, parentSize.y * m_anchorMin.y);
    Vector2 anchorMaxPos = parentMin + Vector2(parentSize.x * m_anchorMax.x, parentSize.y * m_anchorMax.y);

    Vector2 size = (anchorMaxPos - anchorMinPos) + m_sizeDelta;
    Vector2 anchorCenter = (anchorMinPos + anchorMaxPos) * 0.5f;

    Vector2 pivotOffset = Vector2((0.5f - m_pivot.x) * size.x, (0.5f - m_pivot.y) * size.y);
    Vector2 localCenter = anchorCenter + m_anchoredPosition + pivotOffset;
    Vector2 worldCenter = parentCenter + localCenter;

    Transform* tf = GetTransform();
    if (tf)
    {
        Vector3 position = tf->GetPosition();
        position.x = worldCenter.x;
        position.y = worldCenter.y;
        tf->SetPosition(position);

        Vector3 scale = tf->GetScale();
        scale.x = size.x;
        scale.y = size.y;
        tf->SetScale(scale);
    }

    m_cachedSize = size;
    m_cachedWorldCenter = worldCenter;
}

void RectTransform::ApplyLayoutRecursive(float screenWidth, float screenHeight)
{
    ApplyLayout(screenWidth, screenHeight);

    if (auto* layout = GetComponent<UILayoutGroup>())
    {
        layout->ApplyLayout(this, screenWidth, screenHeight);
    }

    Transform* tf = GetTransform();
    if (!tf) return;

    for (Transform* child : tf->GetChildren())
    {
        if (!child) continue;
        RectTransform* childRect = child->_GetOwner()->GetComponent<RectTransform>();
        if (childRect)
        {
            childRect->ApplyLayoutRecursive(screenWidth, screenHeight);
        }
    }
}

Vector2 RectTransform::GetScreenPosition(float screenWidth, float screenHeight) const
{
    Vector2 screenCenter = Vector2(screenWidth * 0.5f, screenHeight * 0.5f);
    return Vector2(screenCenter.x + m_cachedWorldCenter.x, screenCenter.y - m_cachedWorldCenter.y);
}
