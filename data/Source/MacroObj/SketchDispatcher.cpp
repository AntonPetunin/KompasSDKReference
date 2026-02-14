////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////


#include <MacroObj_pch.h>
#include "SketchDispatcher.h"

extern ksapi::IApplication * ksAppl;

//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
SketchDispatcher::SketchDispatcher()
{
}


void SketchDispatcher::SetSketch(const ksapi::IKompasDocument2DPtr & sketch)
{
  m_sketch = sketch;
}

//------------------------------------------------------------------------------
/**
  Создать окружность по центру и радиусу
*/
//---
IDrawingObjectPtr SketchDispatcher::CreateCircle(double xc, double yc, double rad, unsigned short style)
{
  IDrawingContainerPtr drawCont = GetContainer();
  ICirclePtr circle = drawCont->GetCircles()->Add();

  circle->SetStyle(ksCSNormal);
  circle->SetXc(xc);
  circle->SetYc(yc);
  circle->SetRadius(rad);
  circle->SetStyle(style);
  circle->Update();

  return circle;
}

//------------------------------------------------------------------------------
/**
  Создать группу
*/
//---
IDrawingGroupPtr SketchDispatcher::CreateDrawingGroup(bool tmpGroup)
{
  if (IKompasDocument2DPtr kDoc2D = m_sketch)
  {
    IDrawingGroupsPtr drawGroups = kDoc2D->GetDrawingGroups();
    IDrawingGroupPtr drawGroup = drawGroups->Add(tmpGroup, L"");
    drawGroup->Open();
    m_groups.push(drawGroup);
    return drawGroup;
  }

  return nullptr;
}


//------------------------------------------------------------------------------
/**
  Добавить объект в группу
*/
//---
void SketchDispatcher::AddObjGroup(ksapi::IDrawingGroupPtr & group, ksapi::IMacroObjectPtr & macroObject)
{
  if (macroObject && group)
    group->AddObjects({macroObject});
}


//------------------------------------------------------------------------------
/**
  Открыть группу
*/
//---
ksapi::IDrawingGroupPtr SketchDispatcher::OpenDrawingGroup(IDrawingGroupPtr & createdGroup)
{
  if (createdGroup)
  {
    createdGroup->Open();
    m_groups.push(createdGroup);
    return createdGroup;
  }

  return nullptr;
}


//------------------------------------------------------------------------------
/**
  Очистить группу
*/
//---
void SketchDispatcher::DeleteDrawingGroup(ksapi::IDrawingGroupPtr & group)
{
  if (group)
    group->Delete();
}


//------------------------------------------------------------------------------
/**
  Закрыть редактирование группы
*/
//---
IDrawingGroupPtr SketchDispatcher::CloseGroup()
{
  if (!m_groups.empty())
  {
    IDrawingGroupPtr group = m_groups.top();
    m_groups.pop();
    group->Close();
    return group;
  }

  return nullptr;
}


ksapi::IDrawingObjectPtr SketchDispatcher::CreateDiamDimension(double xc, double yc, double rad, std::int32_t arrowType,
                                                               std::int32_t sign)
{
  IDrawingObjectPtr res;

  if (m_sketch)
  {
    ksapi::IViewsAndLayersManagerPtr viewsManager = m_sketch->GetViewsAndLayersManager();
    ksapi::IViewsPtr views = viewsManager ? viewsManager->GetViews() : nullptr;
    ksapi::ISymbols2DContainerPtr symbols2dContainer = views ? views->GetActiveView() : nullptr;
    ksapi::IDiametralDimensionsPtr diametralDims = symbols2dContainer ? symbols2dContainer->GetDiametralDimensions() : nullptr;

    if (ksapi::IDiametralDimensionPtr diametralDim = diametralDims ? diametralDims->Add() : nullptr)
    {
      // изменить положение
      diametralDim->SetXc(xc);
      diametralDim->SetYc(yc);
      diametralDim->SetRadius(rad);

      if (ksapi::IDimensionParamsPtr params = diametralDim)
        params->SetArrowPos(static_cast<ksDimensionArrowPosEnum>(arrowType));

      if (ksapi::IDimensionTextPtr text = diametralDim)
        text->SetSign(sign);

      diametralDim->Update();
      res = diametralDim;
    }
  }

  return res;
}

//------------------------------------------------------------------------------
/**
  Установить активный контейнер объектов
*/
//---
void SketchDispatcher::SetDocumentDrawingContainer()
{
  if (m_sketch)
  {
    IViewsAndLayersManagerPtr valMng = m_sketch->GetViewsAndLayersManager();
    IViewsPtr views = valMng->GetViews();
    IViewPtr view = views->GetActiveView();
    IDrawingContainerPtr drawCont = view;
    m_containers.push(drawCont);
  }
}


//------------------------------------------------------------------------------
/**
  Получить активный контейнер объектов
*/
//---
ksapi::IDrawingContainerPtr SketchDispatcher::GetContainer()
{
  if (m_containers.empty())
    SetDocumentDrawingContainer();

  return m_containers.top();
}
