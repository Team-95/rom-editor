#include <node.h>

using v8::Local;
using v8::Object;
using v8::String;

const char* ToCString(Local<String> str);
