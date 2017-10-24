#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <regex>
#include <cctype>

using namespace std;

static char *_format_message(const char *fmt, va_list args)
{
  if(!fmt)
    return g_strdup("");

  return g_strdup_vprintf(fmt, args);
}

extern "C" void debug_printf(const char *fmt, ...)
{
  const char *debug = getenv("NODE_GIR_DEBUG");
  if (debug == NULL || *debug == '\0') {
    return;
  }
  printf(" DEBUG: ");

  va_list args;
  va_start (args, fmt);
  char *msg = _format_message(fmt, args);
  va_end(args);

  printf("%s", msg);
  g_free(msg);
}

vector<string> split_string(const string &text, const regex &re, bool include_full_match = false) {
  auto parts = vector<string>(
    sregex_token_iterator(text.begin(), text.end(), re, -1),
    sregex_token_iterator()
  );
  return parts;
}

namespace gir {
namespace Util {

gchar *utf8StringFromValue(v8::Handle<v8::Value> value) {
  v8::Local<v8::String> str = value->ToString();
  gchar *buffer = g_new(gchar, str->Utf8Length());
  str->WriteUtf8(buffer);
  return buffer;
}

static regex underscore_re = regex("_");

/**
 * This function returns a new string, converting the input
 * string from snake case to camel case.
 * e.g. 'set_label' --> 'setLabel'
 */
string toCamelCase(const string input) {
  vector<string> words = split_string(input, underscore_re);
  stringstream output;
  for (auto it = words.begin(); it != words.end(); it++) {
    auto word = *it;
    if (word.size() >= 1 && it != words.begin()) {
      word[0] = toupper(word[0]);
    }
    output << word;
  }
  return output.str();
}

string toSnakeCase(const string input) {
  string output;
  for (auto letter : input) {
    if (isupper(letter)) { // FIXME: this will break on methods that begin with a capital (i should write a test case!)
      output.append(1, '_');
      output.append(1, (char)tolower(letter));
    } else {
      output.append(1, letter);
    }
  }
  return output;
}

}
}
