////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////


#include "Step4_pch.h"
#include <locale>
#include <codecvt>

#include "Doc2DDispatcher.h"

extern ksapi::IApplication * ksAppl;

//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
Doc2DDispatcher::Doc2DDispatcher()
{
  SetActiveDocument();
}


//------------------------------------------------------------------------------
/**
  Установить активный документ
*/
//---
void Doc2DDispatcher::SetActiveDocument()
{
  IKompasDocument2DPtr doc = ksAppl ? ksAppl->GetActiveDocument() : nullptr;

  if (doc && doc != m_document)
    m_document = doc;
}


//------------------------------------------------------------------------------
/**
  Создать отрезок по координатам
*/
//---
void Doc2DDispatcher::CreateLineSegment(double x1, double y1, double x2, double y2, unsigned short style)
{
  IDrawingContainerPtr drawCont = GetContainer();
  ksapi::ILineSegmentsPtr lines = drawCont->GetLineSegments();
  ksapi::ILineSegmentPtr line = lines->Add();

  line->SetX1(x1);
  line->SetY1(y1);
  line->SetX2(x2);
  line->SetY2(y2);
  line->SetStyle(style);
  line->Update();
}


//------------------------------------------------------------------------------
/**
  Создать окружность по центру и радиусу
*/
//---
void Doc2DDispatcher::CreateCircle(double xc, double yc, double rad, unsigned short style)
{
  IDrawingContainerPtr drawCont = GetContainer();
  ICirclePtr circle = drawCont->GetCircles()->Add();

  circle->SetStyle(ksCSNormal);
  circle->SetXc(xc);
  circle->SetYc(yc);
  circle->SetRadius(rad);
  circle->SetStyle(style);
  circle->Update();
}


//------------------------------------------------------------------------------
/**
  Создать группу
*/
//---
IDrawingGroupPtr Doc2DDispatcher::CreateDrawingGroup(bool tmpGroup)
{
  if (IKompasDocument2DPtr kDoc2D = m_document)
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
void Doc2DDispatcher::AddObjGroup(ksapi::IDrawingGroupPtr & group, ksapi::IMacroObjectPtr & macroObject)
{
  if (macroObject && group)
    group->AddObjects({macroObject});
}


//------------------------------------------------------------------------------
/**
  Открыть группу
*/
//---
ksapi::IDrawingGroupPtr Doc2DDispatcher::OpenDrawingGroup(IDrawingGroupPtr & createdGroup)
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
void Doc2DDispatcher::ClearDrawingGroup(ksapi::IDrawingGroupPtr & group, bool deleteTmp)
{
  if (group)
    group->Clear(deleteTmp);
}


//------------------------------------------------------------------------------
/**
  Закрыть редактирование группы
*/
//---
IDrawingGroupPtr Doc2DDispatcher::CloseGroup()
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


//------------------------------------------------------------------------------
/**
  Создать макро
*/
//---
IMacroObjectPtr Doc2DDispatcher::CreateMacroObject(bool multyLayer)
{
  if (IDrawingContainerPtr drawCont = GetContainer())
  {
    IMacroObjectPtr macroObj = drawCont->GetMacroObjects()->Add(multyLayer);

    if (IDrawingContainerPtr newDrawCont = macroObj)
    {
      m_containers.push(newDrawCont);
      m_macroObjs.push(macroObj);
      return macroObj;
    }
  }

  return nullptr;
}


//------------------------------------------------------------------------------
/**
  Закрыть работу с макро
*/
//---
IMacroObjectPtr Doc2DDispatcher::CloseMacroObject()
{
  if (!m_containers.empty())
  {
    IMacroObjectPtr macroObj = m_macroObjs.top();
    macroObj->Update();

    m_containers.pop();
    m_macroObjs.pop();

    return macroObj;
  }

  return nullptr;
}


//------------------------------------------------------------------------------
/**
  Установить активный контейнер объектов
*/
//---
void Doc2DDispatcher::SetDocumentDrawingContainer()
{
  if (m_document)
  {
    IViewsAndLayersManagerPtr valMng = m_document->GetViewsAndLayersManager();
    IViewsPtr views = valMng->GetViews();

    if (IDrawingContainerPtr drawCont = views ? views->GetActiveView() : nullptr)
      m_containers.push(drawCont);
  }
}


//------------------------------------------------------------------------------
/**
  Получить активный контейнер объектов
*/
//---
ksapi::IDrawingContainerPtr Doc2DDispatcher::GetContainer()
{
  if (m_containers.empty())
    SetDocumentDrawingContainer();

  return m_containers.top();
}


namespace macro
{
//------------------------------------------------------------------------------
/**
  Установить положение и угол макро
*/
//---
bool SetMacroPlacement(ksapi::IMacroObjectPtr & macroObj, double x, double y, double angle, bool mirrorSymetry, bool relative)
{
  return macroObj && macroObj->SetPlacement(x, y, angle, mirrorSymetry, relative);
}
} // namespace macro


//------------------------------------------------------------------------------
/**
  Конвертировать строку из std::string в std::wstring
*/
//---
std::wstring GetWStringFromString(const std::string & str)
{
#pragma warning(disable : 4996)
  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.from_bytes(str);
}


//------------------------------------------------------------------------------
/**
  Конвертировать строку из std::wstring в std::string
*/
//---
std::string GetStringFromWString(const std::wstring & wstr)
{
#pragma warning(disable : 4996)
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return converter.to_bytes(wstr.c_str());
}
