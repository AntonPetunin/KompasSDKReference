////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Класс для работы с хот точками
*/
//
////////////////////////////////////////////////////////////////////////////////

#include "HotPoint.h"
#include "Gayka1_pch.h"
#include "ParameterObject.h"
#include <KompasLibraryActions.h>

//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
LibPropObj::LibPropObj(ParameterObject * _obj)
  : count(0)
  , m_obj(_obj)
{
}

//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
LibPropObj::~LibPropObj()
{
  if (m_obj)
    delete m_obj;
}

void LibPropObj::AddRef()
{
  count++;
}

void LibPropObj::Release()
{
  if (!--count)
    delete this;
}

IAPIObject * LibPropObj::QueryInterface(IAPIObjectIID id)
{
  if (id == IID_ILibraryObject)
    return static_cast<ILibraryObject *>(this);

  if (id == IID_ILibraryPropertiesObject)
    return static_cast<ILibraryPropertiesObject *>(this);

  return nullptr;
}


//------------------------------------------------------------------------------
/**
  Имя группы объектов по умолчанию макроэлементы
*/
//---
std::wstring LibPropObj::GetGroupName()
{
  return m_obj ? m_obj->GetGroupName() : L"";
}


//------------------------------------------------------------------------------
/**
  Добавить свойство в список свойств
*/
//---
bool LibPropObj::GetProperty(int32_t index, const IPropertyParamPtr & propertyParam)
{
  return m_obj && propertyParam && m_obj->GetProperty(index, *propertyParam);
}


//------------------------------------------------------------------------------
/**
  Обновить параметры свойства
*/
//---
bool LibPropObj::UpdateProperty(const IPropertyParamPtr & propertyParam)
{
  return m_obj && propertyParam && m_obj->UpdateProperty(*propertyParam);
}


//------------------------------------------------------------------------------
/**
  Изменить свойство
*/
//---
int LibPropObj::ApplyProperty(const IPropertyParamPtr & propertyParam)
{
  int res = 0;

  if (m_obj && propertyParam)
  {
    m_obj->ApplyProperty(*propertyParam);

    if (m_obj->Changed()) // Завершение изменения параметров
      m_obj->RedrawMacro();
  }

  return res;
}

//------------------------------------------------------------------------------
/**
  Конструктор
*/
//---
HPObj::HPObj(ParameterObject * _obj)
  : LibPropObj(_obj)
{
}


//------------------------------------------------------------------------------
/**
  Деструктор
*/
//---
HPObj::~HPObj()
{
}


void HPObj::AddRef()
{
  ++count;
}

void HPObj::Release()
{
  if (!--count)
    delete this;
}

IAPIObject * HPObj::QueryInterface(IAPIObjectIID id)
{
  if (id == IID_ILibraryHotPointsObject)
    return static_cast<ILibraryHotPointsObject *>(this);

  return LibPropObj::QueryInterface(id);
}


bool HPObj::BeginDrag(int32_t index)
{
  return !m_obj || m_obj->Prepare();
}

bool HPObj::EndDrag(int32_t index, bool success)
{
  return !m_obj || m_obj->EditComplete(index, success);
}

bool HPObj::Get(int32_t index, const IObject2DHotPointPtr & hotPoint)
{
  return m_obj && hotPoint && m_obj->GetHotPoint(*hotPoint, index);
}

bool HPObj::Set(int32_t index, double x, double y)
{
  return m_obj && m_obj->SetHotPoint(x, y, index);
}

std::wstring HPObj::GetCursorText(int32_t index)
{
  return m_obj ? m_obj->GetCursorTextEx(index) : L"";
}

bool HPObj::ExecuteMenuCommand(int32_t id)
{
  return m_obj && m_obj->ExecuteCommand(id);
}

//------------------------------------------------------------------------------
/**
  Получить popup-меню ассоциированое с данным библиотечным элементом, если index = -1
  и ассоциированое с данной hot-точкой, если index > -1
*/
//---
IMenuPtr HPObj::GetMenu(int32_t index)
{
  return m_obj ? m_obj->GetMenu(index) : nullptr;
}

void HPObj::GetMenuCommandState(int32_t commanId, bool & enable, bool & checked)
{
  if (m_obj)
    m_obj->GetCommandState(commanId, enable, checked);
}

bool HPObj::Select(int32_t index)
{
  return !m_obj || m_obj->SelectHotPoint(index);
}

bool HPObj::Unselect(int32_t index)
{
  return !m_obj || m_obj->UnselectHotPoint(index);
}

ParameterObject * NewCParObject(int command, IMacroObjectPtr & macroObj);

//------------------------------------------------------------------------------
/**
  Создает соответствующий объект для работы с Hot точками
*/
//---
HPObj * LibObjInterfaceEntry(IKompasAPIObjectPtr currentObject)
{
  // Если не Hot точки обнулить
  IMacroObjectPtr macroObject = currentObject;

  if (IUserParametersPtr userParams = macroObject)
  {
    if (ParameterObject * obj = NewCParObject(userParams->GetCommand(), macroObject))
      return new HPObj(obj);
  }

  return nullptr;
}
