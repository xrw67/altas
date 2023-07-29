#ifndef BBT_HTTP_CLIENT_H_
#define BBT_HTTP_CLIENT_H_

#include <string>

#include "bbt/base/status.h"
#include "bbt/http/response.h"

namespace bbt {
namespace http {

Status Get(const std::string& address, const std::string& port, Response* rep);

}
}  // namespace bbt

#endif  // BBT_HTTP_CLIENT_H_
