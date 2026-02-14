#pragma once
#include "IPlacementProcess.h"
#include "BaseProcess.h"
#include "LeaderBuildProcess.h"

class ParameterObject;

/// Состояние процесса
enum class ProcessState
{
  processPlacement,
  processSelectLeader,
  processCreateLeader,
  processAborted,
};


////////////////////////////////////////////////////////////////////////////////
//
/// Процесс установки элемента
/**
  \ingroup
*/
////////////////////////////////////////////////////////////////////////////////
class ParameterObjectProcess : public BaseProcess,
                               public IPlacementProcess
{
private:
  ksapi::IProcessParamPtr m_procParam;                 ///< Параметры процесса
  ksapi::IProcessPtr m_process;                        ///< Процесс
  ksapi::IProcess2DPtr m_process2D;                    ///< Процесс 2D документа
  ksapi::IPropertyCheckBoxPtr m_specificationCheckBox; ///< Чек-бокс на создание объекта спецификации

  ProcessState m_processState;       ///< Признак запуска процесса Placement
  ParameterObject & m_processObject; ///< Подконтрольный элемент

public:
  /// Конструктор
  ParameterObjectProcess(ParameterObject & parameterObj);
  /// Конструктор
  ParameterObjectProcess(const ParameterObjectProcess & other) = delete;
  /// Деструктор
  ~ParameterObjectProcess();

  /// Получить базовый объект процесса
  virtual ParameterObject * GetProcessObject() override;

  /// Флаг установлен в true
  void SetChanged();
  /// Флаг установлен в false
  void SetUnchanged();
  /// Получить флаг изменения параметров
  bool Changed();

  /// Отрисовка фантома
  virtual void DrawPhantom(double x, double y, double angle, bool dynamic);

#pragma region IPlacementProcess
  /// Инициализация параметров процесса
  virtual bool InitProcessParam(long toolBarID, SpecPropertyToolBarEnum toolBarType, long firstTabID = 0) override;
  /// Запуск процесса установки объекта
  virtual void InitPlacementProcess() override;
  /// Получить фантом
  virtual ksapi::IPhantom2DPtr GetPhantom2D() override;
#pragma endregion

#pragma region HandlerFunctions
  /// Изменено положения объекта
  virtual bool PlacementChanged(double x, double y, double angle, bool dynamic) override;
  /// Завершение процесса
  virtual bool EndProcess() override;
  /// Обработчик закрытия документа
  bool OnCloseDocument();
#pragma endregion

#pragma region BaseProcess
  /// Запустить процесс
  virtual bool Run(bool modal, bool postMessage) override;
  /// Отписаться от событий
  virtual void UnadviseEvents() override;
  /// Вывод контролов
  virtual void ShowControls() override;
  /// Перерисовать фантом
  virtual void RedrawPhantom() override;
  /// Процесс создания элемента
  virtual void Draw1() override;
  /// Изменить значение контрола
  virtual bool ChangeControlValue(ksapi::IPropertyControl & ctrl) override;
  /// Вывод контрола штриховки
  virtual void ShowHatchControl() override;
#pragma endregion
};

/// Создать процесс установки элемента
ParameterObjectProcess * CreateParameterObjectProcess(ParameterObject & parameterObj);
