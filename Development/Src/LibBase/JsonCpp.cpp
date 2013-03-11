#if 1

#include "AtlasBase.h"

#include <json/config.h>
#include <json/jsoncpp.h>
#include <json/autolink.h>
#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/features.h>
#include <json/forwards.h>

#define JSON_ASSERT_MESSAGE( condition, message ) assert( condition )

#include <json_reader.cpp>
#include <json_value.cpp>
#include <json_writer.cpp>

#endif
