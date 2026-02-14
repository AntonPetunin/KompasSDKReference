#pragma once
#include "BaseEvent.h"


/////////////////////////////////////////////////////////////////////////////
//
// Базовый класс для обработчиков событий
//
/////////////////////////////////////////////////////////////////////////////
class ApplicationEvent final : public BaseEvent
{
public:
  ApplicationEvent();
  virtual ~ApplicationEvent();

  virtual bool Advise() override;   // подписаться на получение событий
  virtual void UnAdvise() override; // отписаться от получения событий

  virtual std::wstring EventCaption() override { return L"ApplicationEvent"; }
  /// Вывести сообщение от обработчика с проверкой
  void ShowEventName(const std::wstring & eventName, ksapi::IKompasDocumentPtr document = nullptr,
                     const std::wstring & args = L"");

#pragma region Обработчики событий
  bool OnBeginCreate(DocumentTypeEnum docType);
  void OnCreateDocument(ksapi::IKompasDocumentPtr document);
  bool OnBeginOpenDocument(const std::wstring & fileName);
  void OnOpenDocument(ksapi::IKompasDocumentPtr document);
  bool OnBeginOpenFile();
  bool OnBeginDragOpenFiles(std::vector<std::wstring> & fileNames, bool insert);
  bool OnBeginRequestFiles(ksRequestFilesTypeEnum requestId, std::vector<std::wstring> & fileNames);
  void OnChangeActiveDocument(ksapi::IKompasDocumentPtr document);
  void OnApplicationDestroy();
  bool OnBeginCloseAllDocument();
  bool OnKeyDown(int32_t & keyCode, int32_t keyFlags, const ksapi::IPressedKeysPtr & systemKeys);
  bool OnKeyUp(int32_t & keyCode, int32_t keyFlags, const ksapi::IPressedKeysPtr & systemKeys);
  bool OnKeyPressed(int32_t & keyCode, int32_t keyFlags, const ksapi::IPressedKeysPtr & systemKeys);
  bool OnBeginChoiceMaterial(int32_t materialId);
  void OnChoiceMaterial(int32_t materialId, const std::wstring & materialName, double density);
  bool OnIsNeedConvertToSavePrevious(ksapi::IKompasDocumentPtr document, ksSaveDocumentVersionEnum version,
                                     ksapi::ISaveToPreviousParamPtr saveParam, bool & needConvert);
  void OnBeginConvertToSavePrevious(ksapi::IKompasDocumentPtr document, ksSaveDocumentVersionEnum version,
                                    ksapi::ISaveToPreviousParamPtr saveParam);
  void OnEndConvertToSavePrevious(ksapi::IKompasDocumentPtr document, ksSaveDocumentVersionEnum version,
                                  ksapi::ISaveToPreviousParamPtr saveParam);
  bool OnChangeTheme(ksThemeEnum theme);
  bool OnBeginChoiceMarking(const std::wstring & oldMarking, std::wstring & newMarking);
#pragma endregion
};
