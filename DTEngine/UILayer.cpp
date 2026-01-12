#include "pch.h"
#include "UILayer.h"
#include "UIManager.h"

BEGINPROPERTY(UILayer)
DTPROPERTY_ACCESSOR(UILayer, m_layerName, GetLayerName, SetLayerName)
DTPROPERTY_ACCESSOR(UILayer, m_layerOrder, GetLayerOrder, SetLayerOrder)
ENDPROPERTY()

void UILayer::SetLayerName(const std::string& name)
{
    if (name.empty())
    {
        m_layerName = "Default";
    }
    else
    {
        m_layerName = name;
    }

    UIManager::Instance().RegisterLayer(m_layerName, m_layerOrder);
}

void UILayer::SetLayerOrder(int order)
{
    m_layerOrder = order;
    UIManager::Instance().RegisterLayer(m_layerName, m_layerOrder);
}
