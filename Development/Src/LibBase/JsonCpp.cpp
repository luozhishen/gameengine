#include "ZionBase.h"

#pragma warning(disable:4127)
#pragma warning(disable:4167)

#include "jsoncpp/include/json/config.h"
#include "jsoncpp/include/json/jsoncpp.h"
#include "jsoncpp/include/json/autolink.h"
#include "jsoncpp/include/json/value.h"
#include "jsoncpp/include/json/reader.h"
#include "jsoncpp/include/json/writer.h"
#include "jsoncpp/include/json/features.h"
#include "jsoncpp/include/json/forwards.h"

#define JSON_ASSERT_MESSAGE( condition, message ) assert( condition )

#include "jsoncpp/src/lib_json/json_reader.cpp"
#include "jsoncpp/src/lib_json/json_value.cpp"
#include "jsoncpp/src/lib_json/json_writer.cpp"
