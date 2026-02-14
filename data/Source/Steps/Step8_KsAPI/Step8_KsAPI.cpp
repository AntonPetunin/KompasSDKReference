////////////////////////////////////////////////////////////////////////////////
//
// Step8_KsAPI.cpp - Работа с атрибутами (KsAPI)
//
// 1.  Создать тип атрибута                      - FuncTypeAttr
// 2.  Удалить тип атрибута                      - DelTypeAttr
// 3.  Получить тип атрибута                     - ShowTypeAttr
// 4.  Заменить тип атрибута                     - ChangeType
// 5.  Создать атрибут определенного типа        - NewAttr
// 6.  Удалить атрибут                           - DelObjAttr
// 7.  Считать атрибут                           - ReadObjAttr
// 8.  Просмотреть атрибут                       - ShowObjAttr
// 9.  Просмотреть библиотеку                    - ShowLib
// 10. Просмотреть тип                           - ShowType
// 11. Просмотреть атрибут                       - WalkFromObjWithAttr
// 12. Создать атрибут документа                 - CreateDocumentAttr
// 13. Удалить атрибут документа                 - DeleteDocumentAttr
// 14. Просмотреть атрибут документа             - ShowDocumentAttr
// 15. Создать атрибут 3D документа              - CreateDocument3DAttr
// 16. Удалить атрибут 3D документа              - DeleteDocument3DAttr
// 17. Просмотреть атрибут 3D документа          - ShowDocument3DAttr
// 18. Просмотреть атрибут объектов 3D документа - ShowObjects3DAttr
//
////////////////////////////////////////////////////////////////////////////////
#include <format>

#include "Resource.h"

#include <KsAPI.h>
#include <KompasLibraryActions.h>

using namespace ksapi;

extern void * GetModule();
extern void LoadResources(IApplication & app);

IApplication * kompasApp = nullptr;


//-------------------------------------------------------------------------------
// Описания используемых функций
// ---
void FuncTypeAttr();
void DelTypeAttr();
void NewAttr();
void ChangeType();
void ShowTypeAttr();
void DelObjAttr();
void ReadObjAttr();
void ShowObjAttr();
void ShowLib();
void ShowType();
void WalkFromObjWithAttr();
void CreateDocumentAttr();
void DeleteDocumentAttr();
void ShowDocumentAttr();
void CreateDocument3DAttr();
void DeleteDocument3DAttr();
void ShowDocument3DAttr();
void ShowObjects3DAttr();


//------------------------------------------------------------------------------
// Вспомогательная функция, вывод сообщения
// ---
int32_t LibMessage(const std::wstring & str, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum dlgButtons, bool showModal)
{
  if (!str.empty())
    return kompasApp->ShowMessageBox(str, L"", dlgType, dlgButtons, showModal);

  return 0;
}


//------------------------------------------------------------------------------
// Загрузить строку из ресурса.
//---
std::wstring LoadStr(int32_t strId)
{
  if (ILibraryManagerPtr libraryManager = kompasApp->GetLibraryManager())
  {
    if (IProceduresLibrariesPtr proceduresLibraries = libraryManager->GetProceduresLibraries())
    {
      if (IProceduresLibraryPtr lib = proceduresLibraries->GetItemByName(libName))
      {
        if (ksapi::IKompasDictionaryPtr dictionary = lib->GetDictionary())
          return dictionary->LoadAndConvertLangString(GetModule(), strId);
      }
    }
  }

  return std::wstring();
}


//-------------------------------------------------------------------------------
// Функция для отображения колонки
// ---
void ShowColumn(IColumnInfoPtr column, int32_t i)
{
  if (column)
  {
    std::wstring caption = column->GetCaption();
    int32_t colType = column->GetColumnType();
    std::wstring defValue = column->GetDefaultValue();
    bool listValue = column->IsListValue();

    // Выдадим информацию о колонке
    std::wstring colInfo = std::vformat(LoadStr(colType == ksValueTypeRecord ? FORMAT_COLUMNINFORECORD : FORMAT_COLUMNINFO),
                                        std::make_wformat_args(i, caption, colType, defValue, listValue));
    LibMessage(colInfo, ksMessageWarning, ksButtonSetOk, true);

    for (int32_t idx = 0, recCount = column->GetRecordColumnsCount(); idx < recCount; idx++)
    {
      if (IColumnInfoPtr recCol = column->GetRecordColumnInfo(idx))
        ShowColumn(recCol, idx);
    }
  }
}


enum documentType
{
  document3D,
  document2D,
  allDocuments
};


ISelectionManagerPtr GetSelectionManager(documentType type)
{
  ISelectionManagerPtr res;

  if (IKompasDocumentPtr doc = kompasApp->GetActiveDocument())
  {
    IKompasDocument2DPtr doc2D = doc;
    if ((type == document2D || type == allDocuments) && doc2D)
      res = doc2D->GetSelectionManager();
    else if (type == document3D || type == allDocuments)
    {
      if (IKompasDocument3DPtr doc3D = doc)
        res = doc3D->GetSelectionManager();
    }
  }

  return res;
}


IChooseManagerPtr GetChooseManager(documentType type)
{
  IChooseManagerPtr res;

  if (IKompasDocumentPtr doc = kompasApp->GetActiveDocument())
  {
    IKompasDocument2DPtr doc2D = doc;
    if ((type == document2D || type == allDocuments) && doc2D)
      res = doc2D->GetChooseManager();
    else if ((type == document3D || type == allDocuments))
    {
      if (IKompasDocument3DPtr doc3D = doc)
        res = doc3D->GetChooseManager();
    }
  }

  return res;
}


IAttrTypeMngPtr GetAttrTypeMng(const std::wstring & fileName)
{
  IAttrTypeMngPtr attrMng;

  if (!fileName.empty())
  {
    if (IKompasDocumentPtr activeDocument = kompasApp->GetActiveDocument())
      attrMng = activeDocument->GetAttrTypeMng();
  }
  else
  {
    attrMng = kompasApp->GetAttrTypeMng(fileName);
  }

  return attrMng;
}


//-------------------------------------------------------------------------------
// Создать тип атрибута
// ---
void FuncTypeAttr()
{
  // Запросить имя библиотеки
  std::wstring fileName =
    kompasApp->GetApplicationDialogs()->ChoiceFile(nullptr, L"*.lat", L"", L"", ksCurrentDocumentLibraries, false);

  IAttrTypeMngPtr attrMng = GetAttrTypeMng(fileName);
  if (!attrMng)
    return;

  if (IAttributeTypePtr attrType = attrMng->CreateAttrType())
  {
    attrType->SetTypeName(L"int_struct_string");
    if (IColumnInfoPtr columnInfo1 = attrType->AddColumn(0, ksValueTypeInteger, L"int"))
    {
      columnInfo1->SetKey(0);
      columnInfo1->SetCaption(L"int");
      columnInfo1->SetColumnType(ksValueTypeInteger);
      columnInfo1->SetDefaultValue(L"100");
      columnInfo1->SetListValue(true);

      std::vector<PropertyValue> values;
      values.push_back(std::wstring(L"100"));
      values.push_back(std::wstring(L"200"));
      values.push_back(std::wstring(L"300"));
      columnInfo1->SetRange(values);
    }

    if (IColumnInfoPtr columnInfo2 = attrType->AddColumn(0, ksValueTypeRecord, L"struct"))
    {
      if (IColumnInfoPtr structCol1 = columnInfo2->AddRecordColumn(0, ksValueTypeFloat, L"double"))
      {
        structCol1->SetKey(0);
        structCol1->SetCaption(L"double");
        structCol1->SetColumnType(ksValueTypeFloat);
        structCol1->SetDefaultValue(L"123456789");
        structCol1->SetListValue(false);
      }

      if (IColumnInfoPtr structCol2 = columnInfo2->AddRecordColumn(0, ksValueTypeInteger, L"long"))
      {
        structCol2->SetKey(0);
        structCol2->SetCaption(L"long");
        structCol2->SetColumnType(ksValueTypeInteger);
        structCol2->SetDefaultValue(L"1000000");
        structCol2->SetListValue(false);
      }

      if (IColumnInfoPtr structCol3 = columnInfo2->AddRecordColumn(0, ksValueTypeString, L"string"))
      {
        structCol3->SetKey(0);
        structCol3->SetCaption(L"string");
        structCol3->SetColumnType(ksValueTypeString);
        structCol3->SetDefaultValue(L"10");
        structCol3->SetListValue(false);
      }
    }

    if (IColumnInfoPtr columnInfo3 = attrType->AddColumn(0, ksValueTypeRecord, L"string"))
    {
      columnInfo3->SetKey(0);
      columnInfo3->SetCaption(L"string");
      columnInfo3->SetColumnType(ksValueTypeString);
      columnInfo3->SetDefaultValue(L"text");
      columnInfo3->SetListValue(false);
    }

    attrType->SetRowsCount(0);
    attrType->SetPassword(L"", L"");
    // key1, key2, key3 - Рекомендуется как код разработчика
    // key4             - Системный код атрибута
    attrType->SetKeysInfo(10, 20, 30, 0);

    attrType->Update(L"");
  }
}


//-------------------------------------------------------------------------------
// Удалить тип атрибута
// ---
void DelTypeAttr()
{
  // Запросить имя библиотеки
  std::wstring nameFile =
    kompasApp->GetApplicationDialogs()->ChoiceFile(nullptr, L"*.lat", L"", L"", ksCurrentDocumentLibraries, false);

  if (nameFile.empty() && kompasApp->GetActiveDocument())
    nameFile = kompasApp->GetActiveDocument()->GetFullPath();

  IAttrTypeMngPtr attrMng = kompasApp->GetAttrTypeMng(nameFile);
  if (attrMng) // Если выбрана библиотека или есть документ
  {
    IAttributeTypePtr attrType = attrMng->ChoiceAttrTypes(kompasApp->GetMainWindowHandle());
    if (attrType && attrType->GetUniqueNumb() > 1)
    {
      std::wstring password;
      if (kompasApp->GetApplicationDialogs()->ReadString(kompasApp->GetMainWindowHandle(), LoadStr(ENTERPASSWORD), 255, password))
      {
        if (!attrType->Delete(password))
          kompasApp->GetKompasError()->Report(); // Выдать сообщение об ошибке на экран
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Получить тип атрибута
// ---
void ShowTypeAttr()
{
  // Запросить имя библиотеки
  std::wstring nameFile =
    kompasApp->GetApplicationDialogs()->ChoiceFile(nullptr, L"*.lat", L"", L"", ksCurrentDocumentLibraries, false);

  if (nameFile.empty() && kompasApp->GetActiveDocument())
    nameFile = kompasApp->GetActiveDocument()->GetFullPath();

  IAttrTypeMngPtr attrMng = kompasApp->GetAttrTypeMng(nameFile);
  if (attrMng) // Если выбрана библиотека или есть документ
  {
    IAttributeTypePtr attrType = attrMng->ChoiceAttrTypes(kompasApp->GetMainWindowHandle());
    if (attrType && attrType->GetUniqueNumb() > 1)
    {
      int32_t key1, key2, key3, key4 = 0;
      attrType->GetKeysInfo(key1, key2, key3, key4);

      std::wstring typeName = attrType->GetTypeName();
      int32_t rowsCount = attrType->GetRowsCount();
      std::wstring attrTypeInfo =
        std::vformat(LoadStr(FORMAT_ATTRTYPEINFO), std::make_wformat_args(key1, key2, key3, key4, typeName, rowsCount));
      LibMessage(attrTypeInfo, ksMessageWarning, ksButtonSetOk, true);

      for (int32_t i = 0, colCount = attrType->GetColumnsCount(); i < colCount; i++)
      {
        if (IColumnInfoPtr colInfo = attrType->GetColumnInfo(i))
          ShowColumn(colInfo, i); // Функция для отображения колонки
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Создание атрибута
// ---
void LibCreateAttr(const std::vector<IKompasAPIObjectPtr> & objects)
{
  // Запросить имя библиотеки
  std::wstring nameFile =
    kompasApp->GetApplicationDialogs()->ChoiceFile(nullptr, L"*.lat", L"", L"", ksCurrentDocumentLibraries, false);

  if (nameFile.empty() && kompasApp->GetActiveDocument())
    nameFile = kompasApp->GetActiveDocument()->GetFullPath();

  IKompasDocumentPtr doc = kompasApp->GetActiveDocument();
  IAttrTypeMngPtr attrMng = kompasApp->GetAttrTypeMng(nameFile);
  if (doc && attrMng)
  {
    WindowHandle hwnd = kompasApp->GetMainWindowHandle();
    if (IAttributeTypePtr attrType = attrMng->ChoiceAttrTypes(hwnd))
    {
      if (IAttributePtr attribute = doc->CreateAttr(attrType->GetUniqueNumb(), nameFile, objects))
      {
        attribute->SetPassword(L"", L"111");
        attribute->SetKeysInfo(L"111", 1, 10, 100, 0);
        attribute->ViewEdit(hwnd, L"111", false);

        attrType->Update(L"111");
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Создать атрибут определенного типа
// ---
void NewAttr()
{
  IKompasDocumentPtr doc = kompasApp->GetActiveDocument();
  if (!doc)
  {
    LibMessage(LoadStr(CANTCREATEATTRIBUTE), ksMessageWarning, ksButtonSetOk, true);
    return;
  }

  if (ISelectionManagerPtr selectionMng = GetSelectionManager(allDocuments))
    LibCreateAttr(selectionMng->GetSelectedObjects()); // Создание атрибута
}


//-------------------------------------------------------------------------------
//
// ---
void SetColumnInfo(IColumnInfo & colInfo, const std::wstring & caption, ksValueTypeEnum colType,
                   const std::wstring & defaultValue, int32_t key, const std::vector<PropertyValue> & range)
{
  colInfo.SetCaption(caption);
  colInfo.SetColumnType(colType);
  colInfo.SetDefaultValue(defaultValue);
  colInfo.SetKey(key);
  colInfo.SetRange(range);
}


//-------------------------------------------------------------------------------
// Заменить тип атрибута
// ---
void ChangeType()
{
  std::wstring fileName =
    kompasApp->GetApplicationDialogs()->ChoiceFile(nullptr, L"*.lat", L"", L"", ksCurrentDocumentLibraries, false);

  if (IAttrTypeMngPtr attrMng = GetAttrTypeMng(fileName))
  {
    if (IAttributeTypePtr attrType = attrMng->ChoiceAttrTypes(kompasApp->GetMainWindowHandle()))
    {
      if (NumericId numbType = attrType->GetUniqueNumb(); numbType > 1000)
      {
        std::wstring password;

        if (kompasApp->GetApplicationDialogs()->ReadString(kompasApp->GetMainWindowHandle(), LoadStr(ENTERPASSWORD), 255,
                                                           password))
        {
          IColumnInfoPtr firstColumn = attrType->GetColumnInfo(0);
          IColumnInfoPtr lastColumn = attrType->GetColumnInfo(attrType->GetColumnsCount() - 1);

          const auto & caption = firstColumn->GetCaption();
          auto colType = firstColumn->GetColumnType();
          const auto & defaultValue = firstColumn->GetDefaultValue();
          auto key = firstColumn->GetKey();
          const auto & range = firstColumn->GetRange();

          SetColumnInfo(*firstColumn, lastColumn->GetCaption(), lastColumn->GetColumnType(), lastColumn->GetDefaultValue(),
                        lastColumn->GetKey(), lastColumn->GetRange());

          SetColumnInfo(*lastColumn, caption, colType, defaultValue, key, range);

          attrType->Update(password);

          if (NumericId uniqueNumb = attrType->GetUniqueNumb(); uniqueNumb > 1)
          {
            std::wstring attrTypeInfo = std::vformat(LoadStr(FORMAT_ATTRTYPEINFO3), std::make_wformat_args(uniqueNumb));
            LibMessage(attrTypeInfo, ksMessageWarning, ksButtonSetOk, true);
          }
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Удалить атрибут
// ---
void DeleteAttributes(const std::vector<IKompasAPIObjectPtr> & objects)
{
  IKompasDocumentPtr doc = kompasApp->GetActiveDocument();
  if (doc)
  {
    std::vector<IAttributePtr> attributes = doc->GetAttributes(0, 0, 0, 0, 0, objects);
    for (IAttributePtr & attr : attributes)
    {
      if (attr)
      {
        std::wstring password;
        if (kompasApp->GetApplicationDialogs()->ReadString(kompasApp->GetMainWindowHandle(), LoadStr(ENTERPASSWORD), 10,
                                                           password))
        {
          // Удалить атрибут
          if (!attr->Delete(password, objects))
            kompasApp->GetKompasError()->Report(); // Выдать сообщение об ошибке на экран
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Удалить атрибут
// ---
void DelObj2DAttr()
{
  if (ISelectionManagerPtr selectionMng = GetSelectionManager(document2D))
    DeleteAttributes(selectionMng->GetSelectedObjects()); // Удалить атрибут
}


//-------------------------------------------------------------------------------
// Удалить атрибут объекта 3D
// ---
void DelObj3DAttr()
{
  if (ISelectionManagerPtr selectionMng = GetSelectionManager(document3D))
    DeleteAttributes(selectionMng->GetSelectedObjects()); // Удалить атрибут
}


//-------------------------------------------------------------------------------
// Удалить атрибут
// ---
void DelObjAttr()
{
  if (IKompasDocumentPtr doc = kompasApp->GetActiveDocument())
  {
    DocumentTypeEnum docType = doc->GetDocumentType(); // тип документа
    switch (docType)
    {
      case ksDocumentDrawing:  // 1- Чертеж
      case ksDocumentFragment: // 2- Фрагмент
        DelObj2DAttr();
        break;
      case ksDocumentPart:     // 4- Деталь
      case ksDocumentAssembly: // 5- Сборка
        DelObj3DAttr();
        break;
    }
  }
}


//-------------------------------------------------------------------------------
// Считать атрибут
// ---
void ReadObjAttr()
{
  if (IKompasDocumentPtr doc = kompasApp->GetActiveDocument())
  {
    if (ISelectionManagerPtr selectionMng = GetSelectionManager(document2D))
    {
      std::vector<IAttributePtr> attributes = doc->GetAttributes(0, 0, 0, 0, 0, selectionMng->GetSelectedObjects());
      for (size_t i = 0, count = attributes.size(); i < count; i++)
      {
        IAttributePtr & attr = attributes[i];
        IAttributeTypePtr attrType = attr ? attr->GetAttributeType() : nullptr;
        if (attrType)
        {
          int32_t key1, key2, key3, key4;
          if (attrType->GetKeysInfo(key1, key2, key3, key4))
          {
            NumericId uniqueNumb = attrType->GetUniqueNumb();
            std::wstring attrTypeInfo =
              std::vformat(LoadStr(FORMAT_ATTRTYPEINFO2), std::make_wformat_args(key1, key2, key3, key4, uniqueNumb));
            LibMessage(attrTypeInfo, ksMessageWarning, ksButtonSetOk, true);
          }

          AttributeValue attrFlagVisible = attr->GetFlagVisible();
          std::vector<ksapi::PropertyValue> colsVisible = attrFlagVisible.GetPropertyList();
          size_t colsVisibleSize = 0;

          colsVisibleSize = colsVisible.size();

          for (int32_t i = 0, colCount = attrType->GetColumnsCount(); i < colCount; i++)
          {
            if (IColumnInfoPtr colInfo = attrType->GetColumnInfo(i))
            {
              std::wstring caption = colInfo->GetCaption();
              int32_t colType = colInfo->GetColumnType();
              std::wstring defValue = colInfo->GetDefaultValue();
              bool listValue = colInfo->IsListValue();

              bool flagVisible = false;
              if (i < colsVisibleSize)
              {
                flagVisible = colsVisible.at(i).GetBool();
              }

              // Выдадим информацию о колонке
              std::wstring columnInfo =
                std::vformat(LoadStr(FORMAT_COLUMNINFO2),
                             std::make_wformat_args(i, caption, colType, defValue, listValue, flagVisible));
              LibMessage(columnInfo, ksMessageWarning, ksButtonSetOk, true);
            }
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Выбор объекта пользователем
*/
//---
ksapi::IDrawingObjectPtr PickObject(ksapi::IKompasDocument2D & doc2D)
{
  IProcess2DPtr process2d = doc2D.GetLibProcess(ksProcess2DTypeEnum::ksProcess2DCursor);
  IProcessPtr process = process2d;
  if (!process)
    return nullptr;
  IViewPtr currentView = doc2D.GetViewsAndLayersManager()->GetViews()->GetActiveView();
  if (!currentView)
    return nullptr;

  process->SetPrompt(LoadStr(PICKOBJECT));

  // функция, которая ищет объект по координатам
  IDrawingObjectPtr result;
  auto clickCallback = [currentView, &result](double x, double y, double, bool)
  {
    IFindObjectParametersPtr params = nullptr;
    result = currentView->FindObject(x, y, 1e-6, params);

    // если объект найден, вернём false
    return !result;
  };

  // добавление обработчика
  process2d->Events()->AddPlacementChangedHandler(libName, clickCallback);
  process->Run(true, false);

  return result;
}


//-------------------------------------------------------------------------------
// Просмотреть атрибут
// ---
void ShowObj2DAttr()
{
  IKompasDocument2DPtr doc2D = kompasApp->GetActiveDocument();
  if (IDrawingObjectPtr pickedObj = PickObject(*doc2D))
  {
    IChooseManagerPtr chooseMan = GetChooseManager(document2D);
    chooseMan->Choose({pickedObj});
    doc2D->ViewEditAttr(kompasApp->GetMainWindowHandle(), {pickedObj}); // Вывести диалог просмотра атрибутов объекта
    chooseMan->Unchoose({pickedObj});
  }
}


//-------------------------------------------------------------------------------
// Просмотреть атрибут 3D объекта
// ---
void ShowObj3DAttr()
{
  if (ISelectionManagerPtr selectionMng = GetSelectionManager(document3D))
  {
    std::vector<IKompasAPIObjectPtr> selectedObjects = selectionMng->GetSelectedObjects();
    if (selectedObjects.size() == 1)
    {
      if (IKompasDocumentPtr doc = kompasApp->GetActiveDocument())
        doc->ViewEditAttr(kompasApp->GetMainWindowHandle(), selectedObjects);
    }
  }
}


//-------------------------------------------------------------------------------
// Просмотреть атрибут
// ---
void ShowObjAttr()
{
  if (IKompasDocumentPtr doc = kompasApp->GetActiveDocument())
  {
    DocumentTypeEnum docType = doc->GetDocumentType(); // тип документа
    switch (docType)
    {
      case ksDocumentDrawing:  // 1- Чертеж
      case ksDocumentFragment: // 2- Фрагмент
        ShowObj2DAttr();
        break;
      case ksDocumentPart:     // 4- Деталь
      case ksDocumentAssembly: // 5- Сборка
        ShowObj3DAttr();
        break;
    }
  }
}

//-------------------------------------------------------------------------------
// Просмотреть библиотеку
// ---
void ShowLib()
{
  // Запросить имя библиотеки
  std::wstring fileName =
    kompasApp->GetApplicationDialogs()->ChoiceFile(nullptr, L"*.lat", L"", L"", ksCurrentDocumentLibraries, false);

  if (IAttrTypeMngPtr attrMng = GetAttrTypeMng(fileName))
  {
    IAttributeTypePtr attrType = attrMng->ChoiceAttrTypes(kompasApp->GetMainWindowHandle());
    NumericId numbType = attrType ? attrType->GetUniqueNumb() : 0;

    if (numbType > 1)
    {
      std::wstring attrTypeInfo = std::vformat(LoadStr(FORMAT_ATTRTYPEINFO3), std::make_wformat_args(numbType));
      LibMessage(attrTypeInfo, ksMessageWarning, ksButtonSetOk, true);
    }
  }
}


//-------------------------------------------------------------------------------
// Просмотреть тип
// ---
void ShowType()
{
  // Запросить имя библиотеки
  std::wstring fileName =
    kompasApp->GetApplicationDialogs()->ChoiceFile(nullptr, L"*.lat", L"", L"", ksCurrentDocumentLibraries, false);

  if (IAttrTypeMngPtr attrMng = GetAttrTypeMng(fileName))
  {
    IAttributeTypePtr attrType = attrMng->ChoiceAttrTypes(kompasApp->GetMainWindowHandle());

    if (attrType && attrType->GetUniqueNumb() > 1000)
    {
      std::wstring password;
      if (kompasApp->GetApplicationDialogs()->ReadString(kompasApp->GetMainWindowHandle(), LoadStr(ENTERPASSWORD), 255, password))
        attrType->ViewEdit(kompasApp->GetMainWindowHandle(), password, true);
    }
  }
}


//-------------------------------------------------------------------------------
// Просмотреть атрибут 3D объекта
// ---
void WalkFromObj3DWithAttr()
{
  if (IKompasDocumentPtr doc = kompasApp->GetActiveDocument())
  {
    if (ISelectionManagerPtr selectionManager = GetSelectionManager(document3D))
    {
      std::vector<IAttributePtr> attributes = doc->GetAttributes(0, 0, 0, 0, 0, selectionManager->GetSelectedObjects());

      size_t count = attributes.size();
      std::wstring attrInfo = std::vformat(LoadStr(FORMAT_ATTRINFO), std::make_wformat_args(count));
      LibMessage(attrInfo, ksMessageWarning, ksButtonSetOk, true);

      for (IAttributePtr & attr : attributes)
      {
        if (attr)
        {
          WindowHandle hwnd = kompasApp->GetMainWindowHandle();
          std::wstring password;
          if (kompasApp->GetApplicationDialogs()->ReadString(hwnd, LoadStr(ENTERPASSWORD), 255, password))
          {
            if (IChooseManagerPtr chooseManager = GetChooseManager(document3D))
            {
              std::vector<IKompasAPIObjectPtr> objects = attr->GetObjects();
              chooseManager->Choose(objects);

              attr->ViewEdit(hwnd, password, false);

              if (LibMessage(LoadStr(DELETEATTRIBUTE), ksMessageWarning, ksButtonSetYesNoCancel, true) == ksButtonYes)
                attr->Delete(password, objects);

              chooseManager->UnchooseAll();
            }
          }
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Просмотреть атрибут
// ---
void WalkFromObj2DWithAttr()
{
  if (IKompasDocumentPtr doc = kompasApp->GetActiveDocument())
  {
    if (ISelectionManagerPtr selectionManager = GetSelectionManager(document2D))
    {
      WindowHandle hwnd = kompasApp->GetMainWindowHandle();
      std::vector<IAttributePtr> attributes = doc->GetAttributes(1, 0, 0, 0, 0, selectionManager->GetSelectedObjects());
      for (IAttributePtr & attr : attributes)
      {
        if (attr)
          attr->ViewEdit(hwnd, L"111", false);
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Просмотреть атрибут
// ---
void WalkFromObjWithAttr()
{
  if (IKompasDocumentPtr doc = kompasApp->GetActiveDocument())
  {
    DocumentTypeEnum docType = doc->GetDocumentType(); // тип документа

    switch (docType)
    {
      case ksDocumentDrawing:  // 1- Чертеж
      case ksDocumentFragment: // 2- Фрагмент
        WalkFromObj2DWithAttr();
        break;
      case ksDocumentPart:     // 4- Деталь
      case ksDocumentAssembly: // 5- Сборка
        WalkFromObj3DWithAttr();
        break;
    }
  }
}


//-------------------------------------------------------------------------------
// Создать атрибут документа
// ---
void CreateDocumentAttr()
{
  if (IKompasDocumentPtr doc = kompasApp->GetActiveDocument())
    LibCreateAttr({doc});
}


//-------------------------------------------------------------------------------
// Удалить атрибуты документа
// ---
void DeleteDocumentAttr()
{
  if (IKompasDocumentPtr doc = kompasApp->GetActiveDocument())
    DeleteAttributes({doc});
}


//-------------------------------------------------------------------------------
// Просмотреть атрибут документа
// ---
void ShowDocumentAttr()
{
  if (IKompasDocumentPtr doc = kompasApp->GetActiveDocument())
    doc->ViewEditAttr(kompasApp->GetMainWindowHandle(), {doc});
}


//-------------------------------------------------------------------------------
// Создать атрибут документа
// ---
void CreateDocument3DAttr() // Создать атрибут 3D документа
{
  IKompasDocumentPtr doc = kompasApp->GetActiveDocument();
  DocumentTypeEnum docType = doc ? doc->GetDocumentType() : ksDocumentUnknown;
  if (docType == ksDocumentPart || docType == ksDocumentAssembly)
    LibCreateAttr({doc});
}


//-------------------------------------------------------------------------------
// Удалитьть атрибут 3D документа
// ---
void DeleteDocument3DAttr()
{
  IKompasDocumentPtr doc = kompasApp->GetActiveDocument();
  DocumentTypeEnum docType = doc ? doc->GetDocumentType() : ksDocumentUnknown;

  if (docType == ksDocumentPart || docType == ksDocumentAssembly)
  {
    if (doc)
    {
      std::vector<IAttributePtr> attributes = doc->GetAttributes(0, 0, 0, 0, 0, {doc});
      size_t attributesCount = attributes.size();
      size_t delAttr = 0;

      std::wstring attrInfo = std::vformat(LoadStr(FORMAT_ATTRINFO), std::make_wformat_args(attributesCount));
      LibMessage(attrInfo, ksMessageWarning, ksButtonSetOk, true);

      for (IAttributePtr & attr : attributes)
      {
        if (attr)
        {
          WindowHandle hwnd = kompasApp->GetMainWindowHandle();
          std::wstring password;
          if (kompasApp->GetApplicationDialogs()->ReadString(hwnd, LoadStr(ENTERPASSWORD), 255, password))
          {
            // Просмотр атрибутов
            attr->ViewEdit(hwnd, password, true);

            // Удаление атрибутов
            if (LibMessage(LoadStr(DELETEATTRIBUTE), ksMessageError, ksButtonSetYesNo, true) == ksButtonYes)
            {
              if (attr->Delete(password, {doc}))
                delAttr++;
            }
          }
        }
      }

      attributesCount -= delAttr;
      attrInfo = std::vformat(LoadStr(FORMAT_ATTRREMAINED), std::make_wformat_args(attributesCount));
      LibMessage(attrInfo, ksMessageWarning, ksButtonSetOk, true);
    }
  }
}


//-------------------------------------------------------------------------------
// Просмотреть атрибут 3D документа
// ---
void ShowDocument3DAttr()
{
  IKompasDocumentPtr doc = kompasApp->GetActiveDocument();
  DocumentTypeEnum docType = doc ? doc->GetDocumentType() : ksDocumentUnknown;

  if (docType == ksDocumentPart || docType == ksDocumentAssembly)
    doc->ViewEditAttr(kompasApp->GetMainWindowHandle(), {nullptr});
}


//-------------------------------------------------------------------------------
// Просмотреть атрибуты документа и детали сборки
// ---
void ShowObjects3DAttr()
{
  IKompasDocumentPtr doc = kompasApp->GetActiveDocument();
  DocumentTypeEnum docType = doc ? doc->GetDocumentType() : ksDocumentUnknown;

  if (docType == ksDocumentAssembly || docType == ksDocumentPart)
  {
    IKompasDocument3DPtr doc3D = doc;
    if (doc3D)
    {
      WindowHandle hwnd = kompasApp->GetMainWindowHandle();
      std::vector<IKompasAPIObjectPtr> objects = doc3D->GetObjectsByAttr(0, 0, 0, 0, 0, {o3d_unknown});
      if (objects.size())
        doc3D->ViewEditAttr(hwnd, objects); // Просмотр атрибутов документа

      if (docType == ksDocumentAssembly)
      {
        if (IPartPtr part0 = doc3D->GetTopPart()->GetParts()->GetPart(0)) // Первая деталь
        {
          if (IFeaturePtr feature0 = part0) // Лист первой детали
          {
            // Листы элеменов детали
            std::vector<IFeaturePtr> subfeatures0 = feature0->GetSubFeatures(ksOperTree, true, true);
            if (subfeatures0.size())
            {
              std::vector<IKompasAPIObjectPtr> objs(subfeatures0.begin(), subfeatures0.end());
              doc->ViewEditAttr(hwnd, objs); // Просмотр атрибутов
            }
          }
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
// Обработчик команд библиотеки.
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  IKompasDocumentPtr doc = kompasApp->GetActiveDocument();
  DocumentTypeEnum docType = doc ? doc->GetDocumentType() : ksDocumentUnknown;

  switch (docType)
  {
    case ksDocumentUnknown:       // Нет документа (работаем с библиотекой атрибутов)
    case ksDocumentDrawing:       // Чертеж.
    case ksDocumentFragment:      // Фрагмент.
    case ksDocumentSpecification: // Спецификация.
    case ksDocumentTextual:       // Текстовый документ
    case ksDocumentPart:          // Деталь.
    case ksDocumentAssembly:      // Сборка.

      switch (commandId)
      {
        case 1:
          FuncTypeAttr();
          break; // Создать тип атрибута
        case 2:
          DelTypeAttr();
          break; // Удалить тип атрибута
        case 3:
          ShowTypeAttr();
          break; // Получить тип атрибута
        case 4:
          ChangeType(); // Заменить тип атрибута
          break;
        case 5:
          NewAttr();
          break; // Создать атрибут определенного типа
        case 6:
          DelObjAttr();
          break; // Удалить атрибут
        case 7:
          ReadObjAttr();
          break; // Считать атрибут
        case 8:
          ShowObjAttr();
          break; // Просмотреть атрибут
        case 9:
          ShowLib();
          break; // Просмотреть библиотеку
        case 10:
          ShowType();
          break; // Просмотреть тип
        case 11:
          WalkFromObjWithAttr();
          break; // Просмотреть атрибут
        case 12:
          CreateDocumentAttr();
          break; // Создать атрибут документа
        case 13:
          DeleteDocumentAttr();
          break; // Удалить атрибут документа
        case 14:
          ShowDocumentAttr();
          break; // Просмотреть атрибут документа
        case 15:
          CreateDocument3DAttr();
          break; // Создать атрибут 3D документа
        case 16:
          DeleteDocument3DAttr();
          break; // Удалить атрибут 3D документа
        case 17:
          ShowDocument3DAttr();
          break; // Просмотреть атрибут 3D документа
        case 18:
          ShowObjects3DAttr();
          break; // Просмотреть атрибут объектов 3D документа
      }
  }
}


//------------------------------------------------------------------------------
/**
  Получить номер версии приложения.
*/
//---
void GetApplicationVersion(int32_t & major, int32_t & minor, int32_t & release, int32_t & build)
{
  major = BuildVersionMajor;
  minor = BuildVersionMinor;
  release = BuildVersionPatch;
  build = BuildVersionBuild;
}


//------------------------------------------------------------------------------
// Инициализация библиотеки.
//---
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libaryActions)
{
  libaryActions.AddGetApplicationVersion(GetApplicationVersion);
  libaryActions.AddRunCommandHandler(RunCommand);

  kompasApp = &app;
  LoadResources(app);

  return true;
}


//------------------------------------------------------------------------------
// Освобождения ресурсов перед выгрузкой библиотеки.
//---
APP_EXP_FUNC(void) UnloadKompasLibrary()
{
}
