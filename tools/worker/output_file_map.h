// Copyright 2019 The Bazel Authors. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef BUILD_BAZEL_RULES_SWIFT_TOOLS_WORKER_OUTPUT_FILE_MAP_H
#define BUILD_BAZEL_RULES_SWIFT_TOOLS_WORKER_OUTPUT_FILE_MAP_H

#include <string>

#include "absl/container/btree_map.h"
#include "absl/strings/string_view.h"
#include <nlohmann/json.hpp>

namespace bazel_rules_swift {

// Supports loading and rewriting a `swiftc` output file map to support
// incremental compilation.
//
// See
// https://github.com/apple/swift/blob/master/docs/Driver.md#output-file-maps
// for more information on how the Swift driver uses this file.
class OutputFileMap {
 public:
  explicit OutputFileMap() {}

  // The in-memory JSON-based representation of the output file map.
  const nlohmann::json &json() const { return json_; }

  // A map containing expected output files that will be generated in the
  // incremental storage area. The key is the original object path; the
  // corresponding value is its location in the incremental storage area.
  const absl::btree_map<std::string, std::string> &incremental_outputs() const {
    return incremental_outputs_;
  }

  // Reads the output file map from the JSON file at the given path, and updates
  // it to support incremental builds.
  void ReadFromPath(absl::string_view path);

  // Writes the output file map as JSON to the file at the given path.
  void WriteToPath(absl::string_view path);

 private:
  // Modifies the output file map's JSON structure in-place to replace file
  // paths with equivalents in the incremental storage area.
  void UpdateForIncremental(absl::string_view path);

  nlohmann::json json_;
  absl::btree_map<std::string, std::string> incremental_outputs_;
};

}  // namespace bazel_rules_swift

#endif  // BUILD_BAZEL_RULES_SWIFT_TOOLS_WORKER_OUTPUT_FILE_MAP_H_
