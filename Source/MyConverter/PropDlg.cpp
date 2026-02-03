////////////////////////////////////////////////////////////////////////////////
//
/**
  \file 
  \brief Реализация получения параметров от пользователя.
*/
//
////////////////////////////////////////////////////////////////////////////////
#include <MyConverter_pch.h>
#include <MyConverter.h>

extern ksapi::IApplication * kompasApp; ///< Интерфейс приложения KsAPI.

//------------------------------------------------------------------------------
/**
  Получить признак сохранения стиля линий от пользовтеля.
*/
//---
bool ShowPropDlg()
{
  ksMessageButtonEnum btn = kompasApp->ShowMessageDialog(nullptr /*windowHandle*/, LoadResourceString(IDS_STYLE_DOC),
                                                         LoadResourceString(IDS_DLG_CAPTION), L"" /*explanation*/,
                                                         ksMessageQuestion, ksButtonSetYesNo, true /*showModal*/,
                                                         LoadResourceString(IDS_DLG_BTN_YES), LoadResourceString(IDS_DLG_BTN_NO),
                                                         L"" /*cancelButton*/, 0 /*helpId*/, L"" /*helpFileName*/);
  return btn == ksButtonYes;
}
