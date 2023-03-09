/**
 * @file LogFormatter.cpp
 * @brief
 * @author ziv
 * @email
 * @date 23-3-2.
 * @copyright Copyright (c) 2023年 ziv All rights reserved.
 */

#include "LogFormatter.h"

#include <ctype.h>

#include <functional>
#include <map>

#include "Logging.h"

namespace kafka {
static std::string pattern_ =
    "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T%f:%l%T%m%n";

static std::string levelToString(LogConfiguration::Level level) {
  switch(level) {
#define XX(name) \
    case LogConfiguration::name: \
        return #name; \
        break;

    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);
#undef XX
    default:
      return "UNKNOW";
  }
  return "UNKNOW";
}

class MessageFormatItem : public LogFormatter::FormatItem {
 public:
  MessageFormatItem(const std::string& str = "") {}
  void format(std::ostream& os, std::shared_ptr<Logging> logging) override {
    os << logging->getContent();
  }
};

class LevelFormatItem : public LogFormatter::FormatItem {
 public:
  LevelFormatItem(const std::string& str = "") {}
  void format(std::ostream& os, std::shared_ptr<Logging> logging) override {
    os << levelToString(logging->getLevel());
  }
};

//    class ElapseFormatItem : public LogFormatter::FormatItem {
//    public:
//        ElapseFormatItem(const std::string& str = "") {}
//        void format(std::ostream &os, std::shared_ptr<Logging> logging)
//        override {
//            os << 0;
//        }
//    };

//    class NameFormatItem : public LogFormatter::FormatItem {
//    public:
//        NameFormatItem(const std::string& str = "") {}
//        void format(std::ostream &os, std::shared_ptr<Logging> logging)
//        override {
//            os << logging->getLogger()->getName();
//        }
//    };

class ThreadIdFormatItem : public LogFormatter::FormatItem {
 public:
  ThreadIdFormatItem(const std::string& str = "") {}
  void format(std::ostream& os, std::shared_ptr<Logging> logging) override {
    os << logging->getThreadId();
  }
};

class FiberIdFormatItem : public LogFormatter::FormatItem {
 public:
  FiberIdFormatItem(const std::string& str = "") {}
  void format(std::ostream& os, std::shared_ptr<Logging> logging) override {
    os << logging->getCoroutineId();
  }
};

class ThreadNameFormatItem : public LogFormatter::FormatItem {
 public:
  ThreadNameFormatItem(const std::string& str = "") {}
  void format(std::ostream& os, std::shared_ptr<Logging> logging) override {
    os << logging->getThreadName();
  }
};

class DateTimeFormatItem : public LogFormatter::FormatItem {
 public:
  DateTimeFormatItem(const std::string& format = "%Y-%m-%d %H:%M:%S")
      : format_(format) {
    if (format_.empty()) {
      format_ = "%Y-%m-%d %H:%M:%S";
    }
  }

  void format(std::ostream& os, std::shared_ptr<Logging> logging) override {
    struct tm tm;
    time_t time = logging->getTime();
    localtime_r(&time, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), format_.c_str(), &tm);
    os << buf;
  }

 private:
  std::string format_;
};

class FilenameFormatItem : public LogFormatter::FormatItem {
 public:
  FilenameFormatItem(const std::string& str = "") {}
  void format(std::ostream& os, std::shared_ptr<Logging> logging) override {
    os << logging->getFile();
  }
};

class LineFormatItem : public LogFormatter::FormatItem {
 public:
  LineFormatItem(const std::string& str = "") {}
  void format(std::ostream& os, std::shared_ptr<Logging> logging) override {
    os << logging->getLine();
  }
};

class NewLineFormatItem : public LogFormatter::FormatItem {
 public:
  NewLineFormatItem(const std::string& str = "") {}
  void format(std::ostream& os, std::shared_ptr<Logging> logging) override {
    os << std::endl;
  }
};

class StringFormatItem : public LogFormatter::FormatItem {
 public:
  StringFormatItem(const std::string& str) : string_(str) {}
  void format(std::ostream& os, std::shared_ptr<Logging> logging) override {
    os << string_;
  }

 private:
  std::string string_;
};

class TabFormatItem : public LogFormatter::FormatItem {
 public:
  TabFormatItem(const std::string& str = "") {}
  void format(std::ostream& os, std::shared_ptr<Logging> logging) override {
    os << "\t";
  }

 private:
  std::string string_;
};

void LogFormatter::init() {
  std::vector<std::tuple<std::string, std::string, int>> vec;
  std::string nstr;
  for (size_t i = 0; i < pattern_.size(); ++i) {
    if (pattern_[i] != '%') {
      nstr.append(1, pattern_[i]);
      continue;
    }

    if ((i + 1) < pattern_.size()) {
      if (pattern_[i + 1] == '%') {
        nstr.append(1, '%');
        continue;
      }
    }

    size_t n = i + 1;
    int fmt_status = 0;
    size_t fmt_begin = 0;

    std::string str;
    std::string fmt;
    while (n < pattern_.size()) {
      if (!fmt_status &&
          (!isalpha(pattern_[n]) && pattern_[n] != '{' && pattern_[n] != '}')) {
        str = pattern_.substr(i + 1, n - i - 1);
        break;
      }
      if (fmt_status == 0) {
        if (pattern_[n] == '{') {
          str = pattern_.substr(i + 1, n - i - 1);
          // std::cout << "*" << str << std::endl;
          fmt_status = 1;  // 解析格式
          fmt_begin = n;
          ++n;
          continue;
        }
      } else if (fmt_status == 1) {
        if (pattern_[n] == '}') {
          fmt = pattern_.substr(fmt_begin + 1, n - fmt_begin - 1);
          // std::cout << "#" << fmt << std::endl;
          fmt_status = 0;
          ++n;
          break;
        }
      }
      ++n;
      if (n == pattern_.size()) {
        if (str.empty()) {
          str = pattern_.substr(i + 1);
        }
      }
    }

    if (fmt_status == 0) {
      if (!nstr.empty()) {
        vec.push_back(std::make_tuple(nstr, std::string(), 0));
        nstr.clear();
      }
      vec.push_back(std::make_tuple(str, fmt, 1));
      i = n - 1;
    } else if (fmt_status == 1) {
      std::cout << "pattern parse error: " << pattern_ << " - "
                << pattern_.substr(i) << std::endl;
      error_ = true;
      vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
    }
  }

  if (!nstr.empty()) {
    vec.push_back(std::make_tuple(nstr, "", 0));
  }
  static std::map<std::string, std::function<FormatItem::FormatItemPtr(
                                   const std::string& str)>>
      kFormatItem = {
#define XX(str, C)                                  \
  {                                                 \
    #str, [](const std::string& fmt) {              \
      return FormatItem::FormatItemPtr(new C(fmt)); \
    }                                               \
  }

          XX(m, MessageFormatItem),     // m:消息
          XX(p, LevelFormatItem),       // p:日志级别
          XX(t, ThreadIdFormatItem),    // t:线程id
          XX(n, NewLineFormatItem),     // n:换行
          XX(d, DateTimeFormatItem),    // d:时间
          XX(f, FilenameFormatItem),    // f:文件名
          XX(l, LineFormatItem),        // l:行号
          XX(T, TabFormatItem),         // T:Tab
          XX(F, FiberIdFormatItem),     // F:协程id
          XX(N, ThreadNameFormatItem),  // N:线程名称
#undef XX
      };

  for (auto& i : vec) {
    if (std::get<2>(i) == 0) {
      items_.push_back(
          FormatItem::FormatItemPtr(new StringFormatItem(std::get<0>(i))));
    } else {
      auto it = kFormatItem.find(std::get<0>(i));
      if (it == kFormatItem.end()) {
        items_.push_back(FormatItem::FormatItemPtr(
            new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
        error_ = true;
      } else {
        items_.push_back(it->second(std::get<1>(i)));
      }
    }

    // std::cout << "(" << std::get<0>(i) << ") - (" << std::get<1>(i) << ") -
    // (" << std::get<2>(i) << ")" << std::endl;
  }
  // std::cout << items_.size() << std::endl;
}

std::ostream& LogFormatter::format(std::ostream& os,
                                   std::shared_ptr<kafka::Logging> logging) {
  for (auto item : items_) {
    item->format(os, logging);
  }

  return os;
}
}  // namespace kafka