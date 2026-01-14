#include "pch.h"
#include "UILayoutGroup.h"
#include "RectTransform.h"
#include "Transform.h"
#include "GameObject.h"

BEGINPROPERTY(UIHorizontalLayout)
DTPROPERTY(UIHorizontalLayout, m_padding)
DTPROPERTY(UIHorizontalLayout, m_spacing)
DTPROPERTY(UIHorizontalLayout, m_alignment)
DTPROPERTY(UIHorizontalLayout, m_controlChildSize)
DTPROPERTY(UIHorizontalLayout, m_childForceExpandWidth)
DTPROPERTY(UIHorizontalLayout, m_childForceExpandHeight)
ENDPROPERTY()

BEGINPROPERTY(UIVerticalLayout)
DTPROPERTY(UIVerticalLayout, m_padding)
DTPROPERTY(UIVerticalLayout, m_spacing)
DTPROPERTY(UIVerticalLayout, m_alignment)
DTPROPERTY(UIVerticalLayout, m_controlChildSize)
DTPROPERTY(UIVerticalLayout, m_childForceExpandWidth)
DTPROPERTY(UIVerticalLayout, m_childForceExpandHeight)
ENDPROPERTY()

BEGINPROPERTY(UIGridLayout)
DTPROPERTY(UIGridLayout, m_padding)
DTPROPERTY(UIGridLayout, m_spacing)
DTPROPERTY(UIGridLayout, m_cellSize)
DTPROPERTY(UIGridLayout, m_constraintCount)
DTPROPERTY(UIGridLayout, m_constraint)
DTPROPERTY(UIGridLayout, m_alignment)
ENDPROPERTY()

int UILayoutGroup::GetHorizontalAlign(UIAlignment alignment)
{
    switch (alignment)
    {
    case UIAlignment::UpperLeft:
    case UIAlignment::MiddleLeft:
    case UIAlignment::LowerLeft:
        return 0;
    case UIAlignment::UpperCenter:
    case UIAlignment::MiddleCenter:
    case UIAlignment::LowerCenter:
        return 1;
    default:
        return 2;
    }
}

int UILayoutGroup::GetVerticalAlign(UIAlignment alignment)
{
    switch (alignment)
    {
    case UIAlignment::UpperLeft:
    case UIAlignment::UpperCenter:
    case UIAlignment::UpperRight:
        return 0;
    case UIAlignment::MiddleLeft:
    case UIAlignment::MiddleCenter:
    case UIAlignment::MiddleRight:
        return 1;
    default:
        return 2;
    }
}

static std::vector<RectTransform*> CollectChildRects(RectTransform* parentRect)
{
    std::vector<RectTransform*> children;
    if (!parentRect) return children;

    Transform* parentTf = parentRect->GetTransform();
    if (!parentTf) return children;

    for (Transform* child : parentTf->GetChildren())
    {
        if (!child) continue;
        RectTransform* rect = child->_GetOwner()->GetComponent<RectTransform>();
        if (rect) children.push_back(rect);
    }

    return children;
}

void UIHorizontalLayout::ApplyLayout(RectTransform* parentRect, float, float)
{
    if (!parentRect) return;

    std::vector<RectTransform*> children = CollectChildRects(parentRect);
    if (children.empty()) return;

    Vector2 parentSize = parentRect->GetSize();
    float parentWidth = parentSize.x;
    float parentHeight = parentSize.y;

    float paddingLeft = m_padding.x;
    float paddingTop = m_padding.y;
    float paddingRight = m_padding.z;
    float paddingBottom = m_padding.w;

    std::vector<Vector2> childSizes;
    childSizes.reserve(children.size());

    float totalWidth = 0.0f;
    for (RectTransform* child : children)
    {
        Vector2 size = child->GetSizeDelta();
        if (m_childForceExpandWidth)
        {
            size.x = 0.0f;
        }
        if (m_childForceExpandHeight)
        {
            size.y = parentHeight - paddingTop - paddingBottom;
        }
        childSizes.push_back(size);
        totalWidth += size.x;
    }

    if (m_childForceExpandWidth)
    {
        float availableWidth = parentWidth - paddingLeft - paddingRight - (m_spacing * (children.size() - 1));
        float size = (children.empty()) ? 0.0f : availableWidth / static_cast<float>(children.size());
        for (auto& childSize : childSizes)
        {
            childSize.x = size;
        }
        totalWidth = availableWidth;
    }

    totalWidth += m_spacing * (children.size() - 1);

    float startX = 0.0f;
    int hAlign = GetHorizontalAlign(static_cast<UIAlignment>(m_alignment));
    if (hAlign == 0)
    {
        startX = -parentWidth * 0.5f + paddingLeft;
    }
    else if (hAlign == 1)
    {
        startX = -totalWidth * 0.5f;
    }
    else
    {
        startX = parentWidth * 0.5f - paddingRight - totalWidth;
    }

    int vAlign = GetVerticalAlign(static_cast<UIAlignment>(m_alignment));
    float baseY = 0.0f;
    float firstHeight = childSizes.empty() ? 0.0f : childSizes.front().y;

    if (vAlign == 0)
    {
        baseY = parentHeight * 0.5f - paddingTop - firstHeight * 0.5f;
    }
    else if (vAlign == 1)
    {
        baseY = 0.0f;
    }
    else
    {
        baseY = -parentHeight * 0.5f + paddingBottom + firstHeight * 0.5f;
    }

    float cursorX = startX;
    for (size_t i = 0; i < children.size(); ++i)
    {
        RectTransform* child = children[i];
        Vector2 size = childSizes[i];

        if (m_controlChildSize)
        {
            child->SetSizeDelta(size);
        }

        child->SetAnchorMin(Vector2(0.5f, 0.5f));
        child->SetAnchorMax(Vector2(0.5f, 0.5f));
        child->SetPivot(Vector2(0.5f, 0.5f));

        float centerX = cursorX + size.x * 0.5f;
        child->SetAnchoredPosition(Vector2(centerX, baseY));

        cursorX += size.x + m_spacing;
    }
}

void UIVerticalLayout::ApplyLayout(RectTransform* parentRect, float, float)
{
    if (!parentRect) return;

    std::vector<RectTransform*> children = CollectChildRects(parentRect);
    if (children.empty()) return;

    Vector2 parentSize = parentRect->GetSize();
    float parentWidth = parentSize.x;
    float parentHeight = parentSize.y;

    float paddingLeft = m_padding.x;
    float paddingTop = m_padding.y;
    float paddingRight = m_padding.z;
    float paddingBottom = m_padding.w;

    std::vector<Vector2> childSizes;
    childSizes.reserve(children.size());

    float totalHeight = 0.0f;
    for (RectTransform* child : children)
    {
        Vector2 size = child->GetSizeDelta();
        if (m_childForceExpandHeight)
        {
            size.y = 0.0f;
        }
        if (m_childForceExpandWidth)
        {
            size.x = parentWidth - paddingLeft - paddingRight;
        }
        childSizes.push_back(size);
        totalHeight += size.y;
    }

    if (m_childForceExpandHeight)
    {
        float availableHeight = parentHeight - paddingTop - paddingBottom - (m_spacing * (children.size() - 1));
        float size = (children.empty()) ? 0.0f : availableHeight / static_cast<float>(children.size());
        for (auto& childSize : childSizes)
        {
            childSize.y = size;
        }
        totalHeight = availableHeight;
    }

    totalHeight += m_spacing * (children.size() - 1);

    float startY = 0.0f;
    int vAlign = GetVerticalAlign(static_cast<UIAlignment>(m_alignment));
    if (vAlign == 0)
    {
        startY = parentHeight * 0.5f - paddingTop;
    }
    else if (vAlign == 1)
    {
        startY = totalHeight * 0.5f;
    }
    else
    {
        startY = -parentHeight * 0.5f + paddingBottom + totalHeight;
    }

    int hAlign = GetHorizontalAlign(static_cast<UIAlignment>(m_alignment));
    float baseX = 0.0f;
    float firstWidth = childSizes.empty() ? 0.0f : childSizes.front().x;

    if (hAlign == 0)
    {
        baseX = -parentWidth * 0.5f + paddingLeft + firstWidth * 0.5f;
    }
    else if (hAlign == 1)
    {
        baseX = 0.0f;
    }
    else
    {
        baseX = parentWidth * 0.5f - paddingRight - firstWidth * 0.5f;
    }

    float cursorY = startY;
    for (size_t i = 0; i < children.size(); ++i)
    {
        RectTransform* child = children[i];
        Vector2 size = childSizes[i];

        if (m_controlChildSize)
        {
            child->SetSizeDelta(size);
        }

        child->SetAnchorMin(Vector2(0.5f, 0.5f));
        child->SetAnchorMax(Vector2(0.5f, 0.5f));
        child->SetPivot(Vector2(0.5f, 0.5f));

        float centerY = cursorY - size.y * 0.5f;
        child->SetAnchoredPosition(Vector2(baseX, centerY));

        cursorY -= size.y + m_spacing;
    }
}

void UIGridLayout::ApplyLayout(RectTransform* parentRect, float, float)
{
    if (!parentRect) return;

    std::vector<RectTransform*> children = CollectChildRects(parentRect);
    if (children.empty()) return;

    Vector2 parentSize = parentRect->GetSize();
    float parentWidth = parentSize.x;
    float parentHeight = parentSize.y;

    float paddingLeft = m_padding.x;
    float paddingTop = m_padding.y;
    float paddingRight = m_padding.z;
    float paddingBottom = m_padding.w;

    int count = static_cast<int>(children.size());
    int constraintCount = std::max(1, m_constraintCount);

    int columns = 1;
    int rows = 1;

    if (m_constraint == static_cast<int>(UIGridConstraint::FixedRowCount))
    {
        rows = constraintCount;
        columns = (count + rows - 1) / rows;
    }
    else
    {
        columns = constraintCount;
        rows = (count + columns - 1) / columns;
    }

    float gridWidth = columns * m_cellSize.x + (columns - 1) * m_spacing.x;
    float gridHeight = rows * m_cellSize.y + (rows - 1) * m_spacing.y;

    int hAlign = GetHorizontalAlign(static_cast<UIAlignment>(m_alignment));
    int vAlign = GetVerticalAlign(static_cast<UIAlignment>(m_alignment));

    float startX = 0.0f;
    if (hAlign == 0)
    {
        startX = -parentWidth * 0.5f + paddingLeft;
    }
    else if (hAlign == 1)
    {
        startX = -gridWidth * 0.5f;
    }
    else
    {
        startX = parentWidth * 0.5f - paddingRight - gridWidth;
    }

    float startY = 0.0f;
    if (vAlign == 0)
    {
        startY = parentHeight * 0.5f - paddingTop;
    }
    else if (vAlign == 1)
    {
        startY = gridHeight * 0.5f;
    }
    else
    {
        startY = -parentHeight * 0.5f + paddingBottom + gridHeight;
    }

    for (int i = 0; i < count; ++i)
    {
        int row = i / columns;
        int col = i % columns;

        RectTransform* child = children[i];
        child->SetSizeDelta(m_cellSize);
        child->SetAnchorMin(Vector2(0.5f, 0.5f));
        child->SetAnchorMax(Vector2(0.5f, 0.5f));
        child->SetPivot(Vector2(0.5f, 0.5f));

        float x = startX + col * (m_cellSize.x + m_spacing.x) + m_cellSize.x * 0.5f;
        float y = startY - row * (m_cellSize.y + m_spacing.y) - m_cellSize.y * 0.5f;

        child->SetAnchoredPosition(Vector2(x, y));
    }
}
