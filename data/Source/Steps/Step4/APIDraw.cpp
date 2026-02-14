#include "APIDraw.h"

#include "Step4_pch.h"
#include "Events/Process2DEvent.h"
#include "Resource.h"
#include "Step4.h"

#define IDB_RECT     0xE6E9l
#define IDB_CIRCLE   0xE584l
#define IDB_TRIANGLE 0xEA99l

#define ID_MENUBUTTON1 1
#define ID_MENUBUTTON2 2
#define ID_MENUBUTTON3 3

extern ksapi::IApplication * ksAppl;
extern std::wstring handlerOwner;

//------------------------------------------------------------------------------
/**
  Построение геометрии
*/
//---
void APIDraw::GetGroup()
{
  switch (m_phantomType)
  {
    case DrawType::DrawRect:
    {
      dispatcher.CreateLineSegment(-10, 0, 10, 0, 1);
      dispatcher.CreateLineSegment(10, 0, 10, 20, 1);
      dispatcher.CreateLineSegment(10, 20, -10, 20, 1);
      dispatcher.CreateLineSegment(-10, 20, -10, 0, 1);
      break;
    }
    case DrawType::DrawCircle:
    {
      dispatcher.CreateCircle(0, 0, 20, 1);
      break;
    }
    case DrawType::DrawTreangle:
    {
      dispatcher.CreateLineSegment(-10, 0, 10, 0, 1);
      dispatcher.CreateLineSegment(10, 0, 0, 20, 1);
      dispatcher.CreateLineSegment(0, 20, -10, 0, 1);
      break;
    }
  }
}


//------------------------------------------------------------------------------
/**
  Отрисовка фантома
*/
//---
void APIDraw::DrawPhantom(double x, double y, double angle, bool dynamic)
{
  if (!dynamic)
  {
    // Создание временной группы
    ksapi::IDrawingGroupPtr gr1 = dispatcher.CreateDrawingGroup(true);
    dispatcher.CloseGroup();
    // Меняем местами две группы
    gr1->AddObjects({m_macroObj});
    macro::SetMacroPlacement(m_macroObj, x, y, angle, false, false);
    gr1->Store();

    RedrawPhantom();
  }
}


//------------------------------------------------------------------------------
/**
  Изменение фантома
*/
//---
void APIDraw::RedrawPhantom()
{
  if (ksapi::IProcess2DPtr process2D = m_process)
  {
    if (ksapi::IPhantom2DPtr phantom = process2D->GetPhantom2D())
    {
      bool newPh = !m_phantomGroup;

      if (newPh)
      {
        phantom->SetPhantomType(ksMoveGroupPhantom);
        dispatcher.CreateDrawingGroup(true);
        m_phantomGroup = dispatcher.CloseGroup();
        phantom->SetPhantomGroup(m_phantomGroup);
      }

      if (m_phantomGroup)
      {
        m_phantomGroup->Clear(true);
        m_phantomGroup->Open();
        m_macroObj = dispatcher.CreateMacroObject(false);
        GetGroup();
        dispatcher.CloseMacroObject();
        m_phantomGroup->Close();
      }

      if (!newPh)
        phantom->Update();
    }
  }
}


//------------------------------------------------------------------------------
/**
  Отображение кнопок спецпанели
*/
//---
void APIDraw::ShowControls()
{
  m_procParam->AddSpecToolbarButton(ID_BUTTON1, IDB_RECT, LoadStr(IDS_RECT), L"");
  m_procParam->AddSpecToolbarButton(ID_BUTTON2, IDB_CIRCLE, LoadStr(IDS_CIRCLE), L"");
  m_procParam->AddSpecToolbarButton(ID_BUTTON3, IDB_TRIANGLE, LoadStr(IDS_TRIANGLE), L"");
}


//------------------------------------------------------------------------------
/**
  Изменение положения
*/
//---
bool APIDraw::PlacementChanged(double x, double y, double angle, bool dynamic)
{
  DrawPhantom(x, y, angle, dynamic);
  return true;
}

//------------------------------------------------------------------------------
/**
  Завершение процесса
*/
//---
void APIDraw::EndProcess()
{
  // Отписываемся от событий
  if (m_procParam)
    m_procParam->Events()->RemoveAllHandlers(handlerOwner);
}

//------------------------------------------------------------------------------
/**
  Установка состояния кнопки спецпанели
*/
//---
bool APIDraw::ButtonUpdate(std::int32_t buttonID, std::int32_t & check, bool & enable)
{
  switch (buttonID)
  {
    case ID_BUTTON1:
    case ID_BUTTON2:
    case ID_BUTTON3:
    {
      check = std::int32_t(m_phantomType == static_cast<DrawType>(buttonID));
      break;
    }
  }
  return true;
}


//------------------------------------------------------------------------------
/**
  Нажатие кнопки
*/
//---
bool APIDraw::ButtonClick(std::int32_t buttonID)
{
  bool res = false;

  switch (buttonID)
  {
    case ID_BUTTON1:
    case ID_BUTTON2:
    case ID_BUTTON3:
    {
      m_phantomType = static_cast<DrawType>(buttonID);
      UpdateProcessMenu();
      RedrawPhantom();
      res = true;
      break;
    }
    case ID_MENUBUTTON1:
    {
      res = ButtonClick(ID_BUTTON1);
      break;
    }
    case ID_MENUBUTTON2:
    {
      res = ButtonClick(ID_BUTTON2);
      break;
    }
    case ID_MENUBUTTON3:
    {
      res = ButtonClick(ID_BUTTON3);
      break;
    }
  }
  return res;
}


//------------------------------------------------------------------------------
/**
  Запуск процесса
*/
//---
void APIDraw::ChoicePlacement()
{
  if (ksapi::IKompasDocument2DPtr doc2D = ksAppl ? ksAppl->GetActiveDocument() : nullptr)
  {
    ksapi::IProcess2DPtr process2D(doc2D->GetLibProcess(ksProcess2DPlacement));
    ksapi::IProcessPtr process(process2D);

    if (process)
    {
      process->SetProcessParam(m_procParam);
      m_process = process;
      new Process2DEvent(process2D, *this);
      UpdateProcessMenu();
      RedrawPhantom();
      process->Run(true, false);
    }
  }
}


//------------------------------------------------------------------------------
/**
  Обновить состав меню процесса
*/
//---
void APIDraw::UpdateProcessMenu()
{
  if (m_process)
  {
    IMenuPtr menu = ksAppl->CreateMenu();
    if (m_phantomType != static_cast<DrawType>(ID_BUTTON1))
      menu->Append(ID_MENUBUTTON1, LoadStr(IDS_RECT), 0, L"");
    if (m_phantomType != static_cast<DrawType>(ID_BUTTON2))
      menu->Append(ID_MENUBUTTON2, LoadStr(IDS_CIRCLE), 0, L"");
    if (m_phantomType != static_cast<DrawType>(ID_BUTTON3))
      menu->Append(ID_MENUBUTTON3, LoadStr(IDS_TRIANGLE), 0, L"");
    m_process->SetMenu(menu);
    m_process->Update();
  }
}
