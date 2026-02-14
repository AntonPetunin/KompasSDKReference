////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief 
*/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stack>
#include <string>
#include <KsAPI.h>

using namespace ksapi;

struct PropertyParam;
class ParameterObject;


////////////////////////////////////////////////////////////////////////////////
//
/// Диспетчер активного документа
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class Doc2DDispatcher
{
private:
  IKompasDocument2DPtr m_document;               ///< Активный документ
  std::stack<IDrawingContainerPtr> m_containers; ///< Контейнеры объектов
  std::stack<IDrawingGroupPtr> m_groups;         ///< Группы
  std::stack<IMacroObjectPtr> m_macroObjs;       ///< Макро-объекты

public:
  /// Конструктор
  Doc2DDispatcher();

  /// Установить активный документ
  void SetActiveDocument();
  /// Получить активный документ
  IKompasDocument2DPtr GetActiveDocument() const { return m_document; };
  /// Создать отрезок по координатам
  void CreateLineSegment(double x1, double y1, double x2, double y2, unsigned short style);
  /// Создать окружность по центру и радиусу
  void CreateCircle(double xc, double yc, double rad, unsigned short style);
  /// Создать группу
  IDrawingGroupPtr CreateDrawingGroup(bool tmpGroup);
  /// Добавить объект в группу
  void AddObjGroup(ksapi::IDrawingGroupPtr & group, ksapi::IMacroObjectPtr & macroObject);
  /// Открыть группу
  IDrawingGroupPtr OpenDrawingGroup(IDrawingGroupPtr & createdGroup);
  /// Очистить группу
  void ClearDrawingGroup(ksapi::IDrawingGroupPtr & group, bool deleteTmp = true);
  /// Закрыть редактирование группы
  IDrawingGroupPtr CloseGroup();
  /// Создать макро
  IMacroObjectPtr CreateMacroObject(bool multyLayer);
  /// Закрыть работу с макро
  IMacroObjectPtr CloseMacroObject();

private:
  /// Установить активный контейнер объектов
  void SetDocumentDrawingContainer();
  /// Получить активный контейнер объектов
  IDrawingContainerPtr GetContainer();
};


namespace macro
{
/// Установить положение и угол макро
bool SetMacroPlacement(ksapi::IMacroObjectPtr & macroObj, double x, double y, double angle, bool mirrorSymetry, bool relative);
} // namespace macro


/// Конвертировать строку из std::string в std::wstring
std::wstring GetWStringFromString(const std::string & str);
/// Конвертировать строку из std::wstring в std::string
std::string GetStringFromWString(const std::wstring & wstr);
