#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "netapi32.lib")

#include <windows.h>
#include <node.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <lm.h>


namespace NodePrj {

  using v8::Exception;
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Number;
  using v8::Object;
  using v8::String;
  using v8::Value;

  //функция преобразования типов wstring->string
  std::string ws2s(const std::wstring& s)
    {
        int len;
        int slength = (int)s.length() + 1;
        len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
        char* buf = new char[len];
        WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
        std::string r(buf);
        delete[] buf;
        return r;
    }

  void Check(const FunctionCallbackInfo<Value>& args) {

    Isolate* isolate = args.GetIsolate();

    std::string names = ""; //строка всех имен пользователей
    //переменные для функции получения списка пользователей
    LPUSER_INFO_0 pBuf = NULL;
    LPUSER_INFO_0 pTmpBuf;
    DWORD dwLevel = 0;
    DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    DWORD dwResumeHandle = 0;
    DWORD i;
    NET_API_STATUS nStatus;
    LPTSTR pszServerName = NULL;

    //функция получения списка пользователей
    nStatus = NetUserEnum((LPCWSTR)pszServerName,
      dwLevel,
      FILTER_NORMAL_ACCOUNT, // global users
      (LPBYTE*)&pBuf,
      dwPrefMaxLen,
      &dwEntriesRead,
      &dwTotalEntries,
      &dwResumeHandle);

    //создание строки names всех имен
    if ((pTmpBuf=pBuf) != NULL)
    {
      for(i = 0; (i < dwEntriesRead); i++)
      {
        names = names+ws2s(pTmpBuf->usri0_name)+'=';
        pTmpBuf++;
      }
    } 
    //отправка готовой строки 
    Local<String> ans = String::NewFromUtf8(isolate,names.c_str());
    args.GetReturnValue().Set(ans);
  }

  void Init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "check", Check);
  }

  NODE_MODULE(addon, Init);

}
