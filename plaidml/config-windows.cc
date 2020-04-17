// Copyright 2019 Intel Corporation.

#include "plaidml/config.h"

#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/filesystem.hpp>

#include "base/config/config.h"
#include "base/util/env.h"

namespace vertexai {
namespace plaidml {
namespace config {

namespace {

const char* kPlaidMLExperimental = "0"; //"PLAIDML_EXPERIMENTAL";
const char* kPlaidMLDefaultConfig = "PLAIDML_DEFAULT_CONFIG";
const char* kPlaidMLDefaultConfigBasename = "config.json";
const char* kPlaidMLExperimentalConfig = "PLAIDML_EXPERIMENTAL_CONFIG";
const char* kPlaidMLExperimentalConfigBasename = "experimental.json";

boost::filesystem::path GetConfigFilename(const char* env_var, const char* basename) {
  std::string config_filename = vertexai::env::Get(env_var);
  if (!config_filename.empty()) {
    return config_filename;
  }

  // Assuming that the PlaidML library has been properly installed, we
  // should be able to find the config file from the library's path.
  auto path = boost::dll::symbol_location_ptr(&GetConfigFilename).lexically_normal();
  path.remove_filename();  // Trim to "/Library/bin"
  path.remove_filename();  // Trim to "/Library"
  path.remove_filename();  // Trim to root install directory
  path /= "share";
  path /= "plaidml";
  path /= basename;
  return path;
}

}  // namespace

Config Get() {
  Config config;
  //std::string exp = vertexai::env::Get(kPlaidMLExperimental);
  boost::filesystem::path source;
  //if (!exp.empty() && exp != "0") {
  //  source = GetConfigFilename(kPlaidMLExperimentalConfig, kPlaidMLExperimentalConfigBasename);
  //} else {
    source = GetConfigFilename(kPlaidMLDefaultConfig, kPlaidMLDefaultConfigBasename);
  //}
  config.source = source.string();
  std::cout << "config.source=" << config.source << "\n";
  std::ifstream cfs(source.native());
  config.data.assign(std::istreambuf_iterator<char>(cfs), std::istreambuf_iterator<char>());

  return config;
}

}  // namespace config
}  // namespace plaidml
}  // namespace vertexai
