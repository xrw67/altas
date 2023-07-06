#include "bbt/appkit/service.h"

#include <gmock/gmock.h>

namespace bbt {

class MockService : public Service {
 public:
  static Service* New(const char* name) {
    return reinterpret_cast<Service*>(new MockService(name));
  }

  const char* service_name() const { return name_; }

 private:
  MockService(const char* name) : name_(name) {}

  const char* name_;
};

TEST(Service, Register) {
  ServiceManager* mgr = ServiceManager::New();
  Service* svc1 = MockService::New("svc1");
  svc1->AddRefManually();

  // Register
  ASSERT_EQ(mgr->Register(NULL).code(), StatusCode::kInvalidArgument);
  ASSERT_EQ(mgr->Register(MockService::New(NULL)).code(),
            StatusCode::kInvalidArgument);
  ASSERT_EQ(mgr->Register(MockService::New("")).code(),
            StatusCode::kInvalidArgument);

  ASSERT_EQ(mgr->Register(svc1).code(), StatusCode::kOk);
  ASSERT_EQ(mgr->Register(svc1).code(), StatusCode::kAlreadyExists);

  auto svcs = mgr->List();
  ASSERT_EQ(svcs.size(), 1);
  ASSERT_EQ(svcs[0], "svc1");

  // Refcnt
  Service* client1 = NULL;
  Service* client2 = NULL;

  ASSERT_EQ(mgr->Get(NULL, NULL).code(), StatusCode::kInvalidArgument);
  ASSERT_EQ(mgr->Get("", NULL).code(), StatusCode::kInvalidArgument);
  ASSERT_EQ(mgr->Get("svc666", NULL).code(), StatusCode::kInvalidArgument);
  ASSERT_EQ(mgr->Get("svc666", &client1).code(), StatusCode::kNotFound);

  ASSERT_EQ(mgr->Get("svc1", &client1).code(), StatusCode::kOk);
  ASSERT_EQ(mgr->Get("svc1", &client2).code(), StatusCode::kOk);

  ASSERT_EQ(client1, client2);
  ASSERT_STREQ(client1->service_name(), "svc1");
  ASSERT_EQ(client1->ref_count(), 4);

  mgr->Put(client2);
  ASSERT_EQ(client1->ref_count(), 3);

  mgr->Put(client1);
  ASSERT_EQ(client2->ref_count(), 2);

  // Unregister
  ASSERT_EQ(mgr->Unregister(NULL).code(), StatusCode::kInvalidArgument);
  ASSERT_EQ(mgr->Unregister(MockService::New(NULL)).code(),
            StatusCode::kInvalidArgument);
  ASSERT_EQ(mgr->Unregister(MockService::New("")).code(),
            StatusCode::kInvalidArgument);
  ASSERT_EQ(mgr->Unregister(svc1).code(), StatusCode::kOk);
  ASSERT_EQ(mgr->Unregister(svc1).code(), StatusCode::kNotFound);

  ServiceManager::Release(mgr);
  svc1->RemoveRefManually();
}

}  // namespace bbt