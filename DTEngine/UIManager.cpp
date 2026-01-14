#include "pch.h"
#include "UIManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "RectTransform.h"
#include "Canvas.h"

void UIManager::OnResize(float width, float height)
{
    EnsureDefaultLayers();
    m_lastWidth = width;
    m_lastHeight = height;
}

void UIManager::UpdateLayout(Scene* scene, float width, float height)
{
    if (!scene || width <= 0.0f || height <= 0.0f) return;

    EnsureDefaultLayers();

    if (m_lastWidth != width || m_lastHeight != height)
    {
        m_lastWidth = width;
        m_lastHeight = height;
    }

    std::vector<RectTransform*> roots;
    std::vector<RectTransform*> canvasRoots;

    auto collectRectChildren = [&](Transform* parent, auto&& collectRef) -> void {
        if (!parent) return;

        for (Transform* child : parent->GetChildren())
        {
            if (!child) continue;
            if (auto* rect = child->_GetOwner()->GetComponent<RectTransform>())
            {
                canvasRoots.push_back(rect);
            }
            else
            {
                collectRef(child, collectRef);
            }
        }
    };

    const auto& gameObjects = scene->GetGameObjects();
    for (const auto& go : gameObjects)
    {
        if (!go || !go->IsActiveInHierarchy()) continue;

        if (go->GetComponent<Canvas>())
        {
            if (auto* rect = go->GetComponent<RectTransform>())
            {
                canvasRoots.push_back(rect);
            }
            else if (auto* tf = go->GetTransform())
            {
                collectRectChildren(tf, collectRectChildren);
            }
            continue;
        }

        RectTransform* rect = go->GetComponent<RectTransform>();
        if (!rect) continue;

        Transform* tf = rect->GetTransform();
        Transform* parent = tf ? tf->GetParent() : nullptr;
        RectTransform* parentRect = parent ? parent->_GetOwner()->GetComponent<RectTransform>() : nullptr;
        if (!parentRect)
        {
            roots.push_back(rect);
        }
    }

    if (!canvasRoots.empty())
    {
        for (RectTransform* root : canvasRoots)
        {
            root->ApplyLayoutRecursive(width, height);
        }
        return;
    }

    for (RectTransform* root : roots)
    {
        root->ApplyLayoutRecursive(width, height);
    }
}

void UIManager::RegisterLayer(const std::string& name, int order)
{
    EnsureDefaultLayers();
    if (name.empty()) return;

    m_layerOrders[name] = order;
}

int UIManager::GetLayerOrder(const std::string& name) const
{
    auto it = m_layerOrders.find(name);
    if (it != m_layerOrders.end())
    {
        return it->second;
    }
    return 0;
}

void UIManager::EnsureDefaultLayers()
{
    if (m_layerOrders.empty())
    {
        m_layerOrders["Default"] = 0;
        m_layerOrders["Last"] = 100;
    }
}
