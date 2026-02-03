#include "Step3_KsAPI_3D.h"
#include <MessageUtils.h>
#include <CursorProcess.h>

ksapi::IApplication * kompasApp = nullptr;

int oType = o3d_edge; ///< Тип объектов для функции фильтрации

// Описания используемых функций.
void Rough3DWork(ksapi::IKompasDocument3DPtr & doc);
void Base3DWork(ksapi::IKompasDocument3DPtr & doc);
void Leader3DWork(ksapi::IKompasDocument3DPtr & doc);
void BrandLeader3DWork(ksapi::IKompasDocument3DPtr & doc);
void MarkLeader3DWork(ksapi::IKompasDocument3DPtr & doc);
void Tolerance3DWork(ksapi::IKompasDocument3DPtr & doc);

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
  Головная функция библиотеки.
*/
//---
void RunCommand(unsigned int commandId, ksapi::ksRunCommandModeEnum mode)
{
  ksapi::IKompasDocument3DPtr doc = kompasApp->GetActiveDocument();

  if (doc)
  {
    switch (commandId)
    {
      case 1:
        Rough3DWork(doc);
        break; // Создание и редактирование шероховатости 3D
      case 2:
        Base3DWork(doc);
        break; // Создание и редактирование базы 3D
      case 3:
        Leader3DWork(doc);
        break; // Создание и редактирование линии-выноски 3D
      case 4:
        BrandLeader3DWork(doc);
        break; // Создание и редактирование знака клеймения 3D
      case 5:
        MarkLeader3DWork(doc);
        break; // Создание и редактирование знака маркировки 3D
      case 6:
        Tolerance3DWork(doc);
        break; // Создание и редактирование допуска формы 3D
    }
  }
  else
  {
    LibMessage(LoadStr(IDS_NODOC), ksMessageError);
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

  kompasApp = &app;
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

//------------------------------------------------------------------------------
/**
  Получить контейнер обозначений 3D.
*/
//---
ksapi::ISymbols3DContainerPtr GetSymbols3DContainer(ksapi::IKompasDocument3DPtr & doc)
{
  if (doc)
    return doc->GetTopPart();

  return nullptr;
}

//------------------------------------------------------------------------------
/**
  Функция фильтрации.
*/
//---
bool __stdcall UserFilterProc(ksapi::IModelObjectPtr e)
{
  if (e && (!oType || e->GetType() == oType))
    return true;
  else
    return false;
}

//------------------------------------------------------------------------------
/**
  Установить параметры шероховатости.
*/
//---
void SetRoughPars(ksapi::IRough3DPtr & rough)
{
  if (rough)
  {
    if (ksapi::IRoughParamsPtr roughParams = rough)
    {
      roughParams->SetProcessingByContour(true);
      roughParams->SetLeaderLength(20);
      roughParams->SetLeaderAngle(45);

      if (ksapi::ITextPtr txt1 = roughParams->GetRoughParamText())
        txt1->SetStr(L"1");

      if (ksapi::ITextPtr txt2 = roughParams->GetProcessText())
        txt2->SetStr(L"2");

      if (ksapi::ITextPtr txt3 = roughParams->GetBaseLengthText())
        txt3->SetStr(L"3");
    }
  }
}

//------------------------------------------------------------------------------
/**
  Создать шероховатость 3D.
*/
//---
bool CreateRough3D(ksapi::IRough3DPtr & rough)
{
  bool res = false;

  if (rough)
  {
    ksapi::IModelObjectPtr baseObj;

    auto && getBaseObj = [&](const ksapi::IKompasAPIObjectPtr & object)
    {
      baseObj = object;
      return false;
    };

    CursorProcess cursorProcess;
    if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
      process.Get()->SetCaption(LoadStr(IDS_OBJ));

    cursorProcess.Execute(getBaseObj);

    if (baseObj)
    {
      double x, y, z;

      ksapi::IProcessPtr process;

      if (process = cursorProcess.GetProcess())
        process.Get()->SetCaption(LoadStr(IDS_POINT));

      auto && getPlacement = [&](const ksapi::IKompasAPIObjectPtr & object)
      {
        ksapi::IProcess3DPtr proc3D = process;
        ksapi::IPlacement3DPtr placement = proc3D->GetPlacement();
        placement->GetOrigin(x, y, z);

        return false;
      };

      cursorProcess.Execute(getPlacement);

      rough->SetBasePosition(x, y, z, baseObj);
      rough->SetBasePlane(o3d_planeXOZ);

      SetRoughPars(rough);

      rough->Update();

      res = true;
    }
  }

  return res;
}

//------------------------------------------------------------------------------
/**
  Задать плоскость обозначения по вершине базового объекта.
*/
//---
void SetPosition(ksapi::IRough3DPtr & rough, ksapi::IModelObjectPtr & mObj)
{
  if (rough && mObj)
  {
    if (ksapi::IFacePtr face = mObj)
    {
      std::vector<ksapi::IEdgePtr> edgeCol = face->GetLimitingEdges();

      if (!edgeCol.empty())
        if (ksapi::IEdgePtr edge = edgeCol[0])
          if (ksapi::IModelObjectPtr vertex = edge->GetVertex(true))
            rough->SetPositionObject(vertex);
    }
  }
}

//------------------------------------------------------------------------------
/**
  Получить новое имя объекта.
*/
//---
std::wstring GetNewName(ksapi::IFeaturePtr & feature, std::int32_t strID)
{
  std::wstring res;

  if (feature)
  {
    std::wstring newName = LoadStr(strID);
    std::wstring name = feature->GetName();

    auto pos = name.find(L':');

    if (pos != std::wstring::npos)
      newName += name.substr(pos + 1);

    res = newName;
  }

  return res;
}

//------------------------------------------------------------------------------
/**
  Редактировать шероховатость 3D.
*/
//---
void EditRough3D(ksapi::IRough3DPtr & rough)
{
  if (rough)
  {
    if (ksapi::IRoughParamsPtr roughPars = rough)
      roughPars->SetProcessingByContour(false);

    if (ksapi::IFeaturePtr featObj = rough)
    {
      rough->SetName(GetNewName(featObj, IDS_ROUGH));

      ksapi::IModelObjectPtr baseObj = rough->GetBaseObject();
      SetPosition(rough, baseObj);
    }

    if (ksapi::IColorParamPtr colorPars = rough)
      colorPars->SetColor(0xFF0000); // Изменить цвет на синий.

    rough->Update();
  }
}

//------------------------------------------------------------------------------
/**
  Создание и редактирование шероховатости 3D.
*/
//---
void Rough3DWork(ksapi::IKompasDocument3DPtr & doc)
{
  if (ksapi::ISymbols3DContainerPtr symbCont = GetSymbols3DContainer(doc))
  {
    if (ksapi::IRoughs3DPtr roughs = symbCont->GetRoughs3D())
    {
      ksapi::IRough3DPtr rough = roughs->Add();

      if (rough)
      {
        if (CreateRough3D(rough))
        {
          ksapi::IFeaturePtr feature = rough;

          std::wstring name = feature ? feature->GetName() : L"";

          if (YesNoMessage(LoadStr(IDS_EDIT)) == 1)
          {
            ksapi::IRough3DPtr rough = roughs->GetRough3DByName(name);
            EditRough3D(rough);
          }
        }
        else
        {
          LibMessage(LoadStr(IDS_NOCREATE), ksMessageError);
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
/**
  Создать обозначение базы.
*/
//---
bool CreateBase3D(ksapi::IBase3DPtr & base)
{
  bool res = false;

  if (base)
  {
    ksapi::IModelObjectPtr baseObj;

    auto && getBaseObj = [&](const ksapi::IKompasAPIObjectPtr & object)
    {
      baseObj = object;
      return false;
    };

    CursorProcess cursorProcess;

    if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
      process.Get()->SetCaption(LoadStr(IDS_OBJ));

    cursorProcess.Execute(getBaseObj);

    if (baseObj)
    {
      double x, y, z;

      ksapi::IProcessPtr process;

      if (process = cursorProcess.GetProcess())
        process.Get()->SetCaption(LoadStr(IDS_POINT));

      auto && getPlacement = [&](const ksapi::IKompasAPIObjectPtr & object)
      {
        ksapi::IProcess3DPtr proc3D = process;
        ksapi::IPlacement3DPtr placement = proc3D->GetPlacement();
        placement->GetOrigin(x, y, z);

        return false;
      };

      cursorProcess.Execute(getPlacement);

      base->SetBranchBeginPoint(x, y, z, baseObj);
      base->SetBasePlane(o3d_planeXOZ);
      base->Update();

      res = true;
    }
  }

  return res;
}

//------------------------------------------------------------------------------
/**
  Задать компоненту текста.
*/
//---
void AddTextItem(ksapi::ITextLine & line, const std::wstring & str, ksTextItemEnum textItem)
{
  if (ksapi::ITextItemPtr item = line.Add())
  {
    item->SetStr(str);
    item->SetItemType(textItem);
    item->Update();
  }
}

//------------------------------------------------------------------------------
/**
  Редактирование обозначения базы.
*/
//---
void EditBase3D(ksapi::IBase3DPtr & base)
{
  if (base)
  {
    base->SetAutoSorted(false);

    if (ksapi::ITextPtr txt = base->GetText())
    {
      txt->SetStr(L"");

      ksapi::ITextLinePtr line(txt->Add());

      if (line)
      {
        AddTextItem(*line, L"A", ksTItString);
        AddTextItem(*line, L"", ksTItSBase);
        AddTextItem(*line, L"1", ksTItSLowerIndex);
        AddTextItem(*line, L"", ksTItSEnd);
      }
    }

    base->SetDrawType(false);
    double x, y, z;

    base->GetBranchEndPoint(x, y, z);
    base->SetBranchEndPoint(x + 10, y + 10, z);

    if (ksapi::IFeaturePtr featObj = base)
      base->SetName(GetNewName(featObj, IDS_BASE));

    if (ksapi::IColorParamPtr colorPars = base)
      colorPars->SetColor(0x000000); // Изменить цвет на черный.

    base->Update();
  }
}

//------------------------------------------------------------------------------
/**
  Создание и редактирование обозначения базы.
*/
//---
void Base3DWork(ksapi::IKompasDocument3DPtr & doc)
{
  ksapi::ISymbols3DContainerPtr symbCont = GetSymbols3DContainer(doc);

  if (symbCont)
  {
    if (ksapi::IBases3DPtr bases = symbCont->GetBases3D())
    {
      ksapi::IBase3DPtr base = bases->Add();

      if (base)
      {
        if (CreateBase3D(base))
        {
          ksapi::IFeaturePtr feature = base;

          std::wstring name = feature ? feature->GetName() : L"";

          if (YesNoMessage(LoadStr(IDS_EDIT)) == 1)
          {
            ksapi::IBase3DPtr base = bases->GetBase3DByName(name);

            EditBase3D(base);
          }
        }
        else
        {
          LibMessage(LoadStr(IDS_NOCREATE));
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
/**
  Установить тексты линии-выноски.
*/
//---
void SetLeader3DTexts(ksapi::IBaseLeader3DPtr & baseLeader)
{
  if (ksapi::ILeaderPtr leader = baseLeader)
  {
    if (ksapi::ITextPtr txtOnSh = leader->GetTextOnShelf())
      txtOnSh->SetStr(L"1");

    if (ksapi::ITextPtr txtUnderSh = leader->GetTextUnderShelf())
      txtUnderSh->SetStr(L"2");

    if (ksapi::ITextPtr txtOnBr = leader->GetTextOnBranch())
      txtOnBr->SetStr(L"3");

    if (ksapi::ITextPtr txtUnderBr = leader->GetTextUnderBranch())
      txtUnderBr->SetStr(L"4");

    if (ksapi::ITextPtr txtAfterSh = leader->GetTextAfterShelf())
      txtAfterSh->SetStr(L"5");
  }
}

//------------------------------------------------------------------------------
/**
  Создать ответвление линии-выноски.
*/
//---
void CreateBranchsLeader(ksapi::IBaseLeader3DPtr & leader)
{
  if (leader)
  {
    ksapi::IModelObjectPtr baseObj;

    auto && getBaseObj = [&](const ksapi::IKompasAPIObjectPtr & object)
    {
      baseObj = object;
      return false;
    };

    CursorProcess cursorProcess;

    if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
      process.Get()->SetCaption(LoadStr(IDS_OBJ));

    cursorProcess.Execute(getBaseObj);

    if (baseObj)
    {
      double x, y, z, x1, y1, z1;

      ksapi::IProcessPtr process;

      if (process = cursorProcess.GetProcess())
        process.Get()->SetCaption(LoadStr(IDS_BEGINBRANCH));

      // Указать точку начала ножки.
      // Точка должна лежать на опорном объекте
      auto && getBeginBranch = [&](const ksapi::IKompasAPIObjectPtr & object)
      {
        ksapi::IProcess3DPtr proc3D = process;
        ksapi::IPlacement3DPtr placement = proc3D->GetPlacement();
        placement->GetOrigin(x, y, z);

        return false;
      };

      cursorProcess.Execute(getBeginBranch);

      ksapi::IModelObjectPtr posObj;

      auto && getPosObj = [&](const ksapi::IKompasAPIObjectPtr & object)
      {
        posObj = object;
        return false;
      };

      if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
        process.Get()->SetCaption(LoadStr(IDS_POSOBJ));

      cursorProcess.Execute(getPosObj);

      if (posObj)
      {
        if (process = cursorProcess.GetProcess())
          process.Get()->SetCaption(LoadStr(IDS_BEGINSHELF));

        // Указать точку, задающую положние объектов.
        // Точка должна лежать в плоскости обозначения.
        auto && getBeginShelf = [&](const ksapi::IKompasAPIObjectPtr & object)
        {
          ksapi::IProcess3DPtr proc3D = process;
          ksapi::IPlacement3DPtr placement = proc3D->GetPlacement();
          placement->GetOrigin(x1, y1, z1);

          return false;
        };

        cursorProcess.Execute(getBeginShelf);

        if (ksapi::IBranchs3DPtr branchs3D = leader)
          branchs3D->AddBranchByPoint(x, y, z, baseObj);

        leader->SetPositionObject(posObj);
        leader->SetPosition(x1, y1, z1);
      }
    }
  }
}

//------------------------------------------------------------------------------
/**
  Создать линию-выноску.
*/
//---
bool CreateLeader3D(ksapi::IBaseLeader3DPtr & leader)
{
  bool res = false;

  if (leader)
  {
    CreateBranchsLeader(leader);
    SetLeader3DTexts(leader);

    leader->Update();

    res = true;
  }

  return res;
}

//------------------------------------------------------------------------------
/**
  Изменить параметры линии-выноски.
*/
//---
void EditLeaderPars(ksapi::IBaseLeader3DPtr & baseLeader)
{
  if (baseLeader)
  {
    if (ksapi::ILeaderPtr leader = baseLeader)
    {
      leader->SetSignType(ksLGlueSign);
      leader->SetArround(true);

      leader->SetBranchBegin(2, false);

      if (ksapi::ITextPtr txt = leader->GetTextOnShelf())
      {
        txt->SetStr(L"");

        if (ksapi::ITextLinePtr line = txt->Add())
        {
          AddTextItem(*line, L"1", ksTItString);
          AddTextItem(*line, L"1", ksTItNumerator);
          AddTextItem(*line, L"1", ksTItDenominator);
          AddTextItem(*line, L"1", ksTItFractionEnd);
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
/**
  Добавить прямолинейное ответвление.
*/
//---
void LeaderAddBranch(ksapi::IBaseLeader3DPtr & leader)
{
  if (leader)
  {
    ksapi::IModelObjectPtr baseObj;

    auto && getBaseObj = [&](const ksapi::IKompasAPIObjectPtr & object)
    {
      baseObj = object;
      return false;
    };

    CursorProcess cursorProcess;

    if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
      process.Get()->SetCaption(LoadStr(IDS_OBJ));

    cursorProcess.Execute(getBaseObj);

    if (baseObj)
    {
      double x, y, z;

      ksapi::IProcessPtr process;

      if (process = cursorProcess.GetProcess())
        process.Get()->SetCaption(LoadStr(IDS_BEGINBRANCH));

      auto && getPlacement = [&](const ksapi::IKompasAPIObjectPtr & object)
      {
        ksapi::IProcess3DPtr proc3D = process;
        ksapi::IPlacement3DPtr placement = proc3D->GetPlacement();
        placement->GetOrigin(x, y, z);

        return false;
      };

      cursorProcess.Execute(getPlacement);

      if (ksapi::IBranchs3DPtr branchs3D = leader)
        branchs3D->AddBranchByPoint(x, y, z, baseObj);
    }
  }
}

//------------------------------------------------------------------------------
/**
  Редактировать линию-выноску.
*/
//---
void EditLeader3D(ksapi::IBaseLeader3DPtr & leader)
{
  if (leader)
  {
    LeaderAddBranch(leader);
    EditLeaderPars(leader);

    if (ksapi::IFeaturePtr featObj = leader)
      leader->SetName(GetNewName(featObj, IDS_LEADER));

    if (ksapi::IColorParamPtr colorPars = leader)
      colorPars->SetColor(0x00287B); // Изменить цвет на коричневый.

    leader->Update();
  }
}

//------------------------------------------------------------------------------
/**
  Создание и редактирование линии-выноски 3D.
*/
//---
void Leader3DWork(ksapi::IKompasDocument3DPtr & doc)
{
  if (ksapi::ISymbols3DContainerPtr symbCont = GetSymbols3DContainer(doc))
  {
    if (ksapi::ILeaders3DPtr leaders = symbCont->GetLeaders3D())
    {
      ksapi::IBaseLeader3DPtr leader = leaders->Add(o3d_leader3D);

      if (leader)
      {
        if (CreateLeader3D(leader))
        {
          ksapi::IFeaturePtr feature = leader;
          std::wstring name = feature ? feature->GetName() : L"";

          if (YesNoMessage(LoadStr(IDS_EDIT)) == 1)
          {
            ksapi::IBaseLeader3DPtr leader = leaders->GetLeader3DByName(name);
            EditLeader3D(leader);
          }
        }
        else
        {
          LibMessage(LoadStr(IDS_NOCREATE));
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
/**
  Создать знак клеймения 3D.
*/
//---
bool CreateBrandLeader3D(ksapi::IBaseLeader3DPtr & baseLeader)
{
  bool res = false;

  if (baseLeader)
  {
    CreateBranchsLeader(baseLeader);

    if (ksapi::IBrandLeaderPtr brandLeader = baseLeader)
      brandLeader->SetDirection(false);

    baseLeader->Update();

    res = true;
  }

  return res;
}

//------------------------------------------------------------------------------
/**
  Редактировать знак клеймения.
*/
//---
void EditBrandLeader3D(ksapi::IBaseLeader3DPtr & leader)
{
  if (leader)
  {
    LeaderAddBranch(leader);

    if (ksapi::IBrandLeaderPtr brandLeader = leader)
    {
      ksapi::ITextPtr des = brandLeader->GetDesignation();

      if (des)
        des->SetStr(LoadStr(IDS_MARK2));
    }

    if (ksapi::IFeaturePtr feature = leader)
      leader->SetName(GetNewName(feature, IDS_BRAND));

    if (ksapi::IColorParamPtr colorPars = leader)
      colorPars->SetColor(0x800080); // Изменить цвет на фиолетовый.

    leader->Update();
  }
}

//------------------------------------------------------------------------------
/**
  Создание и редактирование знака клеймения.
*/
//---
void BrandLeader3DWork(ksapi::IKompasDocument3DPtr & doc)
{
  if (ksapi::ISymbols3DContainerPtr symbCont = GetSymbols3DContainer(doc))
  {
    if (ksapi::ILeaders3DPtr leaders = symbCont->GetLeaders3D())
    {
      ksapi::IBaseLeader3DPtr brandLeader = leaders->Add(o3d_brandLeader3D);

      if (brandLeader)
      {
        if (CreateBrandLeader3D(brandLeader))
        {
          ksapi::IFeaturePtr feature = brandLeader;

          std::wstring name = feature ? feature->GetName() : L"";

          if (YesNoMessage(LoadStr(IDS_EDIT)) == 1)
          {
            ksapi::IBaseLeader3DPtr brandLeader = leaders->GetLeader3DByName(name);
            EditBrandLeader3D(brandLeader);
          }
        }
        else
        {
          LibMessage(LoadStr(IDS_NOCREATE));
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
/**
  Создать знак маркировки.
*/
//---
bool CreateMarkLeader3D(ksapi::IBaseLeader3DPtr & baseLeader)
{
  bool res = false;

  if (baseLeader)
  {
    CreateBranchsLeader(baseLeader);

    if (ksapi::IMarkLeaderPtr markLeader = baseLeader)
    {
      if (ksapi::ITextPtr textOnBranch = markLeader->GetTextOnBranch())
        textOnBranch->SetStr(L"2");

      if (ksapi::ITextPtr textUnderBranch = markLeader->GetTextUnderBranch())
        textUnderBranch->SetStr(L"3");
    }

    baseLeader->Update();

    res = true;
  }

  return res;
}

//------------------------------------------------------------------------------
/**
  Редактировать знак маркировки.
*/
//---
void EditMarkLeader3D(ksapi::IBaseLeader3DPtr & baseLeader)
{
  if (baseLeader)
  {
    LeaderAddBranch(baseLeader);

    if (ksapi::IMarkLeaderPtr markLeader = baseLeader)
      if (ksapi::ITextPtr des = markLeader->GetDesignation())
        des->SetStr(L"10");

    if (ksapi::IFeaturePtr featObj = baseLeader)
      baseLeader->SetName(GetNewName(featObj, IDS_MARK));

    if (ksapi::IColorParamPtr colorPars = baseLeader)
      colorPars->SetColor(0xFF99CC); // Изменить цвет на сиреневый.

    baseLeader->Update();
  }
}

//------------------------------------------------------------------------------
/**
  Создание и редактирование знака маркировки 3D.
*/
//---
void MarkLeader3DWork(ksapi::IKompasDocument3DPtr & doc)
{
  ksapi::ISymbols3DContainerPtr symbCont = GetSymbols3DContainer(doc);

  if (symbCont)
  {
    if (ksapi::ILeaders3DPtr leaders = symbCont->GetLeaders3D())
    {
      ksapi::IBaseLeader3DPtr markLeader = leaders->Add(o3d_markLeader3D);

      if (markLeader)
      {
        if (CreateMarkLeader3D(markLeader))
        {
          ksapi::IFeaturePtr feature = markLeader;
          std::wstring name = feature ? feature->GetName() : L"";

          if (YesNoMessage(LoadStr(IDS_EDIT)) == 1)
          {
            ksapi::IBaseLeader3DPtr markLeader = leaders->GetLeader3DByName(name);
            EditMarkLeader3D(markLeader);
          }
        }
        else
        {
          LibMessage(LoadStr(IDS_NOCREATE));
        }
      }
    }
  }
}

//------------------------------------------------------------------------------
/**
  Создать ответвление для доопуска формы.
*/
//---
void CreateToleranceBranch(ksapi::ITolerance3DPtr & tolerance)
{
  if (tolerance)
  {
    ksapi::IModelObjectPtr baseObj;

    auto && getBaseObj = [&](const ksapi::IKompasAPIObjectPtr & object)
    {
      baseObj = object;
      return false;
    };

    CursorProcess cursorProcess;

    if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
      process.Get()->SetCaption(LoadStr(IDS_OBJ));

    cursorProcess.Execute(getBaseObj);

    if (baseObj)
    {
      double x, y, z, x1, y1, z1;

      ksapi::IProcessPtr process;

      if (process = cursorProcess.GetProcess())
        process.Get()->SetCaption(LoadStr(IDS_ENDPOINT));

      // Указать точку конца ножки.
      // Точка должна лежать на опорном объекте.
      auto && getEndPoint = [&](const ksapi::IKompasAPIObjectPtr & object)
      {
        ksapi::IProcess3DPtr proc3D = process;
        ksapi::IPlacement3DPtr placement = proc3D->GetPlacement();
        placement->GetOrigin(x, y, z);

        return false;
      };

      cursorProcess.Execute(getEndPoint);

      // Объект, задающий плоскость обозначения.
      ksapi::IModelObjectPtr posObj;

      auto && getPosObj = [&](const ksapi::IKompasAPIObjectPtr & object)
      {
        posObj = object;
        return false;
      };

      if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
        process.Get()->SetCaption(LoadStr(IDS_POSOBJ));

      cursorProcess.Execute(getPosObj);

      if (posObj)
      {
        if (process = cursorProcess.GetProcess())
          process.Get()->SetCaption(LoadStr(IDS_TABLE));

        // Указать точку вставки таблицы.
        auto && getTablePoint = [&](const ksapi::IKompasAPIObjectPtr & object)
        {
          ksapi::IProcess3DPtr proc3D = process;
          ksapi::IPlacement3DPtr placement = proc3D->GetPlacement();
          placement->GetOrigin(x1, y1, z1);

          return false;
        };

        cursorProcess.Execute(getTablePoint);


        if (ksapi::IBranchs3DPtr branchs3D = tolerance)
          branchs3D->AddBranchByPoint(x, y, z, baseObj);

        tolerance->SetPositionObject(posObj);
        tolerance->SetPosition(x1, y1, z1);
      }
    }
  }
}

//------------------------------------------------------------------------------
/**
  Задать текст в ячейках допуска формы.
*/
//---
void SetToleranceText(ksapi::IToleranceParamPtr & toleranceParam)
{
  if (toleranceParam)
  {
    ksapi::ITablePtr table = toleranceParam->GetTable();

    if (table)
    {
      // Добавить 3 столбца (1 уже есть)
      table->AddColumn(-1, true);
      table->AddColumn(-1, true);
      table->AddColumn(-1, true);

      ksapi::ITableCellPtr cell(table->GetCell(0, 0));
      ksapi::ITextLinePtr txt;

      if (cell)
      {
        txt = cell->GetText();

        if (txt)
          txt->SetStr(L"@22~");
      }

      cell = table->GetCell(0, 1);

      if (cell)
      {
        txt = cell->GetText();

        if (txt)
          txt->SetStr(L"@2~");
      }

      cell = table->GetCell(0, 2);

      if (cell)
      {
        txt = cell->GetText();

        if (txt)
          txt->SetStr(L"B");
      }

      cell = table->GetCell(0, 3);

      if (cell)
      {
        txt = cell->GetText();

        if (txt)
          txt->SetStr(L"@30~");
      }
    }
  }
}

//------------------------------------------------------------------------------
/**
  Создать допуск формы.
*/
//---
bool CreateTolerance3D(ksapi::ITolerance3DPtr & tolerance)
{
  bool res = false;

  if (tolerance)
  {
    CreateToleranceBranch(tolerance);

    if (ksapi::IToleranceParamPtr toleranceParam = tolerance)
    {
      SetToleranceText(toleranceParam);
      toleranceParam->SetBasePointPos(ksTPBottomCenter);
    }

    tolerance->SetArrowType(0, false);
    tolerance->Update();

    res = true;
  }

  return res;
}

//------------------------------------------------------------------------------
/**
  Добавить ответвление для допуска формы.
*/
//---
void ToleranceAddBranch(ksapi::ITolerance3DPtr & tolerance)
{
  if (tolerance)
  {
    ksapi::IModelObjectPtr baseObj;

    auto && getBaseObj = [&](const ksapi::IKompasAPIObjectPtr & object)
    {
      baseObj = object;
      return false;
    };

    CursorProcess cursorProcess;

    if (ksapi::IProcessPtr process = cursorProcess.GetProcess())
      process.Get()->SetCaption(LoadStr(IDS_OBJ));

    cursorProcess.Execute(getBaseObj);

    if (baseObj)
    {
      double x, y, z;

      ksapi::IProcessPtr process;

      if (process = cursorProcess.GetProcess())
        process.Get()->SetCaption(LoadStr(IDS_ENDPOINT));

      auto && getPlacement = [&](const ksapi::IKompasAPIObjectPtr & object)
      {
        ksapi::IProcess3DPtr proc3D = process;
        ksapi::IPlacement3DPtr placement = proc3D->GetPlacement();
        placement->GetOrigin(x, y, z);

        return false;
      };

      cursorProcess.Execute(getPlacement);

      if (ksapi::IBranchs3DPtr branchs3D = tolerance)
        branchs3D->AddBranchByPoint(x, y, z, baseObj);

      tolerance->SetArrowType(1, true);
    }
  }
}

//------------------------------------------------------------------------------
/**
  Редактировать допуск формы.
*/
//---
void EditTolerance3D(ksapi::ITolerance3DPtr & tolerance)
{
  if (tolerance)
  {
    ToleranceAddBranch(tolerance);

    if (ksapi::IToleranceParamPtr toleranceParam = tolerance)
    {
      toleranceParam->SetVertical(true);
      toleranceParam->SetBasePointPos(ksTPLeftBottom);
    }

    if (ksapi::IFeaturePtr feature = tolerance)
      tolerance->SetName(GetNewName(feature, IDS_TOLERANCE));

    if (ksapi::IColorParamPtr colorPars = tolerance)
      colorPars->SetColor(0x220CBA); // Изменить цвет на темно-красный.

    tolerance->Update();
  }
}

//------------------------------------------------------------------------------
/**
  Создание и редактирование допуска формы 3D.
*/
//---
void Tolerance3DWork(ksapi::IKompasDocument3DPtr & doc)
{
  ksapi::ISymbols3DContainerPtr symbCont = GetSymbols3DContainer(doc);

  if (symbCont)
  {
    ksapi::ITolerances3DPtr tolerances3D = symbCont->GetTolerances3D();

    if (tolerances3D)
    {
      ksapi::ITolerance3DPtr newTolerance = tolerances3D->Add();

      if (newTolerance)
      {
        if (CreateTolerance3D(newTolerance))
        {
          ksapi::IFeaturePtr feature = newTolerance;
          std::wstring name = feature ? feature->GetName() : L"";

          if (YesNoMessage(LoadStr(IDS_EDIT)) == 1)
          {
            ksapi::ITolerance3DPtr tolerance = tolerances3D->GetTolerance3DByName(name);
            EditTolerance3D(tolerance);
          }
        }
        else
        {
          LibMessage(LoadStr(IDS_NOCREATE));
        }
      }
    }
  }
}
