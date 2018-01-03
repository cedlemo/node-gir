#pragma once

#include <girepository.h>
#include <glib.h>
#include <v8.h>

namespace gir {

using namespace v8;

class GIRValue {
public:
    static Local<Value> from_g_value(GValue *v, GIBaseInfo *base_info);
    static bool to_g_value(Local<Value> value, GType t, GValue *v);
    static GType guess_type(Local<Value> value);
};

} // namespace gir
