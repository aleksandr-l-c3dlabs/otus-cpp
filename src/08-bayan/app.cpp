#include "app.h"

#include <algorithm>
#include <filesystem>
#include <regex>
#include <string>
#include <vector>
#include "lib.h"

// #include <iostream>

namespace fs = std::filesystem;

struct Mask {
  std::regex re;
};

void validate(boost::any& v, const std::vector<std::string>& values, HashAlg*,
              int) {
  // Make sure no previous assignment to 'a' was made.
  po::validators::check_first_occurrence(v);
  // Extract the first string from 'values'. If there is more than
  // one string, it's an error, and exception will be thrown.
  const std::string& s = po::validators::get_single_string(values);

  std::string str;
  str.reserve(s.size());
  std::transform(s.begin(), s.end(), std::back_inserter(str),
                 (int (*)(int))std::tolower);

  if (str == "crc32") {
    v = HashAlg::Crc32;
  } else if (str == "md5") {
    v = HashAlg::Md5;
  } else if (str == "sha1") {
    v = HashAlg::Sha1;
  } else {
    throw po::validation_error(po::validation_error::invalid_option_value,
                               "hash-alg", "unknown hash algorithm");
  }
}

void validate(boost::any& v, const std::vector<std::string>& values,
              std::vector<Mask>*, int) {
  if (v.empty())
    v = std::vector<Mask>{};

  auto result = boost::any_cast<std::vector<Mask>>(&v);
  result->reserve(result->size() + values.size());
  for (const auto& str : values)
    try {
      std::regex re(str, std::regex::icase);
      result->push_back(Mask{re});
    } catch (const std::regex_error&) {
      throw po::validation_error(po::validation_error::invalid_option_value,
                                 "mask", "must be a valid regex");
    }
}

namespace boost {
template <>
std::string lexical_cast(const HashAlg& arg) {
  switch (arg) {
    case HashAlg::Crc32:
      return "crc32";

    case HashAlg::Md5:
      return "md5";

    case HashAlg::Sha1:
      return "sha1";
  }

  return "";
}
}  // namespace boost

void fillFinder(EqualFinder& finder, const fs::path& path,
                const std::vector<fs::path>& ignore,
                const std::function<bool(const fs::path&)>& isValid,
                unsigned level) {
  if (!fs::exists(path) || !fs::is_directory(path))
    return;
  for (const auto& entry : fs::directory_iterator(path)) {
    if (entry.is_directory()) {
      if (level == 0)
        continue;
      auto it =
          std::find(ignore.begin(), ignore.end(), fs::absolute(entry.path()));
      if (it != ignore.end())
        continue;
      fillFinder(finder, entry, ignore, isValid, level - 1);
    } else if (isValid(entry.path())) {
      finder.put(fs::absolute(entry.path()));
    }
  }
}

Application::Application(const int argc, const char* argv[])
    : desc("Allowed aoptions") {
  desc.add_options()("help,h", "Produce help message")(
      "folder,f",
      po::value<std::vector<std::string>>()->multitoken()->required(),
      "Folder path for scaning")(
      "ignore,i", po::value<std::vector<std::string>>()->multitoken(),
      "Folder path for exclude from scaning")(
      "level,l", po::value<unsigned>()->default_value(0),
      "Scanning level (default only folder files)")(
      "mask,m", po::value<std::vector<Mask>>()->multitoken(),
      "File masks for scaning")("file-size,s",
                                po::value<std::size_t>()->default_value(1),
                                "Minimal file size, bytes")(
      "block-size,S", po::value<std::size_t>()->required(),
      "Reading block size, bytes")(
      "hash-alg,H", po::value<HashAlg>()->default_value(HashAlg::Crc32),
      "Hash algorithm, available crc32, md5, sha1");

  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  } catch (const std::exception& er) {
    error = er.what();
  }
}

std::vector<std::vector<std::string>> Application::diff_groups() const {
  std::vector<fs::path> ignore;
  if (vm.count("ignore"))
    for (const auto& str : vm["ignore"].as<std::vector<std::string>>()) {
      fs::path path{str};
      if (!fs::exists(path))
        continue;
      ignore.push_back(fs::absolute(path));
    }
  std::function<bool(const fs::path&)> isMasked = [](const fs::path&) {
    return true;
  };
  std::vector<std::regex> masks;
  if (vm.count("mask")) {
    for (const auto& m : vm["mask"].as<std::vector<Mask>>())
      masks.push_back(m.re);
    isMasked = [&masks](const fs::path& path) {
      for (const auto& re : masks)
        if (std::regex_match(path.filename().string(), re))
          return true;
      return false;
    };
  }
  uintmax_t minSize = vm["file-size"].as<std::size_t>();
  auto level = vm["level"].as<unsigned>();
  auto isValid = [&isMasked, &minSize](const fs::path& path) {
    return isMasked(path) && fs::file_size(path) >= minSize;
  };

  EqualFinder finder(vm["hash-alg"].as<HashAlg>(),
                     vm["block-size"].as<std::size_t>());

  for (const auto& str : vm["folder"].as<std::vector<std::string>>()) {
    fillFinder(finder, fs::path{str}, ignore, isValid, level);
  }

  return finder.getResult();
}