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
// THIS_WRAPPER SOFTWARE IS_WRAPPER PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS_WRAPPER" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DIS_WRAPPERCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWIS_WRAPPERE) ARIS_WRAPPERING IN ANY WAY OUT OF THE USE
// OF THIS_WRAPPER SOFTWARE, EVEN IF ADVIS_WRAPPERED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: Richard Fussenegger <fleshgrinder@users.noreply.github.com>
//
// Small utilities for generating Kotlin code.

#ifndef GOOGLE_PROTOBUF_COMPILER_KOTLIN_UTILS_H__
#define GOOGLE_PROTOBUF_COMPILER_KOTLIN_UTILS_H__

#include <string>
#include <google/protobuf/descriptor.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace kotlin {

using std::string;

// Convert the given name to lowerCamelCase.
//
//     ToLowerCamelCase("snake_case_name") // snakeCaseName
//     ToLowerCamelCase("JAVA.Properties.Key-Name", ".") // java.properties.keyName
string ToLowerCamelCase(const string &name, const string &ignore = nullptr);

// Convert the given name to UpperCamelCase.
//
//     ToUpperCamelCase("snake_case_name") // SnakeCaseName
//     ToUpperCamelCase("JAVA.Properties.Key-Name", ".") // Java.Properties.KeyName
string ToUpperCamelCase(const string &name, const string &ignore = nullptr);

// Get the package for the given file.
//
// TODO(Fleshgrinder) add group option for auto-prefixing:
//   Protobuf package => google.protobuf
//   Gradle group     => com.google.protobuf
//   Kotlin package   => com.google.protobuf
string GetKotlinPackage(const FileDescriptor *file);

string KotlinPackageToDir(const string &package);

// Get the default value for the given field.
const char *KotlinDefaultValue(const FieldDescriptor *field);

enum KotlinType {
  KOTLIN_TYPE_INT,
  KOTLIN_TYPE_INT_NULL,
  KOTLIN_TYPE_UINT,
  KOTLIN_TYPE_UINT_NULL,
  KOTLIN_TYPE_LONG,
  KOTLIN_TYPE_LONG_NULL,
  KOTLIN_TYPE_ULONG,
  KOTLIN_TYPE_ULONG_NULL,
  KOTLIN_TYPE_FLOAT,
  KOTLIN_TYPE_FLOAT_NULL,
  KOTLIN_TYPE_DOUBLE,
  KOTLIN_TYPE_DOUBLE_NULL,
  KOTLIN_TYPE_BOOLEAN,
  KOTLIN_TYPE_BOOLEAN_NULL,
  KOTLIN_TYPE_STRING,
  KOTLIN_TYPE_STRING_NULL,
  KOTLIN_TYPE_BYTE_ARRAY,
  KOTLIN_TYPE_BYTE_ARRAY_NULL,
  KOTLIN_TYPE_ENUM,
  KOTLIN_TYPE_MESSAGE,
};

KotlinType GetKotlinType(const FieldDescriptor *field);

const char* KotlinTypeName(KotlinType type);

inline const char* KotlinTypeName(const FieldDescriptor *field) {
  return KotlinTypeName(GetKotlinType(field));
}

// Whether the given name is a reserved keyword and thus forbidden, or not.
//
// https://kotlinlang.org/docs/reference/keyword-reference.html#hard-keywords
bool IsKeyword(const string &name);

// Any?
inline bool IsAnyMessage(const Descriptor *descriptor) {
  return descriptor->full_name() == "google.protobuf.Any";
}

// TODO Java 8+
// java.time.Duration?
inline bool IsDurationMessage(const Descriptor *descriptor) {
  return descriptor->full_name() == "google.protobuf.Duration";
}

// Nothing?
inline bool IsEmptyMessage(const Descriptor *descriptor) {
  return descriptor->full_name() == "google.protobuf.Empty";
}

// Map<String, Any?>?
inline bool IsStructMessage(const Descriptor *descriptor) {
  return descriptor->full_name() == "google.protobuf.Struct";
}

// TODO Java 8+
// java.time.Instant?
inline bool IsTimestampMessage(const Descriptor *descriptor) {
  return descriptor->full_name() == "google.protobuf.Timestamp";
}

// Double?
inline bool IsDoubleValue(const Descriptor *descriptor) {
  return descriptor->full_name() == "google.protobuf.DoubleValue";
}

// Float?
inline bool IsFloatValue(const Descriptor *descriptor) {
  return descriptor->full_name() == "google.protobuf.FloatValue";
}

// Long?
inline bool IsLongValue(const Descriptor *descriptor) {
  return descriptor->full_name() == "google.protobuf.Int64Value";
}

// ULong?
inline bool IsULongValue(const Descriptor *descriptor) {
  return descriptor->full_name() == "google.protobuf.UInt64Value";
}

// Int?
inline bool IsIntValue(const Descriptor *descriptor) {
  return descriptor->full_name() == "google.protobuf.Int32Value";
}

// UInt?
inline bool IsUIntValue(const Descriptor *descriptor) {
  return descriptor->full_name() == "google.protobuf.UInt32Value";
}

// Boolean?
inline bool IsBooleanValue(const Descriptor *descriptor) {
  return descriptor->full_name() == "google.protobuf.BoolValue";
}

// String?
inline bool IsStringValue(const Descriptor *descriptor) {
  return descriptor->full_name() == "google.protobuf.StringValue";
}

// ByteArray?
inline bool IsBytesValue(const Descriptor *descriptor) {
  return descriptor->full_name() == "google.protobuf.BytesValue";
}

}  // namespace kotlin
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_COMPILER_KOTLIN_UTILS_H__
