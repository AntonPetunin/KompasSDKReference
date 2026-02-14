#pragma once

#include <PropManager/CPropMen.h>
#include <SketchDispatcher.h>


//-------------------------------------------------------------------------------
// Класс Отверстие
// ---
class Orifice : public PropertyManagerObject,
                public ksapi::ILibraryPropertiesObject
{
protected:
  double m_diameter;                      ///< Диаметр отверстия
  ksapi::IModelObjectPtr m_face1;         ///< Первая грань
  ksapi::IModelObjectPtr m_face2;         ///< Вторая грань
  ksapi::IMacroObject3DPtr m_macroObject; ///< Макрообъект
  ksapi::IUserDataStoragePtr m_params;    ///< Параметры макрообъекта
  std::wstring m_varName;                 ///<

private:
  int m_refCount;
  ksapi::IProcess3DPtr m_process3D;
  SketchDispatcher m_sketchDisp;

public:
  Orifice(bool forProp = false); // Конструктор
                                 // Записать параметры в хранилище
  virtual ~Orifice() = default;

  virtual void AddRef() override;
  virtual void Release() override;
  ksapi::IAPIObject * QueryInterface(ksapi::IAPIObjectIID id) override;

  void SetUserData();
  // Получить параметры макрообъекта из хранилища
  void GetUserData();

  virtual void Create(bool forProp);  // Создание отверстия
  virtual void EndProcess() override; // Завершить процесс редактирования
  virtual bool ProcessRun() override;

  std::wstring CreateVariable(const ksapi::ICirclePtr & circle);
  bool UpdateVariable(const ksapi::IModelObjectPtr & sketch);

#pragma region ILibraryPropertiesObject
  virtual std::wstring GetGroupName() override;
  virtual bool GetProperty(int32_t index, const ksapi::IPropertyParamPtr & propertyParam) override;
  virtual bool UpdateProperty(const ksapi::IPropertyParamPtr & propertyParam) override;
  virtual int32_t ApplyProperty(const ksapi::IPropertyParamPtr & propertyParam) override;
  virtual bool OnChoiceProperty(const ksapi::IPropertyParamPtr & propertyParam) override;
  virtual bool GetMouseEnterLeavePoint(int32_t propertyId, int32_t pointIndex,
                                       const IMouseEnterLeaveParametersPtr & parameters) override;
#pragma endregion

  // Выдать идентификатор свойства по индексу
  virtual int GetPropertyID(int index);

#pragma region PropertyManagerObject
  // prChangeControlValue - Изменение значения контрола
  virtual bool OnChangeControlValue(std::int32_t ctrlID, double newVal) override;

  // prButtonUpdate        - Установка состояния кнопки спецпанели.
  virtual bool ButtonUpdate(std::int32_t buttonID, std::int32_t & check, bool & enable) override;
  // prButtonClick - Нажатие кнопки.
  virtual bool ButtonClick(std::int32_t buttonID) override;
  // prControlCommand Нажатие кнопки контрола
  virtual bool ControlCommand(const ksapi::IPropertyControlPtr & ctrl, std::int32_t buttonID) override;

  //ksPlacementChanged     Изменение положения.
  virtual bool PlacementChanged(const ksapi::IKompasAPIObjectPtr & object) override;
  // ksFilterProc         - Фильтрация объектов
  virtual bool FilterProc(const ksapi::IKompasAPIObjectPtr & object) override;
  // prFillContextPanel  -  Накачка контекстной панели
  virtual bool FillContextPanel(const ksapi::IProcessContextPanelPtr & contextPanel) override;
  // Получение типа контекстного меню
  virtual ksProcessContextMenuType GetContextMenuType(std::int32_t screenX, std::int32_t screenY) override;
#pragma endregion

  // Выбор грани
  void ChoiceFaces();
  //Установка первой грани
  void SetFace1(ksapi::IModelObjectPtr face);
  //Установка второй грани
  void SetFace2(ksapi::IModelObjectPtr face);
  // Получение первой грани
  ksapi::IModelObjectPtr GetFace1();
  // Получение второй грани
  ksapi::IModelObjectPtr GetFace2();
  std::wstring GetVariableExpression();

  // Наполнение контролов элемента
  virtual void ShowControls() override;
};
