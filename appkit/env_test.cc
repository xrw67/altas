#include "bbt/appkit/env.h"

#include <gmock/gmock.h>

#include "bbt/appkit/object.h"

namespace bbt {

class MockService : public Object {
 public:
  static Object* New(const char* name) {
    return reinterpret_cast<Object*>(new MockService(name));
  }

  const char* obj_name() const { return name_; }

 private:
  MockService(const char* name) : name_(name) {}

  const char* name_;
};

TEST(Env, Object) {
  GlobalEnv* env = GlobalEnv::New();
  Object* svc1 = MockService::New("svc1");
  svc1->AddRefManually();

  // InsertObject
  ASSERT_EQ(env->InsertObject(NULL).code(), StatusCode::kInvalidArgument);
  ASSERT_EQ(env->InsertObject(MockService::New(NULL)).code(),
            StatusCode::kInvalidArgument);
  ASSERT_EQ(env->InsertObject(MockService::New("")).code(),
            StatusCode::kInvalidArgument);

  ASSERT_EQ(env->InsertObject(svc1).code(), StatusCode::kOk);
  ASSERT_EQ(env->InsertObject(svc1).code(), StatusCode::kAlreadyExists);

  // Refcnt
  Object* client1 = NULL;
  Object* client2 = NULL;

  ASSERT_EQ(env->GetObject(NULL, NULL).code(), StatusCode::kInvalidArgument);
  ASSERT_EQ(env->GetObject("", NULL).code(), StatusCode::kInvalidArgument);
  ASSERT_EQ(env->GetObject("svc666", NULL).code(),
            StatusCode::kInvalidArgument);
  ASSERT_EQ(env->GetObject("svc666", &client1).code(), StatusCode::kNotFound);

  ASSERT_EQ(env->GetObject("svc1", &client1).code(), StatusCode::kOk);
  ASSERT_EQ(env->GetObject("svc1", &client2).code(), StatusCode::kOk);

  ASSERT_EQ(client1, client2);
  ASSERT_STREQ(client1->obj_name(), "svc1");
  ASSERT_EQ(client1->ref_count(), 4);

  client2->Release();
  ASSERT_EQ(client1->ref_count(), 3);

  client1->Release();
  ASSERT_EQ(client2->ref_count(), 2);

  // RemoveObject
  env->RemoveObject(NULL);
  env->RemoveObject("");
  env->RemoveObject("svc1");
  env->RemoveObject("svc1");
  env->RemoveObject("svc666");
  ASSERT_EQ(env->GetObject("svc1", &client1).code(), StatusCode::kNotFound);

  GlobalEnv::Release(env);
  svc1->Release();
}

TEST(Env, String) {
  GlobalEnv* env = GlobalEnv::New();

  env->InsertString(NULL, NULL);
  env->InsertString("", NULL);
  env->InsertString("key1", NULL);

  ASSERT_EQ(env->GetString(NULL), "");
  ASSERT_EQ(env->GetString(""), "");
  ASSERT_EQ(env->GetString("key1"), "");

  env->InsertString("key1", "value1");
  ASSERT_EQ(env->GetString("key1"), "value1");

  env->RemoveString(NULL);
  env->RemoveString("");
  env->RemoveString("key1");

  ASSERT_EQ(env->GetString("key1"), "");

  GlobalEnv::Release(env);
}

}  // namespace bbt