////////////////////////////////////////////////////////////////////////////////
//
// TreeEvent  - обработчик событий дерева
//
////////////////////////////////////////////////////////////////////////////////
#include "PropUserEvent.h"

#include "Resource.h"

#include <string>
#include <KsAPI.h>


////////////////////////////////////////////////////////////////////////////////
//
// TreeEvent  - обработчик событий дерева
//
////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------
// Конструктор
// ---
TreeEvent::TreeEvent(IPropertyTreePtr & tree, TreeEventCallBack & obj)
  : BaseEvent(tree)
  , m_obj(obj)
{
  Advise();
}


//-------------------------------------------------------------------------------
// Деструктор
// ---
TreeEvent::~TreeEvent()
{
  Unadvise();
}


//-------------------------------------------------------------------------------
// Подписаться на события.
// ---
void TreeEvent::Advise()
{
  if (IPropertyTreePtr tree = m_container)
  {
    std::wstring handlerOwner = L"IPropertyTree_Sample";
    IPropertyTreeEventsPtr events = tree->Events();

    if (!events)
      return;

    events->AddSelectNodesHandler(handlerOwner,
                                  [this](const std::vector<IPropertyTreeNodePtr> & nodes) { return this->SelectNodes(nodes); });
    events->AddUnselectNodesHandler(handlerOwner, [this](const std::vector<IPropertyTreeNodePtr> & nodes)
                                    { return this->UnSelectNodes(nodes); });
    events->AddCollapseNodeHandler(handlerOwner, [this](const IPropertyTreeNodePtr & node) { return this->Collapse(node); });
    events->AddExpandNodeHandler(handlerOwner, [this](const IPropertyTreeNodePtr & node) { return this->Expand(node); });
    events->AddNodeDblClickHandler(handlerOwner, [this](const IPropertyTreeNodePtr & node) { return this->NodeDblClick(node); });
    events->AddBeginTextEditHandler(handlerOwner,
                                    [this](const IPropertyTreeNodePtr & node) { return this->BeginTextEdit(node); });
    events->AddEndTextEditHandler(handlerOwner, [this](const IPropertyTreeNodePtr & node) { return this->EndTextEdit(node); });
    events->AddNodeDragStartedHandler(handlerOwner,
                                      [this](const IPropertyTreeNodePtr & node) { return this->NodeDragStarted(node); });
    events->AddNodeDragOverHandler(handlerOwner, [this](const IPropertyTreeNodePtr & node, const IPropertyTreeNodePtr & dragNode,
                                                        bool ctrlPressed, bool requiredDropAfter)
                                   { return this->NodeDragOver(node, dragNode, ctrlPressed, requiredDropAfter); });
    events->AddNodeDragDropedHandler(handlerOwner,
                                     [this](const IPropertyTreeNodePtr & node, const IPropertyTreeNodePtr & dragNode,
                                            bool ctrlPressed) { return this->NodeDragDroped(node, dragNode, ctrlPressed); });
    events->AddContextMenuRequestHandler(handlerOwner,
                                         [this](const IPropertyTreeNodePtr & node) { return this->ContextMenuRequest(node); });
  }
}


//-------------------------------------------------------------------------------
// Отписаться от событий.
// ---
void TreeEvent::Unadvise()
{
  if (IPropertyTreePtr tree = m_container)
    tree->Events()->RemoveAllHandlers(L"IPropertyTree_Sample");
}


//-----------------------------------------------------------------------------
///
// ---
bool TreeEvent::Collapse(const IPropertyTreeNodePtr & node)
{
  return node ? m_obj.OnExpandOrCollapse(node, true) : false;
}


//-----------------------------------------------------------------------------
///
// ---
bool TreeEvent::Expand(const IPropertyTreeNodePtr & node)
{
  return node ? m_obj.OnExpandOrCollapse(node, false) : false;
}


//-----------------------------------------------------------------------------
///
// ---
bool TreeEvent::SelectNodes(const std::vector<IPropertyTreeNodePtr> & nodes)
{
  return m_obj.OnSelectNodes(nodes, true);
}


//-----------------------------------------------------------------------------
///
// ---
bool TreeEvent::UnSelectNodes(const std::vector<IPropertyTreeNodePtr> & nodes)
{
  return m_obj.OnSelectNodes(nodes, false);
}


//-----------------------------------------------------------------------------
///
// ---
bool TreeEvent::NodeDragStarted(const IPropertyTreeNodePtr & node)
{
  return m_obj.OnNodeDragStarted(node);
}


//-----------------------------------------------------------------------------
///
// ---
bool TreeEvent::NodeDragOver(const IPropertyTreeNodePtr & node, const IPropertyTreeNodePtr & dragNode, bool ctrlPressed,
                             bool requiredDropAfter)
{
  return m_obj.OnNodeDragOver(node, dragNode, ctrlPressed, requiredDropAfter);
}


//-----------------------------------------------------------------------------
///
// ---
bool TreeEvent::NodeDragDroped(const IPropertyTreeNodePtr & node, const IPropertyTreeNodePtr & dragNode, bool ctrlPressed)
{
  return m_obj.OnNodeDragDroped(node, dragNode, ctrlPressed);
}


//-----------------------------------------------------------------------------
///
// ---
bool TreeEvent::ContextMenuRequest(const IPropertyTreeNodePtr & node)
{
  return m_obj.OnContextMenuRequest(node);
}


//-----------------------------------------------------------------------------
///
// ---
bool TreeEvent::NodeDblClick(const IPropertyTreeNodePtr & node)
{
  return m_obj.OnNodeDblClick(node);
}


//-----------------------------------------------------------------------------
///
// ---
bool TreeEvent::BeginTextEdit(const IPropertyTreeNodePtr & node)
{
  return m_obj.OnBeginEditText(node);
}


//-----------------------------------------------------------------------------
///
// ---
bool TreeEvent::EndTextEdit(const IPropertyTreeNodePtr & node)
{
  return true;
}
