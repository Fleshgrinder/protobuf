// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: Richard Fussenegger <fleshgrinder@users.noreply.github.com>

#include <google/protobuf/compiler/kotlin/kotlin_utils.h>

#include <string>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/stubs/logging.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace kotlin {

using std::string;

string formatCamelCase(
    const string &name,
    const string &ignore,
    const bool upper_case
) {
  GOOGLE_CHECK(!name.empty());
  string result;
  bool seen_separator = upper_case;
  bool seen_upper_case = !upper_case;

  for (char c : name) {
    if (*&ignore != nullptr && ignore.find(c) != string::npos) {
      result += c;
      seen_separator = upper_case;
      seen_upper_case = !upper_case;
    } else if ('0' <= c && c <= '9') {
      result += c;
      seen_separator = false;
      seen_upper_case = false;
    } else if ('a' <= c && c <= 'z') {
      result += seen_separator ? c + ('A' - 'a') : c;
      seen_separator = false;
      seen_upper_case = false;
    } else if ('A' <= c && c <= 'Z') {
      result += seen_upper_case ? c + ('a' - 'A') : c;
      seen_separator = false;
      seen_upper_case = true;
    } else if (!result.empty()) {
      seen_separator = true;
      seen_upper_case = false;
    }
  }

  return result;
}

string ToLowerCamelCase(const string &name, const string &ignore) {
  return formatCamelCase(name, ignore, false);
}

string ToUpperCamelCase(const string &name, const string &ignore) {
  return formatCamelCase(name, ignore, true);
}

string GetKotlinPackage(const FileDescriptor *file) {
  // TODO(Fleshgrinder) do we want a Kotlin package option?
  //if (file->options().has_kotlin_package()) {
  //  package = file->options().kotlin_package();
  //} else
  return file->package().empty() ? "" : ToLowerCamelCase(file->package(), ".");
}

string KotlinPackageToDir(const string &package) {
  string dir = package;

  for (char &c : dir) {
    if (c == '.') {
      c = '/';
    }
  }
  if (!dir.empty()) {
    dir += '/';
  }

  return dir;
}

KotlinType GetKotlinType(const FieldDescriptor *field) {
  switch (field->type()) {
    case FieldDescriptor::TYPE_INT32:
    case FieldDescriptor::TYPE_SINT32:
    case FieldDescriptor::TYPE_FIXED32:
    case FieldDescriptor::TYPE_SFIXED32:
      return KOTLIN_TYPE_INT;

    case FieldDescriptor::TYPE_UINT32:
      return KOTLIN_TYPE_UINT;

    case FieldDescriptor::TYPE_INT64:
    case FieldDescriptor::TYPE_SINT64:
    case FieldDescriptor::TYPE_FIXED64:
    case FieldDescriptor::TYPE_SFIXED64:
      return KOTLIN_TYPE_LONG;

    case FieldDescriptor::TYPE_UINT64:
      return KOTLIN_TYPE_ULONG;

    case FieldDescriptor::TYPE_FLOAT:
      return KOTLIN_TYPE_FLOAT;

    case FieldDescriptor::TYPE_DOUBLE:
      return KOTLIN_TYPE_DOUBLE;

    case FieldDescriptor::TYPE_BOOL:
      return KOTLIN_TYPE_BOOLEAN;

    case FieldDescriptor::TYPE_STRING:
      return KOTLIN_TYPE_STRING;

    case FieldDescriptor::TYPE_BYTES:
      return KOTLIN_TYPE_BYTE_ARRAY;

    case FieldDescriptor::TYPE_ENUM:
      return KOTLIN_TYPE_ENUM;

    case FieldDescriptor::TYPE_GROUP:
    case FieldDescriptor::TYPE_MESSAGE:
      return KOTLIN_TYPE_MESSAGE;
  }

  GOOGLE_LOG(FATAL) << "entered unreachable code";
  return KOTLIN_TYPE_INT;
}

const char* KotlinTypeName(const KotlinType type) {
  switch (type) {
    case KOTLIN_TYPE_INT:
      return "Int";

    case KOTLIN_TYPE_INT_NULL:
      return "Int?";

    case KOTLIN_TYPE_UINT:
      return "UInt";

    case KOTLIN_TYPE_UINT_NULL:
      return "UInt?";

    case KOTLIN_TYPE_LONG:
      return "Long";

    case KOTLIN_TYPE_LONG_NULL:
      return "Long?";

    case KOTLIN_TYPE_ULONG:
      return "ULong";

    case KOTLIN_TYPE_ULONG_NULL:
      return "ULong?";

    case KOTLIN_TYPE_FLOAT:
      return "Float";

    case KOTLIN_TYPE_FLOAT_NULL:
      return "Float?";

    case KOTLIN_TYPE_DOUBLE:
      return "Double";

    case KOTLIN_TYPE_DOUBLE_NULL:
      return "Double?";

    case KOTLIN_TYPE_BOOLEAN:
      return "Boolean";

    case KOTLIN_TYPE_BOOLEAN_NULL:
      return "Boolean?";

    case KOTLIN_TYPE_STRING:
      return "String";

    case KOTLIN_TYPE_STRING_NULL:
      return "String?";

    case KOTLIN_TYPE_BYTE_ARRAY:
      return "ByteArray";

    case KOTLIN_TYPE_BYTE_ARRAY_NULL:
      return "ByteArray?";

    case KOTLIN_TYPE_ENUM:
    case KOTLIN_TYPE_MESSAGE:
      return nullptr;
  }

  GOOGLE_LOG(FATAL) << "entered unreachable code";
  return nullptr;
}

bool IsKeyword(const string &name) {
  static string keywords[] = {
      "as",
      "break",
      "class",
      "continue",
      "do",
      "else",
      "false",
      "for",
      "fun",
      "if",
      "in",
      "interface",
      "is",
      "null",
      "object",
      "package",
      "return",
      "super",
      "this",
      "throw",
      "true",
      "try",
      "typealias",
      "typeof",
      "val",
      "var",
      "when",
      "while",
  };

  for (const string &keyword : keywords) {
    if (name == keyword) {
      return true;
    }
  }
  return false;
}

const char *KotlinDefaultValue(const FieldDescriptor *field) {
  switch (field->type()) {
    case FieldDescriptor::TYPE_GROUP:
    case FieldDescriptor::TYPE_MESSAGE:
      return "null";

    case FieldDescriptor::TYPE_ENUM:
      return "TODO()";

    case FieldDescriptor::TYPE_BYTES:
      return "byteArrayOf()";

    case FieldDescriptor::TYPE_STRING:
      return "\"\"";

    case FieldDescriptor::TYPE_BOOL:
      return "false";

    case FieldDescriptor::TYPE_DOUBLE:
      return "0.0";

    case FieldDescriptor::TYPE_FLOAT:
      return "0f";

    case FieldDescriptor::TYPE_UINT64:
      return "0uL";

    case FieldDescriptor::TYPE_SFIXED64:
    case FieldDescriptor::TYPE_FIXED64:
    case FieldDescriptor::TYPE_SINT64:
    case FieldDescriptor::TYPE_INT64:
      return "0L";

    case FieldDescriptor::TYPE_UINT32:
      return "0u";

    case FieldDescriptor::TYPE_SFIXED32:
    case FieldDescriptor::TYPE_FIXED32:
    case FieldDescriptor::TYPE_SINT32:
    case FieldDescriptor::TYPE_INT32:
      return "0";
  }

  GOOGLE_LOG(FATAL) << "entered unreachable code";
  return nullptr;
}

}  // namespace kotlin
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
