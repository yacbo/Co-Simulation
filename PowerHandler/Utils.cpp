#include "Utils.hpp"
using std::vector;
using std::string;

extern "C" {
typedef api::Api* (__cdecl *CREATEAPI)(const char* username, const char* password, const char* commandLineArguments);
typedef void (__cdecl *DESTROYAPI)(api::Api*&);
}

//#undef GetClassName

api::Api* ApiFixture::instance = 0;
ApiFixture::ApiFixture()
{
  //create an api instance
  dllHandle = LoadLibraryA("digapi.dll"); ///< handle to loaded digapi.dll

  if (!dllHandle) {
    return;
  }

  CREATEAPI createApi = (CREATEAPI) GetProcAddress ((struct HINSTANCE__*)dllHandle, "CreateApiInstance");
  std::cout << "Creating API instance..." << std::endl;
  instance = createApi(NULL, NULL, NULL);
  if (instance) {
    std::cout << "API instance successfully created." << std::endl;
  }
  else{
      std::cout << "API instance failed." << std::endl;
      return;
  }

  int error(0);
  api::Value cmd("rcom/show");
  instance->GetApplication()->Execute("ExecuteCmd", &cmd, &error);//显示软件界面
  if (error > 0)
  {
	  std::cout << "GUI window shows error." << std::endl;
  }
}

ApiFixture::~ApiFixture()
{
  //note: destroying the api instance will immediately terminate the whole process as this is the way
  //      how PowerFactory is closed
  if (dllHandle) {
    std::cout << "Releasing API instance..." << std::endl;
    DESTROYAPI destroyApi = (DESTROYAPI) GetProcAddress ((struct HINSTANCE__*)dllHandle, "DestroyApiInstance");
    destroyApi(instance);
    instance = NULL;

    FreeLibrary(dllHandle);
    dllHandle = NULL;
  }
}

ValueGuard::~ValueGuard()
{
  if (ApiFixture::GetInstance() && m_val) {
    ApiFixture::GetInstance()->ReleaseValue(m_val);
  }
}

DataObjVec Utils::GetChildren(api::DataObject* parent, std::string classNameFilter)
{
  DataObjVec result;

  ValueGuard children(parent->GetChildren(FALSE));
  for (size_t i=0, count = children->VecGetSize(); i < count; ++i) {
    api::DataObject* child = children->VecGetDataObject(i);
    if (classNameFilter.empty()) {
      result.push_back(child);
    }
    else {
      ValueGuard classname(child->GetClassName());
      if (strcmp(classname->GetString(), classNameFilter.c_str()) == 0) {
        result.push_back(child);
      }
    }
  }

  return result;
}
