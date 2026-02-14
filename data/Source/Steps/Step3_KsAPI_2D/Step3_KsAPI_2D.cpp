////////////////////////////////////////////////////////////////////////////////
//
// Step3_API7_2D.cpp - Обозначения 2D
//
// 	1:	LeaderWork( doc );				- Создание и редактирование простой линии выноски
// 	2:	MarkLeaderWork( doc );		- Создание и редактирование знака маркировки
// 	3:	ChangeLeaderWork( doc );	- Создание и редактирование знака изменения
// 	4:	BrandLeaderWork( doc );		- Создание и редактирование знака клеймения
// 	5:	RoughWork( doc );					- Создание и редактирование обозначения шероховатости
// 	6:	BaseWork( doc );					- Создание и редактирование обозначения базы
// 	7:	CutLineWork( doc );				- Создание и редактирование линии разреза/сечения
// 	8:	ViewPointerWork( doc );		- Создание и редактирование стрелки направления взгляда
// 	9:	ToleranceWork( doc );			- Создание и редактирование допуска формы
// 	10:	ObjectsNavigation( doc );	- Навигация по массиву объектов
//
////////////////////////////////////////////////////////////////////////////////

#include "Step3_KsAPI_2D.h"
#include "CursorProcess.h"
#include "Step3_KsAPI_2D_pch.h"
#include "Resource.h"
#include <format>

ksapi::IApplication * ksAppl = nullptr;

namespace lib_func
{
void LightObj(ksapi::IKompasAPIObjectPtr lightObj, bool choose);
std::int32_t LibMessage(const std::wstring & message, ksMessageTypeEnum dlgType = ksMessageInformation,
                        ksMessageButtonSetEnum dlgButtons = ksButtonSetOk, bool showModal = false);
std::int32_t YesNoMessage(const std::wstring & message);
ksapi::IProcessPtr CreateProcess(ksProcess2DTypeEnum procType);
} // namespace lib_func


/// Создание и редактирование простой линии выноски
void LeaderWork(ksapi::IKompasDocument2DPtr & doc);
/// Создание и редактирование знака маркировки
void MarkLeaderWork(ksapi::IKompasDocument2DPtr & doc);
/// Создание и редактирование знака изменения
void ChangeLeaderWork(ksapi::IKompasDocument2DPtr & doc);
/// Создание и редактирование знака клеймения
void BrandLeaderWork(ksapi::IKompasDocument2DPtr & doc);
/// Создание и редактирование обозначения шероховатости
void RoughWork(ksapi::IKompasDocument2DPtr & doc);
/// Создание и редактирование обозначения базы
void BaseWork(ksapi::IKompasDocument2DPtr & doc);
/// Создание и редактирование линии разреза/сечения
void CutLineWork(ksapi::IKompasDocument2DPtr & doc);
/// Создание и редактирование стрелки направления взгляда
void ViewPointerWork(ksapi::IKompasDocument2DPtr & doc);
/// Создание и редактирование допуска формы
void ToleranceWork(ksapi::IKompasDocument2DPtr & doc);
/// Навигация по массиву объектов вида
void ObjectsNavigation(ksapi::IKompasDocument2DPtr & doc);


//------------------------------------------------------------------------------
/**
  Задать идентификатор ресурсов
*/
//---
std::int32_t GetLibraryId()
{
  return IDR_LIBID;
}


//------------------------------------------------------------------------------
/**
  Получить активный документ
*/
//---
ksapi::IKompasDocument2DPtr GetCurrentDocument()
{
  return ksAppl ? ksAppl->GetActiveDocument() : nullptr;
}


//------------------------------------------------------------------------------
/**
  Головная функция библиотеки
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  // Получить текущий документ
  ksapi::IKompasDocument2DPtr doc = GetCurrentDocument();

  if (doc)
  {
    switch (commandId)
    {
      case CM_LEADER:
        LeaderWork(doc);
        break; // Создание и редактирование простой линии выноски
      case CM_MARK_LEADER:
        MarkLeaderWork(doc);
        break; // Создание и редактирование знака маркировки
      case CM_CHANGE_LEADER:
        ChangeLeaderWork(doc);
        break; // Создание и редактирование знака изменения
      case CM_BRAND_LEADER:
        BrandLeaderWork(doc);
        break; // Создание и редактирование знака клеймения
      case CM_ROUGH:
        RoughWork(doc);
        break; // Создание и редактирование обозначения шероховатости
      case CM_BASE:
        BaseWork(doc);
        break; // Создание и редактирование обозначения базы
      case CM_CUT_LINE:
        CutLineWork(doc);
        break; // Создание и редактирование линии разреза/сечения
      case CM_VIEW_POINTER:
        ViewPointerWork(doc);
        break; // Создание и редактирование стрелки направления взгляда
      case CM_TOLERANCE:
        ToleranceWork(doc);
        break; // Создание и редактирование допуска формы
      case CM_OBJECTS_NAVIGATION:
        ObjectsNavigation(doc);
        break; // Навигация по массиву объектов
    }
  }
  else
  {
    lib_func::LibMessage(LoadStr(IDS_NODOC), ksMessageError);
  }
}

extern void LoadResources(ksapi::IApplication & app);

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
/**
  Инициализация библиотеки.
*/
//---
APP_EXP_FUNC(bool) LoadKompasLibrary(ksapi::IApplication & app, ksapi::IKompasLibraryActions & libraryActions)
{
  libraryActions.AddGetApplicationVersion(GetApplicationVersion);
  libraryActions.AddRunCommandHandler(RunCommand);
  libraryActions.AddGetMenuIdHandler(GetLibraryId);

  ksAppl = &app;
  LoadResources(app);

  return true;
}


//------------------------------------------------------------------------------
/**
  Освобождения ресурсов перед выгрузкой библиотеки.
*/
//---
APP_EXP_FUNC(void) UnloadKompasLibrary()
{
}


//-------------------------------------------------------------------------------
//  Получить контейнер обозначений 2D
// ---
ksapi::ISymbols2DContainerPtr GetSymbols2DContainer(ksapi::IKompasDocument2DPtr & doc)
{
  ksapi::ISymbols2DContainerPtr res;

  if (doc)
  {
    // Получим менеджер для работы с видами и слоями
    if (ksapi::IViewsAndLayersManagerPtr viewsMng = doc->GetViewsAndLayersManager())
    {
      // Получим коллекцию видов
      if (ksapi::IViewsPtr views = viewsMng->GetViews())
        // Получаем контейнер
        res = views->GetActiveView();
    }
  }

  return res;
}


//-------------------------------------------------------------------------------
// Создание простой линии выноски
// ---
void CreateLeader(ksapi::ILeaderPtr & leader)
{
  if (leader)
  {
    // Направление полки - вправо
    leader->SetShelfDirection(ksLSRight);

    // Получаем интерфейс ответвлений
    if (ksapi::IBranchsPtr branchs = leader)
    {
      // Координаты начала полки или точка привязки
      branchs->SetX0(100);
      branchs->SetY0(150);
      // Добавить прямолинейные ответвления
      branchs->AddBranchByPoint(-1, 60, 120);
      branchs->AddBranchByPoint(-1, 65, 105);
    }

    // Получить интерфейс текста над полкой
    if (ksapi::ITextPtr txtOnSh = leader->GetTextOnShelf())
      // Изменить текст
      txtOnSh->SetStr(L"1");

    // Получить интерфейс текста под полкой
    if (ksapi::ITextPtr txtUnderSh = leader->GetTextUnderShelf())
      // Изменить текст
      txtUnderSh->SetStr(L"2");

    // Получить интерфейс текста над ножкой
    if (ksapi::ITextPtr txtOnBr = leader->GetTextOnBranch())
      // Изменить текст
      txtOnBr->SetStr(L"3");

    // Получить интерфейс текста под ножкой
    if (ksapi::ITextPtr txtUnderBr = leader->GetTextUnderBranch())
      // Изменить текст
      txtUnderBr->SetStr(L"4");

    // Получить интерфейс текста за полкой
    if (ksapi::ITextPtr txtAfterSh = leader->GetTextAfterShelf())
      // Изменить текст
      txtAfterSh->SetStr(L"5");

    if (ksapi::IBaseLeaderPtr baseLeader = leader)
      // Применить параметры
      baseLeader->Update();
  }
}


//-------------------------------------------------------------------------------
// Задать компоненту текста
// ---
void AddTextItem(ksapi::ITextLinePtr & line, const std::wstring & str, ksTextItemEnum type)
{
  if (line)
  {
    // Получить интерфейс компоненты текста
    if (ksapi::ITextItemPtr item = line->Add())
    {
      // Задать строковое значение
      item->SetStr(str);
      // Задать тип
      item->SetItemType(type);
      // Применить параметры
      item->Update();
    }
  }
}


//-------------------------------------------------------------------------------
// Редактирование линии выноски
// ---
void EditLeader(ksapi::ILeaderPtr & leader)
{
  if (leader)
  {
    // Тип значка - знак склеивания
    leader->SetSignType(ksLGlueSign);
    // Включить признак обоработки по контуру
    leader->SetArround(true);

    // Получаем интерфейс ответвлений
    if (ksapi::IBranchsPtr branchs = leader)
      // Добавить прямолинейное ответвление
      branchs->AddBranchByPoint(-1, 140, 120);

    // Начало созданного ответвления - от конца полки
    leader->SetBranchBegin(2, false);

    // Получить текст над полкой
    if (ksapi::ITextPtr txt = leader->GetTextOnShelf())
    {
      txt->SetStr(L"");

      if (ksapi::ITextLinePtr line = txt->Add())
      {
        // Добавить строку
        AddTextItem(line, L"1", ksTItString);
        // Добавить числитель дроби
        AddTextItem(line, L"1", ksTItNumerator);
        // Добавить знаментатель дроби
        AddTextItem(line, L"1", ksTItDenominator);
        // Закончить дробь
        AddTextItem(line, L"1", ksTItFractionEnd);
      }
    }

    if (ksapi::IBaseLeaderPtr baseLeader = leader)

      if (baseLeader)
        // Применить параметры
        baseLeader->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Создание и редактирование простой линии выноски
*/
//---
void LeaderWork(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию линий выноски
    if (ksapi::ILeadersPtr leadersCol = symbCont->GetLeaders())
    {
      // Добавить простую линию выноски
      if (ksapi::ILeaderPtr leader = leadersCol->Add(ksDrLeader))
      {
        // Создать линию выноски
        CreateLeader(leader);

        // Подсветить созданный объект
        if (ksapi::IBaseLeaderPtr bLeader = leader)
        {
          lib_func::LightObj(bLeader, true /*включить подсветку*/);

          if (lib_func::YesNoMessage(LoadStr(IDS_EDIT)) == 1)
          {
            // Редактировать линию выноски
            EditLeader(leader);
          }

          lib_func::LightObj(bLeader, false /*выключить подсветку*/);
        }
      }
    }
  }
}


//-------------------------------------------------------------------------------
// Создание знака маркировки
// ---
void CreateMarkLeader(ksapi::IMarkLeaderPtr & markLeader)
{
  if (markLeader)
  {
    // Получить интерфейс ответвлений
    if (ksapi::IBranchsPtr branchs = markLeader)
    {
      // Точка привязки
      branchs->SetX0(100);
      branchs->SetY0(190);
      // Добавить прямолинейное ответвление
      branchs->AddBranchByPoint(-1, 60, 120);
    }

    // Получить интерфейс текста обозначения
    if (ksapi::ITextPtr des = markLeader->GetDesignation())
      // Изменить текст
      des->SetStr(LoadStr(IDS_MARK));

    // Получить интерфейс текста над ножкой
    if (ksapi::ITextPtr textOnBranch = markLeader->GetTextOnBranch())
      // Изменить текст
      textOnBranch->SetStr(L"2");

    // Получить интерфейс текста под ножкой
    if (ksapi::ITextPtr textUnderBranch = markLeader->GetTextUnderBranch())
      // Изменить текст
      textUnderBranch->SetStr(L"3");

    // Получить базовый интерфейс линии выноски
    if (ksapi::IBaseLeaderPtr baseLeader = markLeader)
      // Применить параметры
      baseLeader->Update();
  }
}


//-------------------------------------------------------------------------------
// Редактирование знака маркировки
// ---
void EditMarkLeader(ksapi::IMarkLeaderPtr & markLeader)
{
  if (markLeader)
  {
    // Получить интерфейс ответвлений
    if (ksapi::IBranchsPtr branchs = markLeader)
      // Добавить прямолинейное ответвление
      branchs->AddBranchByPoint(-1, 70, 110);

    // Получить базовый интерфейс линии выноски
    if (ksapi::IBaseLeaderPtr baseLeader = markLeader)
    {
      // Тип стрелки
      baseLeader->SetArrowType(ksLeaderArrow);
      // Применить изменения
      baseLeader->Update();
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создание и редактирование знака маркировки
*/
//---
void MarkLeaderWork(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию линий выноски
    if (ksapi::ILeadersPtr leadersCol = symbCont->GetLeaders())
    {
      // Добавить знак маркировки
      if (ksapi::IMarkLeaderPtr mLeader = leadersCol->Add(ksDrMarkerLeader))
      {
        // Создать знак маркировки
        CreateMarkLeader(mLeader);

        // Получить референс объекта
        if (ksapi::IBaseLeaderPtr bLeader = mLeader)
        {
          // Подсветить созданный объект
          lib_func::LightObj(bLeader, true /*включить подсветку*/);

          if (lib_func::YesNoMessage(LoadStr(IDS_EDIT)) == 1)
          {
            // Редактировать знак маркировки
            EditMarkLeader(mLeader);
          }

          lib_func::LightObj(bLeader, false /*выключить подсветку*/);
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создание знака изменения
*/
//---
void CreateChangeLeader(ksapi::IChangeLeaderPtr & changeLeader)
{
  if (changeLeader)
  {
    // Тип значка - квадрат
    changeLeader->SetSignType(ksCLSSquare);

    // Получить интерфейс ответвлений
    if (ksapi::IBranchsPtr branchs = changeLeader)
    {
      // Точка привязки
      branchs->SetX0(70);
      branchs->SetY0(150);
      // Добавить ответвление
      branchs->AddBranchByPoint(-1, 40, 130);
    }

    // Получить интерфейс текста обозначения
    if (ksapi::ITextPtr des = changeLeader->GetDesignation())
      // Изменить текст
      des->SetStr(L"1");

    // Получить базовый интерфейс линии выноски
    if (ksapi::IBaseLeaderPtr baseLeader = changeLeader)
      // Применить параметры
      baseLeader->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Редактирование знака изменения
*/
//---
void EditChangeLeader(ksapi::IChangeLeaderPtr & changeLeader)
{
  if (changeLeader)
  {
    // Тип значка - круг
    changeLeader->SetSignType(ksCLSCircle);
    // Полная длина выноски отключена
    changeLeader->SetFullLeaderLength(false);
    // Длина выноски
    changeLeader->SetLeaderLength(5);

    // Получить базовый интерфейс линии выноски
    if (ksapi::IBaseLeaderPtr baseLeader = changeLeader)
      // Применить изменения
      baseLeader->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Создание и редактирование знака изменения
*/
//---
void ChangeLeaderWork(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию линий выноски
    if (ksapi::ILeadersPtr leadersCol = symbCont->GetLeaders())
    {
      // Добавить знак изменения
      if (ksapi::IChangeLeaderPtr chLeader = leadersCol->Add(ksDrChangeLeader))
      {
        // Создать знак изменения
        CreateChangeLeader(chLeader);

        // Подсветить созданный объект
        if (ksapi::IBaseLeaderPtr bLeader = chLeader)
        {
          lib_func::LightObj(bLeader, true /*включить подсветку*/);

          if (lib_func::YesNoMessage(LoadStr(IDS_EDIT)) == 1)
          {
            // Редактировать знак изменения
            EditChangeLeader(chLeader);
          }

          lib_func::LightObj(bLeader, 0 /*выключить подсветку*/);
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создание знака клеймения
*/
//---
void CreateBrandLeader(ksapi::IBrandLeaderPtr & brandLeader)
{
  if (brandLeader)
  {
    // Направление
    brandLeader->SetDirection(false);

    // Получить интерфейс текста обозначения
    if (ksapi::ITextPtr des = brandLeader->GetDesignation())

      if (des)
        // Изменить текст
        des->SetStr(LoadStr(IDS_MARK2));

    // Получить интерфейс ответвлений
    if (ksapi::IBranchsPtr branchs = brandLeader)
    {
      // Точка привязки
      branchs->SetX0(100);
      branchs->SetY0(150);
      // Добавить ответвление
      branchs->AddBranchByPoint(-1, 60, 110);
    }

    // Получить базовый интерфейс линии выноски
    if (ksapi::IBaseLeaderPtr baseLeader = brandLeader)
      // Применить параметры
      baseLeader->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Редактирование знака изменения
*/
//---
void EditBrandLeader(ksapi::IBrandLeaderPtr & brandLeader)
{
  if (brandLeader)
  {
    // Направление
    brandLeader->SetDirection(true);

    // Получить интерфейс текста над ножкой
    if (ksapi::ITextPtr textOn = brandLeader->GetTextOnBranch())
      // Изменить текст
      textOn->SetStr(L"2");

    // Получить интерфейс текста под ножкой
    if (ksapi::ITextPtr textUnder = brandLeader->GetTextUnderBranch())
      // Изменить текст
      textUnder->SetStr(L"3");

    // Получить интерфейс текста обозначения
    if (ksapi::ITextPtr des = brandLeader->GetDesignation())
      // Изменить текст
      des->SetStr(LoadStr(IDS_MARK3));

    // Получить базовый интерфейс линии выноски
    if (ksapi::IBaseLeaderPtr baseLeader = brandLeader)
      // Применить параметры
      baseLeader->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Создание и редактирование знака клеймения
*/
//---
void BrandLeaderWork(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию линий выноски
    if (ksapi::ILeadersPtr leadersCol = symbCont->GetLeaders())
    {
      // Добавить знак клеймения
      if (ksapi::IBrandLeaderPtr brLeader = leadersCol->Add(ksDrBrandLeader))
      {
        // Создать знак клеймения
        CreateBrandLeader(brLeader);

        // Получить референс объекта
        if (ksapi::IBaseLeaderPtr bLeader = brLeader)
          // Подсветить созданный объект
          lib_func::LightObj(bLeader, true /*включить подсветку*/);

        if (lib_func::YesNoMessage(LoadStr(IDS_EDIT)) == 1)
        {
          // Редактировать знак клеймения
          EditBrandLeader(brLeader);
        }

        lib_func::LightObj(brLeader, false /*выключить подсветку*/);
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создание обозначения шероховатости
*/
//---
bool CreateRough(ksapi::IRoughPtr & rough)
{
  bool res = false;

  if (rough)
  {
    // TODO:
    //// Отключить привязки
    //SnapOptions sOpt;
    //ksGetSysOptions(SNAP_OPTIONS, &sOpt, sizeof(sOpt));
    //sOpt.commonOpt = FALSE;
    //ksSetSysOptions(SNAP_OPTIONS, &sOpt, sizeof(sOpt));

    ksapi::IDrawingObjectPtr baseObj;

    auto findObjCall = [&](double x, double y)
    {
      // Найти объект по указанным координатам
      if (ksapi::IKompasDocument2DPtr doc2D = GetCurrentDocument())
        baseObj = doc2D->FindObject(x, y, /*TODO: ksGetCursorLimit()*/ 2.5, nullptr);

      // Подсветить объект
      lib_func::LightObj(baseObj, true /*включить подсветку*/);

      // Если привязки выключены, включить
      //if (!sOpt.commonOpt)
      //{
      //  sOpt.commonOpt = TRUE;
      //  ksSetSysOptions(SNAP_OPTIONS, &sOpt, sizeof(sOpt));
      //}
    };

    CursorProcess cursorProcess;

    if (ksapi::IProcessPtr & process = cursorProcess.Get())
    {
      cursorProcess.Get()->SetCaption(LoadStr(IDS_COMMAND1));
      cursorProcess.Get()->SetCursorId(ksCursorCatch);
    }

    cursorProcess.Execute(findObjCall);

    // Базовый объект
    if (baseObj)
    {
      auto setRoughCall = [&](double x, double y)
      {
        rough->SetBaseObject(baseObj);

        // Точка привязки
        rough->SetBranchX0(x);
        rough->SetBranchY0(y);

        if (ksapi::IRoughParamsPtr roughPar = rough)
        {
          roughPar->SetShelfDirection(ksLSRight);
          rough->Update();

          // Обработка по контуру включена
          roughPar->SetProcessingByContour(true);
          // Длина линии выноски
          roughPar->SetLeaderLength(20);
          // Угол наклона линии выноски
          roughPar->SetLeaderAngle(45);

          // Получить интерфейс текста параметров шероховатости
          if (ksapi::ITextPtr txt1 = roughPar->GetRoughParamText())
            txt1->SetStr(L"1");

          // Получить интерфейс текста способа обработки поверхности
          if (ksapi::ITextPtr txt2 = roughPar->GetProcessText())
            txt2->SetStr(L"2");

          // Получить интерфейс текста базовой длины
          if (ksapi::ITextPtr txt3 = roughPar->GetBaseLengthText())
            txt3->SetStr(L"3");
        }

        // Применить параметры
        rough->Update();
        res = true;
      };

      if (ksapi::IProcessPtr & process = cursorProcess.Get())
      {
        cursorProcess.Get()->SetCaption(LoadStr(IDS_COMMAND2));
        cursorProcess.Get()->SetCursorId(ksCursorDefault);
      }
      cursorProcess.Execute(setRoughCall);

      if (!res)
        lib_func::LibMessage(LoadStr(IDS_NOPOINT));
    }
    else
      lib_func::LibMessage(LoadStr(IDS_NOOBJ));


    lib_func::LightObj(baseObj, false /*выключить подсветку*/);
    // TODO:
    // Если привязки выключены, включить
    //if (!sOpt.commonOpt)
    //{
    //  sOpt.commonOpt = TRUE;
    //  ksSetSysOptions(SNAP_OPTIONS, &sOpt, sizeof(sOpt));
    //}
  }
  return res;
}


//------------------------------------------------------------------------------
/**
  Редактирование обозначения шероховатости
*/
//---
void EditRough(ksapi::IRoughPtr & rough)
{
  if (rough)
  {
    // Получить интерфейс параметров шероховатости
    if (ksapi::IRoughParamsPtr roughPar = rough)
    {
      // Убрать стрелку
      roughPar->SetArrowType(ksWithoutArrow);
      // Обработка по контуру отключена
      roughPar->SetProcessingByContour(false);
    }

    // Применить изменения
    rough->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Создание и редактирование обозначения шероховатости
*/
//---
void RoughWork(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию обозначений шероховатости
    if (ksapi::IRoughsPtr roughsCol = symbCont->GetRoughs())
    {
      // Добавить обозначение шероховатости
      if (ksapi::IRoughPtr newRough = roughsCol->Add())
      {
        // Создать обозначение шероховатости
        if (CreateRough(newRough))
        {
          // Подсветить созданный объект
          lib_func::LightObj(newRough, true /*включить подсветку*/);

          if (lib_func::YesNoMessage(LoadStr(IDS_EDIT)) == 1)
          {
            // Редактировать обозначение шероховатости
            EditRough(newRough);
          }

          lib_func::LightObj(newRough, false /*выключить подсветку*/);
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создание обозначения базы
*/
//---
bool CreateBase(ksapi::IBasePtr & base)
{
  bool res = false;

  if (base)
  {
    // TODO:
    //// Отключить привязки
    //SnapOptions sOpt;
    //ksGetSysOptions(SNAP_OPTIONS, &sOpt, sizeof(sOpt));
    //sOpt.commonOpt = FALSE;
    //ksSetSysOptions(SNAP_OPTIONS, &sOpt, sizeof(sOpt));

    ksapi::IDrawingObjectPtr baseObj;

    // Указать базовый объект
    auto findObjCall = [&](double x, double y)
    {
      // Найти объект по указанным координатам
      if (ksapi::IKompasDocument2DPtr doc2D = GetCurrentDocument())
        baseObj = doc2D->FindObject(x, y, /*TODO: ksGetCursorLimit()*/ 2.5, nullptr);

      // Подсветить объект
      lib_func::LightObj(baseObj, true /*включить подсветку*/);

      // TODO:
      // Если привязки выключены, включить
      //if (!sOpt.commonOpt)
      //{
      //  sOpt.commonOpt = TRUE;
      //  ksSetSysOptions(SNAP_OPTIONS, &sOpt, sizeof(sOpt));
      //}
    };

    CursorProcess cursorProcess;
    if (ksapi::IProcessPtr & process = cursorProcess.Get())
    {
      process->SetCaption(LoadStr(IDS_COMMAND1));
      process->SetCursorId(ksCursorCatch);
    }

    cursorProcess.Execute(findObjCall);


    if (baseObj)
    {
      // Указать положение знака
      auto setBaseCall = [&](double x, double y)
      {
        // Базовый объект
        base->SetBaseObject(baseObj);

        // Точка положения знака
        base->SetX0(x);
        base->SetY0(y);
        // Способ отрисовки - перпендикулярно к объекту
        base->SetDrawType(true);
        // Применить параметры
        base->Update();
        res = true;
      };

      if (ksapi::IProcessPtr & process = cursorProcess.Get())
      {
        process->SetCaption(LoadStr(IDS_COMMAND2));
        process->SetCursorId(ksCursorDefault);
      }

      cursorProcess.Execute(setBaseCall);
    }
    else
      lib_func::LibMessage(LoadStr(IDS_NOOBJ));

    if (!res)
      lib_func::LibMessage(LoadStr(IDS_NOPOINT));

    lib_func::LightObj(baseObj, false /*выключить подсветку*/);
    // TODO:
    // Если привязки выключены, включить
    //if (!sOpt.commonOpt)
    //{
    //  sOpt.commonOpt = TRUE;
    //  ksSetSysOptions(SNAP_OPTIONS, &sOpt, sizeof(sOpt));
    //}
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Редактирование обозначения базы
*/
//---
void EditBase(ksapi::IBasePtr & base)
{
  if (base)
  {
    // Тип отрисовки - произвольно
    base->SetDrawType(false);
    // Отключить автосортировку
    base->SetAutoSorted(false);
    // Конечная точка выноски
    base->SetBranchX(base->GetX0() + 10);
    base->SetBranchY(base->GetY0() + 10);

    // Получить интерфейс текста обозначения базы
    if (ksapi::ITextPtr txt = base->GetText())
      // Изменить текст
      txt->SetStr(L"B");

    // Применить параметры
    base->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Создание и редактирование обозначения базы
*/
//---
void BaseWork(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию обозначений базы
    if (ksapi::IBasesPtr basesCol = symbCont->GetBases())
    {
      // Добавить обозначение базы
      if (ksapi::IBasePtr newBase = basesCol->Add())
      {
        // Создать обозначение базы
        if (CreateBase(newBase))
        {
          // Подсветить созданный объект
          lib_func::LightObj(newBase, true /*включить подсветку*/);

          if (lib_func::YesNoMessage(LoadStr(IDS_EDIT)) == 1)
          {
            // Редактировать обозначение базы
            EditBase(newBase);
          }

          lib_func::LightObj(newBase, false /*выключить подсветку*/);
        }
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создать текст с заданным размером шрифта
*/
//---
void SetTextSmallFont(ksapi::ITextPtr & txt, const std::wstring & str, double size)
{
  if (txt)
  {
    // Получить интерфейс строки текста
    if (ksapi::ITextLinePtr line = txt->Add())
    {
      // Получить интерфейс компоненты текста
      if (ksapi::ITextItemPtr item = line->Add())
      {
        // Получить интерфейс шрифта
        if (ksapi::ITextFontPtr font = item)
          // Задать высоту шрифта
          font->SetHeight(size);

        // Задать значение строки
        item->SetStr(str);
        // Применить параметры
        item->Update();
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создание линии разреза/сечения
*/
//---
void CreateCutLine(ksapi::ICutLinePtr & cutLine)
{
  if (cutLine)
  {
    // Координаты начального текста
    cutLine->SetX1(80);
    cutLine->SetY1(165);
    // Координаты конечного текста
    cutLine->SetX2(120);
    cutLine->SetY2(200);
    // Расположение стрелок - слева
    cutLine->SetArrowPos(true);
    // Размещение дополнительного текста - у первой стрелки
    cutLine->SetAdditionalTextPos(true);

    // Создать массив точек линии разреза
    std::vector<double> points{80, 165, 120, 200};

    // Задать массив точек линии разреза
    cutLine->SetPoints(points);

    // Получить интерфейс дополнительного текста
    if (ksapi::ITextPtr adText = cutLine->GetAdditionalText())
      SetTextSmallFont(adText, L"(1)", 7);

    // Применить параметры
    cutLine->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Редактирование линии разреза/сечения
*/
//---
void EditCutLine(ksapi::ICutLinePtr & cutLine)
{
  if (cutLine)
  {
    // Расположение стрелок - справа по направлению ломаной
    cutLine->SetArrowPos(false);
    // Расположение дополнительного текста - у второй стрелки
    cutLine->SetAdditionalTextPos(false);
    // Отключить автосортировку
    cutLine->SetAutoSorted(false);

    // Создать массив точек линии разреза
    std::vector<double> points{80, 165, 115, 165, 120, 200};

    // Задать массив точек линии разреза
    cutLine->SetPoints(points);
    // Применить изменения
    cutLine->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Создание и редактирование линии разреза/сечения
*/
//---
void CutLineWork(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию линий разреза/сечения
    if (ksapi::ICutLinesPtr cutCol = symbCont->GetCutLines())
    {
      // Добавить линию разреза/сечения
      if (ksapi::ICutLinePtr newCut = cutCol->Add())
      {
        // Создать линию разреза/сечения
        CreateCutLine(newCut);

        // Подсветить созданный объект
        lib_func::LightObj(newCut, true /*включить подсветку*/);

        if (lib_func::YesNoMessage(LoadStr(IDS_EDIT)) == 1)
        {
          // Редактировать линию разреза/сечения
          EditCutLine(newCut);
        }

        lib_func::LightObj(newCut, false /*выключить подсветку*/);
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создание стрелки направления взгляда
*/
//---
void CreateViewPointer(ksapi::IViewPointerPtr & viewPointer)
{
  if (viewPointer)
  {
    // Начальная точка стрелки
    viewPointer->SetX1(100);
    viewPointer->SetY1(150);
    // Конечная точка стрелки
    viewPointer->SetX2(120);
    viewPointer->SetY2(160);
    // Применить параметры
    viewPointer->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Редактирование стрелки направления взгляда
*/
//---
void EditViewPointer(ksapi::IViewPointerPtr & viewPointer)
{
  if (viewPointer)
  {
    // Конечная точка стрелки
    viewPointer->SetX2(90);
    viewPointer->SetY2(140);
    // Включить автосортировку
    viewPointer->SetAutoSorted(true);

    // Получить интерфейс текста обозначения направления взгляда
    if (ksapi::ITextPtr adText = viewPointer->GetAdditionalText())
    {
      double fontSize = 7;
      // Изменить текст
      SetTextSmallFont(adText, L"(1)", fontSize);
    }

    // Применить параметры
    viewPointer->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Создание и редактирование стрелки направления взгляда
*/
//---
void ViewPointerWork(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию стрелок направления взгляда
    if (ksapi::IViewPointersPtr viewPointsCol = symbCont->GetViewPointers())
    {
      // Добавить стрелку направления взгляда
      if (ksapi::IViewPointerPtr newPointer = viewPointsCol->Add())
      {
        // Создать стрелку направления взгляда
        CreateViewPointer(newPointer);

        // Подсветить созданный объект
        lib_func::LightObj(newPointer, true /*включить подсветку*/);

        if (lib_func::YesNoMessage(LoadStr(IDS_EDIT)) == 1)
        {
          // Редактировать стрелку направления взгляда
          EditViewPointer(newPointer);
        }

        lib_func::LightObj(newPointer, false /*выключить подсветку*/);
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Записать текст в ячейку
*/
//---
void PutTextIntoCell(ksapi::ITablePtr & table, std::int32_t row, std::int32_t column, const std::wstring & text)
{
  if (!table)
    return;

  if (ksapi::ITableCellPtr cell = table->GetCell(row, column))
  {
    ksapi::ITextLinePtr txt = cell->GetText();

    if (txt)
      txt->SetStr(text);
  }
}


//------------------------------------------------------------------------------
/**
  Задать текст в ячейках допуска формы
*/
//---
void SetToleranceText(ksapi::IToleranceParamPtr & tolPar)
{
  if (tolPar)
  {
    // Получить интерфейс таблицы с текстом допуска формы
    if (ksapi::ITablePtr tolTable = tolPar->GetTable())
    {
      // Добавить 3 столбца (1 уже есть)
      tolTable->AddColumn(-1, true /*справа*/);
      tolTable->AddColumn(-1, true /*справа*/);
      tolTable->AddColumn(-1, true /*справа*/);

      // Записать текст в 1-ю ячейку
      PutTextIntoCell(tolTable, 0, 0, L"@22~");
      // Записать текст во 2-ю ячейку
      PutTextIntoCell(tolTable, 0, 1, L"@2~");
      // Записать текст в 3-ю ячейку
      PutTextIntoCell(tolTable, 0, 2, L"B");
      // Записать текст в 4-ю ячейку
      PutTextIntoCell(tolTable, 0, 3, L"@30~");
    }
  }
}


//------------------------------------------------------------------------------
/**
  Создание допуска формы
*/
//---
void CreateTolerance(ksapi::ITolerancePtr & tolerance)
{
  if (tolerance)
  {
    // Получить интерфейс ответвления
    if (ksapi::IBranchsPtr branchs = tolerance)
    {
      // Задать точку привязки
      branchs->SetX0(100);
      branchs->SetY0(150);
      // Добавить 2 ответвления
      branchs->AddBranchByPoint(-1, 100, 120);
      branchs->AddBranchByPoint(-1, 50, 155);
    }

    // Получить интерфейс параметров допуска формы
    if (ksapi::IToleranceParamPtr tolPar = tolerance)
    {
      // Создать текст в ячейках
      SetToleranceText(tolPar);
      // Положение базовой точки относительно таблицы - внизу посередине
      tolPar->SetBasePointPos(ksTPBottomCenter);
    }

    // Тип стрелки 1-го ответвления - треугольник
    tolerance->SetToleranceArrowType(0, ksToleranceArrowType::ksTATriangle);
    // Положение 1-го ответвления относительно таблицы - внизу посередине
    tolerance->SetBranchPos(0, ksTPBottomCenter);
    // Тип стрелки 2-го ответвления - стрелка
    tolerance->SetToleranceArrowType(1, ksToleranceArrowType::ksTAArrow);
    // Положение 2-го ответвления относительно таблицы - слева посередине
    tolerance->SetBranchPos(1, ksTPLeftCenter);
    // Применить параметры
    tolerance->Update();
  }
}


//-------------------------------------------------------------------------------
// Редактирование допуска формы
// ---
void EditTolerance(ksapi::ITolerancePtr & tolerance)
{
  if (tolerance)
  {
    // Получитьинтерфейс параметров допуска формы
    if (ksapi::IToleranceParamPtr tolPar = tolerance)
    {
      // Получить интерфейс таблицы с текстом допуска формы
      if (ksapi::ITablePtr tolTable = tolPar->GetTable())
      {
        // Изменить текст во 2-й ячейке
        PutTextIntoCell(tolTable, 0, 1, L"@2~15");
      }

      // Задать признак вертикальности
      tolPar->SetVertical(true);
    }

    // Получить интерфейс ответвлений
    if (ksapi::IBranchsPtr branchs = tolerance)
    {
      // Удалить ответвление
      branchs->DeleteBranch(0);
      // Добавить новое ответвление
      branchs->AddBranchByPoint(-1, 130, 120);
    }

    tolerance->SetToleranceArrowType(1, ksTATriangle);
    tolerance->SetBranchPos(1, ksTPBottomCenter);
    // Применить изменения
    tolerance->Update();
  }
}


//------------------------------------------------------------------------------
/**
  Создание и редактирование допуска формы
*/
//---
void ToleranceWork(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер условных обозначений
  if (ksapi::ISymbols2DContainerPtr symbCont = GetSymbols2DContainer(doc))
  {
    // Получить коллекцию допусков формы
    if (ksapi::ITolerancesPtr tolerancesCol = symbCont->GetTolerances())
    {
      // Добавить допуск формы
      if (ksapi::ITolerancePtr newTol = tolerancesCol->Add())
      {
        // Создать допуск формы
        CreateTolerance(newTol);

        // Подсветить созданный объект
        lib_func::LightObj(newTol, 1 /*включить подсветку*/);

        if (lib_func::YesNoMessage(LoadStr(IDS_EDIT)) == 1)
        {
          // Редактировать допуск формы
          EditTolerance(newTol);
        }
        lib_func::LightObj(newTol, false /*выключить подсветку*/);
      }
    }
  }
}


//------------------------------------------------------------------------------
/**
  Получить контейнер графических объектов
*/
//---
ksapi::IDrawingContainerPtr GetDrawingContainer(ksapi::IKompasDocument2DPtr & doc)
{
  ksapi::IDrawingContainerPtr res;

  if (doc)
  {
    // Получить менеджер видов и слоев
    if (ksapi::IViewsAndLayersManagerPtr mng = doc->GetViewsAndLayersManager())
    {
      // Получить коллекцию видов
      if (ksapi::IViewsPtr viewsCol = mng->GetViews())
      {
        // Получить активный вид
        if (ksapi::IViewPtr view = viewsCol->GetActiveView())
          // Получить контейнер графических объектов
          res = view;
      }
    }
  }

  return res;
}


//------------------------------------------------------------------------------
/**
  Выдать сообщение с параметрами простой линии выноски
*/
//---
void GetLeaderPar(ksapi::ILeaderPtr & leader)
{
  if (leader)
  {
    double x0, y0;

    // Получить координаты точки привязки

    if (ksapi::IBranchsPtr branchs = leader)
    {
      x0 = branchs->GetX0();
      y0 = branchs->GetY0();
    }

    std::wstring outMessage;
    // "Простая линия выноски"
    outMessage
      .append(LoadStr(IDS_LEADER))
      // "\nТочка привязки:\nX0 = %4.2f, Y0 = %4.2f"
      .append(std::vformat(std::wstring_view(LoadStr(IDS_POINT)), std::make_wformat_args(x0, y0)))
      // "\nПризнак обработки по контуру: "
      .append(LoadStr(IDS_ARROUND));


    if (leader->IsArround())
      // "включен"
      outMessage.append(LoadStr(IDS_ON));
    else
      // "отключен"
      outMessage.append(LoadStr(IDS_OFF));

    // "\nНачало ответвления: "
    outMessage.append(LoadStr(IDS_BRANCHBEGIN));

    if (leader->GetBranchBegin(0))
      // "от начала полки"
      outMessage.append(LoadStr(IDS_BEGINSHELF));
    else
      // "от конца полки"
      outMessage.append(LoadStr(IDS_ENDSHELF));

    // "\nПризнак параллельности ответвлений: "
    outMessage.append(LoadStr(IDS_PARALLEL));

    if (leader->IsParallelBranch())
      // "включен"
      outMessage.append(LoadStr(IDS_ON));
    else
      // "отключен"
      outMessage.append(LoadStr(IDS_OFF));

    // Выдать сообщение
    lib_func::LibMessage(outMessage, ksMessageWarning, ksButtonSetOk, true);
  }
}


//------------------------------------------------------------------------------
/**
  Выдать сообщение с параметрами знака маркировки
*/
//---
void GetMarkLeaderPar(ksapi::IMarkLeaderPtr & markLeader)
{
  if (markLeader)
  {
    double x0, y0;
    std::int32_t count;

    if (ksapi::IBranchsPtr branchs = markLeader)
    {
      // Получить координаты точки привязки
      x0 = branchs->GetX0();
      y0 = branchs->GetY0();
      // Получить количество ответвлений
      count = branchs->GetBranchCount();
    }

    std::wstring outMessage;
    outMessage
      // "Знак маркировки"
      .append(LoadStr(IDS_MARKLEADER))
      // "\nТочка привязки:\nX0 = %4.2f, Y0 = %4.2f"
      .append(std::vformat(std::wstring_view(LoadStr(IDS_POINT)), std::make_wformat_args(x0, y0)))
      // "\nКоличество ответвлений: %d"
      .append(std::vformat(std::wstring_view(LoadStr(IDS_BRANCHCOUNT)), std::make_wformat_args(count)));

    // Выдать сообщение
    lib_func::LibMessage(outMessage, ksMessageWarning, ksButtonSetOk, true);
  }
}


//------------------------------------------------------------------------------
/**
  Выдать сообщение с параметрами знака клеймения
*/
//---
void GetBrandLeaderPar(ksapi::IBrandLeaderPtr & brandLeader)
{
  if (brandLeader)
  {
    // Выдать сообщение
    lib_func::LibMessage(LoadStr(IDS_BRANDLEADER), ksMessageWarning, ksButtonSetOk, true);
  }
}


//------------------------------------------------------------------------------
/**
  Выдать сообщение с параметрами знака изменения
*/
//---
void GetChangeLeaderPar(ksapi::IChangeLeaderPtr & changeLeader)
{
  if (changeLeader)
  {
    // Выдать сообщение
    lib_func::LibMessage(LoadStr(IDS_CHANLEADER), ksMessageWarning, ksButtonSetOk, true);
  }
}


//------------------------------------------------------------------------------
/**
  Выдать сообщение с параметрами обозначения шероховатости
*/
//---
void GetRoughPar(ksapi::IRoughPtr & rough)
{
  if (rough)
  {
    // Выдать сообщение
    lib_func::LibMessage(LoadStr(IDS_ROUGH), ksMessageWarning, ksButtonSetOk, true);
  }
}


//------------------------------------------------------------------------------
/**
  Выдать сообщение с параметрами обозначения базы
*/
//---
void GetBasePar(ksapi::IBasePtr & base)
{
  if (base)
  {
    // Выдать сообщение
    lib_func::LibMessage(LoadStr(IDS_BASE), ksMessageWarning, ksButtonSetOk, true);
  }
}


//------------------------------------------------------------------------------
/**
  Выдать сообщение с параметрами линии разреза/сечения
*/
//---
void GetCutLinePar(ksapi::ICutLinePtr & cutLine)
{
  if (cutLine)
  {
    // Выдать сообщение
    lib_func::LibMessage(LoadStr(IDS_CUTLINE), ksMessageWarning, ksButtonSetOk, true);
  }
}


//------------------------------------------------------------------------------
/**
  Выдать сообщение с параметрами стрелки направления взгляда
*/
//---
void GetViewPointerPar(ksapi::IViewPointerPtr & viewPointer)
{
  if (viewPointer)
  {
    // Выдать сообщение
    lib_func::LibMessage(LoadStr(IDS_VIEWPOINTER), ksMessageWarning, ksButtonSetOk, true);
  }
}


//------------------------------------------------------------------------------
/**
  Выдать сообщение с параметрами допуска формы
*/
//---
void GetTolerancePar(ksapi::ITolerancePtr & tolerance)
{
  if (tolerance)
  {
    // Выдать сообщение
    lib_func::LibMessage(LoadStr(IDS_TOLERANCE), ksMessageWarning, ksButtonSetOk, true);
  }
}


//------------------------------------------------------------------------------
/**
  Навигация по массиву объектов вида
*/
//---
void ObjectsNavigation(ksapi::IKompasDocument2DPtr & doc)
{
  // Получить контейнер графических объектов
  ksapi::IDrawingContainerPtr drawCont(GetDrawingContainer(doc));

  if (drawCont)
  {
    // Тип 0 - все объекты
    std::vector<std::int32_t> types{0};
    // Получить массив SafeArray объектов
    std::vector<ksapi::IDrawingObjectPtr> drawObjs = drawCont->GetObjects(types);

    // Если массив есть и он не пуст
    if (!drawObjs.empty())
    {
      for (auto drawObj : drawObjs)
      {
        // Получить элемент из массива
        if (drawObj)
        {
          // Получить тип объекта
          DrawingObjectTypeEnum type = drawObj->GetDrawingObjectType();
          // Подсветить объект
          lib_func::LightObj(drawObj, true /*включить*/);

          // В зависимости от типа вывести сообщение для данного типа объектов
          switch (type)
          {
            // Простая линия выноски
            case ksDrLeader:
            {
              ksapi::ILeaderPtr leader(drawObj);
              GetLeaderPar(leader);
              break;
            }

            // Знак маркировки
            case ksDrMarkerLeader:
            {
              ksapi::IMarkLeaderPtr markLeader(drawObj);
              GetMarkLeaderPar(markLeader);
              break;
            }

            // Знак клеймения
            case ksDrBrandLeader:
            {
              ksapi::IBrandLeaderPtr brandLeader(drawObj);
              GetBrandLeaderPar(brandLeader);
              break;
            }

            // Знак изменения
            case ksDrChangeLeader:
            {
              ksapi::IChangeLeaderPtr changeLeader(drawObj);
              GetChangeLeaderPar(changeLeader);
              break;
            }

            // Обозначение шероховатости
            case ksDrRough:
            {
              ksapi::IRoughPtr rough(drawObj);
              GetRoughPar(rough);
              break;
            }

            // Обозначение базы
            case ksDrBase:
            {
              ksapi::IBasePtr base(drawObj);
              GetBasePar(base);
              break;
            }

            // Линия разреза/сечения
            case ksDrCut:
            {
              ksapi::ICutLinePtr cutLine(drawObj);
              GetCutLinePar(cutLine);
              break;
            }

            // Стрелка направления взгляда
            case ksDrWPointer:
            {
              ksapi::IViewPointerPtr viewPointer(drawObj);
              GetViewPointerPar(viewPointer);
              break;
            }

            // Допуск формы
            case ksDrTolerance:
            {
              ksapi::ITolerancePtr tolerance(drawObj);
              GetTolerancePar(tolerance);
              break;
            }
          }
          // Убрать подсветку
          lib_func::LightObj(drawObj, false /*выключить*/);
        }
      }
    }
  }
}

namespace lib_func
{
//------------------------------------------------------------------------------
/**
  Подсветить объект
*/
//---
void LightObj(ksapi::IKompasAPIObjectPtr lightObj, bool choose)
{
  if (ksapi::IKompasDocument2DPtr document = GetCurrentDocument())
  {
    if (ksapi::IChooseManagerPtr chooseMng = document->GetChooseManager())
    {
      if (choose)
        chooseMng->Choose({lightObj});
      else
        chooseMng->Unchoose({lightObj});
    }
  }
}


//------------------------------------------------------------------------------
/**
  Сообщение от библиотеки
*/
//---
std::int32_t LibMessage(const std::wstring & message, ksMessageTypeEnum dlgType, ksMessageButtonSetEnum dlgButtons,
                        bool showModal)
{
  std::int32_t res = 0;

  if (!message.empty() && ksAppl)
    res = ksAppl->ShowMessageBox(message, LoadStr(IDR_LIBID), dlgType, dlgButtons, showModal);

  return res;
}


/// Результат диалога
enum DialogResult
{
  Positive = 6,
  Negative = 7,
  Cancel = 2
};


//------------------------------------------------------------------------------
/**
  Диалог да/нет/отмена
*/
//---
std::int32_t YesNoMessage(const std::wstring & message)
{
  std::int32_t res = LibMessage(message, ksMessageWarning, ksButtonSetYesNoCancel, false);
  return std::int32_t(res == DialogResult::Positive);
}

ksapi::IProcessPtr CreateProcess(ksProcess2DTypeEnum procType)
{
  ksapi::IProcessPtr process;

  if (ksAppl)
  {
    ksapi::IKompasDocument2DPtr doc2D = ksAppl->GetActiveDocument();
    process = doc2D->GetLibProcess(procType);
    process->Run(false, false);
  }

  return process;
}

} // namespace lib_func

extern void * GetModule();

//------------------------------------------------------------------------------
/**
  Загрузить строку из ресурса
*/
//---
std::wstring LoadStr(std::int32_t strID)
{
  std::wstring resStr;

  if (ksAppl)
  {
    if (ksapi::ILibraryManagerPtr libMng = ksAppl->GetLibraryManager())
    {
      if (ksapi::IProceduresLibrariesPtr procLibs = libMng->GetProceduresLibraries())
      {
        if (ksapi::IProceduresLibraryPtr procLib = procLibs->GetItemByName(L"Step3_KsAPI_2D"))
        {
          if (ksapi::IKompasDictionaryPtr dictionary = procLib->GetDictionary())
            return dictionary->LoadAndConvertLangString(GetModule(), strID);
        }
      }
    }
  }

  return resStr;
}
