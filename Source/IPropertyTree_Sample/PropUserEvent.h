////////////////////////////////////////////////////////////////////////////////
//
// TreeEvent  - обработчик событий дерева
//
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "BaseEvent.h"


//-----------------------------------------------------------------------------
//
// ---
class TreeEventCallBack
{
public:
  virtual bool OnExpandOrCollapse(const IPropertyTreeNodePtr & node, bool collapse) = 0;
  virtual bool OnSelectNodes(const std::vector<IPropertyTreeNodePtr> & nodes, bool selected) = 0;
  virtual bool OnNodeDblClick(const IPropertyTreeNodePtr & node) = 0;
  virtual bool OnBeginEditText(const IPropertyTreeNodePtr & node) = 0;
  virtual bool OnNodeDragStarted(const IPropertyTreeNodePtr & node) = 0;
  virtual bool OnNodeDragOver(const IPropertyTreeNodePtr & node, const IPropertyTreeNodePtr & dragNode, bool ctrlPressed,
                              bool requiredDropAfter) = 0;
  virtual bool OnNodeDragDroped(const IPropertyTreeNodePtr & node, const IPropertyTreeNodePtr & dragNode, bool ctrlPressed) = 0;
  virtual bool OnContextMenuRequest(const IPropertyTreeNodePtr & node) = 0;
};


//-----------------------------------------------------------------------------
// TreeViewEvent  - обработчик событий дерева
// ---
class TreeEvent : public BaseEvent
{
  TreeEventCallBack & m_obj;

public:
  TreeEvent(IPropertyTreePtr & tree, TreeEventCallBack & obj);
  virtual ~TreeEvent();

  // Подписаться на события.
  void Advise();
  // Отписаться от событий.
  virtual void Unadvise() override;

protected:
  bool Collapse(const IPropertyTreeNodePtr & node);
  bool Expand(const IPropertyTreeNodePtr & node);
  bool NodeDblClick(const IPropertyTreeNodePtr & node);
  bool BeginTextEdit(const IPropertyTreeNodePtr & node);
  bool EndTextEdit(const IPropertyTreeNodePtr & node);
  bool SelectNodes(const std::vector<IPropertyTreeNodePtr> & nodes);
  bool UnSelectNodes(const std::vector<IPropertyTreeNodePtr> & nodes);
  bool NodeDragStarted(const IPropertyTreeNodePtr & node);
  bool NodeDragOver(const IPropertyTreeNodePtr & node, const IPropertyTreeNodePtr & dragNode, bool ctrlPressed,
                    bool requiredDropAfter);
  bool NodeDragDroped(const IPropertyTreeNodePtr & node, const IPropertyTreeNodePtr & dragNode, bool ctrlPressed);
  bool ContextMenuRequest(const IPropertyTreeNodePtr & node);
};
